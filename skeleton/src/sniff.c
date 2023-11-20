#include "analysis.h"
#include "sniff.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pcap.h>
#include <netinet/if_ether.h>

#include "dispatch.h"
#include <netinet/ip.h>

#include <netinet/in.h> //for ntohs function - ntohs converts the network byte order to the host byte order. 
#include <netinet/tcp.h>
#include <arpa/inet.h>



struct pcap_pkthdr header;
const unsigned char *packet;
pcap_t *pcap_handle;

// Declare a global variable to indicate whether Ctrl+C was pressed
// volatile sig_atomic_t ctrl_c_pressed = 0;
extern int synPacketsCount;
extern int uniqueIPArraySize;
extern struct SourceIPArray uniqueSourceIPs;




/**
 * Handles a captured packet.
 * 
 * @param verbose If set to 1, the raw packet is dumped to the terminal. 
 * @param pkthdr The pcap packet header containing information about the captured packet. 
 * @param packet The raw packet data 
 * 
  */
void packetHandler(int verbose, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
  //if verbose is set to 1, dump raw packet to terminal
  if (verbose ){
    dump(packet, pkthdr->len);
  }
  // Dispatch packet for processing
  dispatch(pkthdr, packet, verbose);
}


// Signal handler for Ctrl+C
//
void handle_ctrl_c(int signum) {
  if (signum == SIGINT ) {
    printf("Intrusion Detection Report:\n");
    printf("%d SYN packets detected from %d different IPs (syn attack)\n", synPacketsCount, uniqueIPArraySize);
    freeSourceIPArray(&uniqueSourceIPs);
  }

  if (pcap_handle != NULL) {
    pcap_breakloop(pcap_handle);
  }

  // Close the pcap handle when packet capture is complete. 
  if (pcap_handle != NULL) {
    pcap_close(pcap_handle);
  }

  exit(0);
}


  // Open the specified network interface for packet capture. pcap_open_live() returns the handle to be used for the packet
  // capturing session. check the man page of pcap_open_live()
/**
 * Begins packet sniffing on a designated network interface. Application main sniffing loop
 * 
 * @param interface The name of the network interface for packet capture. 
 * @param verbose Determines whether to display detailed information about each packet. 1=true.  
 * 
  */ 
void sniff(char *interface, int verbose) {

  // Register the signal handler for Ctrl+C
  signal(SIGINT, handle_ctrl_c);

  char errbuf[PCAP_ERRBUF_SIZE];
  //open the specified network interface for paket capture
  pcap_handle = pcap_open_live(interface, 4096, 1, 1000, errbuf);

  //check if the pcaphandle was successfully created. 
  if (pcap_handle == NULL) {
    fprintf(stderr, "Unable to open interface %s\n", errbuf);
    exit(EXIT_FAILURE);
  } else {
    printf("SUCCESS! Opened %s for capture\n", interface);
  }

    //use pcap_loop to efficiently capture packets. 
  if (pcap_loop(pcap_handle, -1, packetHandler, verbose) < 0){
    fprintf(stderr, "Error in pcap_loop: %s\n", pcap_geterr(pcap_handle));
    exit(EXIT_FAILURE);
  }
  

}

/**
 * Utility/Debugging method for dumping raw packet data. For decoding link layer header.
 * Dump is used to print the packet data in ASCII when verbose mode is enabled.
 * 
 * @param data raw packet data to be decoded and dumped. 
 * @param length length of the raw packet data. 
 * 
  */ 
