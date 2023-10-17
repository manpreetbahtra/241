/* Exercise 2: Write a C program that copies the content of one text file into another. 
You should write two versions of the program; one using file streams only and another 
using file descriptors only.*/

//using file descriptors
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>  // for the open() function
#include<unistd.h> // for read(), write() and close() functions

#define BUFSIZE 10

int main(){

    int fd1=open("./textfile1.txt",O_RDONLY);  // opens the file in read only mode and returns a file descriptor for the file
    int fd2=open("./textfile2.txt",O_WRONLY); //write only mode 
    int x;

    if(fd1 < 0){
        write(2,"Error opening file\n",sizeof("Error opening file\n")); // writes the error message to stderr
        exit(1);
    }

    if(fd2 < 0){
        write(2,"Error opening file\n",sizeof("Error opening file\n")); // writes the error message to stderr
        exit(1);
    }

    char str[BUFSIZE];

    printf("Content of the opened file:\n");
    while((x=read(fd1,str,BUFSIZE-1)) > 0){ // read() reads up to BUFSIZE-1 bytes from fd1 and stores in str
                                           // the number of bytes actually read is returned
            str[x]='\0';
            write(1,str,strlen(str));   // '1' is the file dscriptor for stdout, hence this is equivalent to printf("%s", str)
            write(fd2, str, strlen(str)); // fd2- file descriptor where data is to be written, str- buffer to write data to, strlen(str) is the length of the buffer
    }
    printf("\n");
    close(fd2);
    close(fd1);
    return 0;

}