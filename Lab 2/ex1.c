/*Exercise 1: Complete the following program by writing the code for the function 
 count_words() that takes a string as input and returns the number of words in the string */

#include<stdio.h>
#include<string.h>

int count_words(char *str){
    
    // this function should return the number of words in str
    /* To count the no. of words, I can split the string on whitespaces
     */
    int count =0; //no. of words
    int j=0;
    for(j=0; str[j] != '\0'; j++){
        //'\0' is the null character
        if(isspace(str[j]) && !isspace(str[j+1])){
            count++;
        }
    }
    return count;
}

int main(){
    
    
    char str[100];
    
    printf("Enter a string:");
    fgets(str,100,stdin);    // this function reads a line or at most 99 bytes from stdin file stream that represents the keyboard
    printf("Number of words in the entered string is %d\n",count_words(str));
    
    return 0;
}