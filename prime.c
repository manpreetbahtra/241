//Takes an integer as input and checks if it is a prime.

#include<stdio.h>

int main(){

    int p,i;

    printf("Enter a positive integer:");
    scanf("%d", &p);

    for(i=2; i<p; i++){ //check if any integer less than p other than 1 divides p
        if(!(p%i))
        {
            break;
        }
    }
    if(i<p)
    {
        printf("Entered integer is not a prime\n");

    }
    if(i==p)
    {
        printf("Entered number is a Prime Number.\n");
    }

return 0;
}