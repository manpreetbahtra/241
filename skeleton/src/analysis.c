#include "analysis.h"

#include <pcap.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>
#include <string.h>


#define INITIAL_CAPACITY 100



struct ParsedPacket {

  uint8_t sourceMacAddress[6];
  uint8_t destinationMacAddress[6];
  struct in_addr src_ip;//need
  struct in_addr dest_ip;
  uint16_t src_port;
  uint16_t dest_port;
  int urg_flag;
  int ack_flag;
  int psh_flag;
  int rst_flag;
  int syn_flag;
  int fin_flag;
  
};


struct SourceIPArray {
  struct in_addr *addresses;
  size_t size;
  size_t capacity;
};

struct SourceIPArray uniqueSourceIPs;
int uniqueIPArraySize=0;
int synPacketsCount=0;



void initializeSourceIPArray(struct SourceIPArray *ipArray) {
    ipArray->addresses = malloc(INITIAL_CAPACITY * sizeof(struct in_addr));
    if (ipArray->addresses == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    ipArray->size = 0;
    ipArray->capacity = INITIAL_CAPACITY;
}

void printSourceIPArray(const struct SourceIPArray *ipArray) {
    printf("Unique Source IP Addresses:\n");

    for (size_t i = 0; i < ipArray->size; ++i) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ipArray->addresses[i]), ipStr, INET_ADDRSTRLEN);
        printf("%s\n", ipStr);
    }

    printf("\n");
}

void storeUniqueIPAddress(struct SourceIPArray *ipArray, const struct in_addr *ip) {
    // Ensure that sourceIPs is initialized
    
    if (ipArray->addresses == NULL) {
      printf("I am here \n");
      initializeSourceIPArray(ipArray);
    }
    // Check if the IP address is already stored
    for (size_t i = 0; i < ipArray->size; ++i) {
        if (memcmp(ip, &ipArray->addresses[i], sizeof(struct in_addr)) == 0) {
          //memcmp- memory compare. returns <0, >0, 0. returns 0 if memory blocks being compared are equal. 
          // ip= the pointer to ip address being examined
          // &ipArray.addresses[i] - the pointer to each element already in array.
            // IP address already stored, do not store again
          printf("dont store it\n");
          return;
        }
    }

    // Check if the array is full, double its capacity if necessary. 
    if (ipArray->size == ipArray->capacity) {
        ipArray->capacity *= 2;
        ipArray->addresses = realloc(ipArray->addresses, ipArray->capacity * sizeof(struct in_addr));
        if (ipArray->addresses == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    //packet not already stored. store the new ip address
    // Store the unique IP address
    //for a pointer, have to use -> notation to access its fields.
    ipArray->addresses[ipArray->size++] = *ip;
    uniqueIPArraySize= ipArray->size;
    

}


struct ParsedPacket parseAPacket(struct pcap_pkthdr *header,const unsigned char *packet){
  struct ether_header *eth_header = (struct ether_header *)packet;
  struct ip *ip_header = (struct ip *)(packet + ETH_HLEN);
  struct tcphdr *tcp_header = (struct tcphdr *)(packet + ETH_HLEN+ ip_header->ip_hl *4 );

  // Create a ParsedPacket struct and store information
  struct ParsedPacket parsedPacket;
  
  // Store MAC addresses- 
  // memcpy is a C standard library function used for memory copying. 
  // Its purpose is to copy a block of memory from a source address to a destination address.
  memcpy(parsedPacket.sourceMacAddress, eth_header->ether_shost, 6); // accessing ether_shost field from eth_header structure. 
  memcpy(parsedPacket.destinationMacAddress, eth_header->ether_dhost, 6);

  // Store IP addresses
  parsedPacket.src_ip = ip_header->ip_src;
  parsedPacket.dest_ip = ip_header->ip_dst;

  // Store port numbers
  parsedPacket.src_port = ntohs(tcp_header->th_sport);
  parsedPacket.dest_port = ntohs(tcp_header->th_dport);

  // Store TCP flags
  parsedPacket.urg_flag = (tcp_header->th_flags & 0x20) >> 5;
  parsedPacket.ack_flag = (tcp_header->th_flags & 0x10) >> 4;
  parsedPacket.psh_flag = (tcp_header->th_flags & 0x08) >> 3;
  parsedPacket.rst_flag = (tcp_header->th_flags & 0x04) >> 2;
  parsedPacket.syn_flag = (tcp_header->th_flags & 0x02) >> 1;
  parsedPacket.fin_flag = (tcp_header->th_flags & 0x01);


  return parsedPacket;
}


void freeSourceIPArray(struct SourceIPArray *ipArray) {
    free(ipArray->addresses);
    ipArray->addresses = NULL;
    ipArray->size = 0;
    ipArray->capacity = 0;
}

void analyse(struct pcap_pkthdr *header,
             const unsigned char *packet,
             int verbose) {
  //now that i have parsed each packet, i want to analyse it to see if it fits the criteria, then store the relevat ones. 
  //storing each ip unless it has already been stored. 
  //analyse if SYN is 1 and every other flag is 0
  //if is SYN, increment count for number syn packets and check if this IP address has already sent a syn packet, if not add to unique array 
  struct ParsedPacket parsedPacket = parseAPacket(header, packet);
  //packet is passed not *packet because packet is already a pointer
  //i want to check all flags 
  if (parsedPacket.syn_flag && !parsedPacket.urg_flag && !parsedPacket.ack_flag && !parsedPacket.psh_flag && !parsedPacket.rst_flag && !parsedPacket.fin_flag ){
    //i found a syn packet 
    printf("syn packet\n");
    synPacketsCount++;
    //i should check if unique IP address- check is done in storeUniqueIPAddress and storage also done there.
    storeUniqueIPAddress(&uniqueSourceIPs, &parsedPacket.src_ip);
    printSourceIPArray(&uniqueSourceIPs);
  }

}




//in analysis
// issyn=0, isblacklist =0
//he didnt use array like we did 
// he was like *tmp_pointer = something(realloc * sizeOfIPaddress)
// he did something like if issyn, then a mutex lock 
// acquires the mutex lock then does something then releases the lock. 
//then if isBlacklist 

//he said no need for hashmaps, cant install glib. use of header files that we have done so far is okay. 


//now that we have initialised the arry, want to store each packet as it comes. 
//we have noe stored the unique ips. need to count them at the end. 