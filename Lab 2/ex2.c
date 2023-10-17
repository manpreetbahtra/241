/* Exercise 2: Write a C program that copies the content of one text file into another. 
You should write two versions of the program; one using file streams only and another 
using file descriptors only.*/

// using file streams
#include<stdio.h>
#include<stdlib.h> // required for the exit() function

#define BUFSIZE 10

int main(){

    FILE *fptr1=fopen("./textfile1.txt","r");
    FILE *fptr2= fopen("./textfile2.txt", "w");

    if(fptr1==NULL){
        fprintf(stderr,"Error opening file\n");
        exit(1);
    }

     if(fptr2==NULL){
        fprintf(stderr,"Error opening file\n");
        exit(1);
    }

    char str[BUFSIZE];

    printf("Content of the opened file:\n");
    while(fgets(str,BUFSIZE,fptr1)!=NULL){ // read the file line by line
        fprintf(stdout,"%s",str);         // same as doing printf("%s",str);
        fputs(str, fptr2);
    }
    printf("\n");
    fclose(fptr1);
    fclose(fptr2);
    return 0;

}