void dump(const unsigned char *data, int length){
  unsigned int i;
  static unsigned long pcount = 0;
  // Decode Packet Header
  struct ether_header *eth_header = (struct ether_header *) data; //uses a structure pointer that points to the beginning of the packet header being parsed
  unsigned short ethernet_type = ntohs(eth_header->ether_type); // Convert Ethernet type from network byte order to host byte order


  printf("\n\n === PACKET %ld HEADER ===", pcount);
  printf("\nSource MAC: ");
  for (i = 0; i < 6; ++i) {
    printf("%02x", eth_header->ether_shost[i]);
    if (i < 5) {
      printf(":");
    }
  }
  printf("\nDestination MAC: ");
  for (i = 0; i < 6; ++i) {
    printf("%02x", eth_header->ether_dhost[i]);
    if (i < 5) {
      printf(":");
    }
  }
  printf("\nType: %hu\n", eth_header->ether_type);
  printf(" === PACKET %ld DATA == \n", pcount);
  // Decode Packet Data (Skipping over the header)
  int data_bytes = length - ETH_HLEN;
  const unsigned char *payload = data + ETH_HLEN;
  const static int output_sz = 20; // Output this many bytes at a time
  while (data_bytes > 0) {
    int output_bytes = data_bytes < output_sz ? data_bytes : output_sz;
    // Print data in raw hexadecimal form
    for (i = 0; i < output_sz; ++i) {
      if (i < output_bytes) {
        printf("%02x ", payload[i]);
      } else {
        printf ("   "); // Maintain padding for partial lines
      }
    }
    printf ("| ");
    // Print data in ascii form
    for (i = 0; i < output_bytes; ++i) {
      char byte = payload[i];
      if (byte > 31 && byte < 127) {
        // Byte is in printable ascii range
        printf("%c", byte);
      } else {
        printf(".");
      }
    }
    printf("\n");
    payload += output_bytes;
    data_bytes -= output_bytes;
  }
  printf("\n === PACKET %ld IP HEADER ===\n", pcount);
  parse_ip_header(data);
  pcount++;
}


/**
 * breaks down the ip header into different fields. 
 * @param *data Pointer to the start of the packet data. 
 * 
*/ 
void parse_ip_header(const unsigned char *data) {
    struct ip *ip_header = (struct ip *)(data + ETH_HLEN); 
    // Moving the pointer to the startof the ip header. 

    
    printf("Version: %d\n", ip_header->ip_v);
    printf("Header Length: %d words (%d bytes)\n", ip_header->ip_hl, ip_header->ip_hl * 4);
    printf("Type of Service: %d\n", ip_header->ip_tos);
    printf("Total Length: %d bytes\n", ntohs(ip_header->ip_len));
    printf("Identification: %d\n", ntohs(ip_header->ip_id)); //ntohs is used 
    printf("Flags: 0x%04x\n", ntohs(ip_header->ip_off));
    printf("Time to Live: %d\n", ip_header->ip_ttl);
    printf("Protocol: %d\n", ip_header->ip_p); //think it should be tcp for every packet. 
    printf("Header Checksum: 0x%04x\n", ntohs(ip_header->ip_sum));
    printf("Source IP: %s\n", inet_ntoa(ip_header->ip_src));
    printf("Destination IP: %s\n", inet_ntoa(ip_header->ip_dst));

    // Call the function to parse the TCP header with the offset adjusted for the IP header length.
    //ip headre length =ip_header->ip_hl * 4 
    parse_tcp_header(data, ip_header->ip_hl * 4 );
}



/**
 * breaks down the decoded header into different fields. 
 * @param *data Pointer to the start of the TCP header.
 * @param lengthOfIP: Length of the preceding IP header, used to adjust the pointer to the TCP header.
 * 
*/
void parse_tcp_header(const unsigned char *data, int lengthOfIP) {
  struct tcphdr *tcp_header = (struct tcphdr *)(data + ETH_HLEN+ lengthOfIP); //moving the pointer to the start of tcp header
  //printing the required fields. 
  printf("Source Port: %hu\n", ntohs(tcp_header->th_sport));
  printf("Destination Port: %hu\n", ntohs(tcp_header->th_dport));

  // Data Offset is 4 bits (upper 4 bits in the byte)
  printf("Data Offset: %d bytes\n", (tcp_header->th_off >> 4) * 4);
  
  // Flags (8 bits)
  printf("URG: %d, ACK: %d, PSH: %d, RST: %d, SYN: %d, FIN: %d\n",
         (tcp_header->th_flags & 0x20) >> 5,
         (tcp_header->th_flags & 0x10) >> 4,
         (tcp_header->th_flags & 0x08) >> 3,
         (tcp_header->th_flags & 0x04) >> 2,
         (tcp_header->th_flags & 0x02) >> 1,
         tcp_header->th_flags & 0x01);
}






//in analysis
// issyn=0, isblacklist =0
//he didnt use array like we did 
// he was like *tmp_pointer = something(realloc * sizeOfIPaddress)
// he did something like if issyn, then a mutex lock 
// acquires the mutex lock then does something then releases the lock. 
//then if isBlacklist 

//he said no need for hashmaps, cant install glib. use of header files that we have done so far is okay. 

// in sniff.c we need pcap_loop() instead of pcap_next()
// after this lab, i think sniff is just for decoding the headers. analysis is for storing and stuff. 