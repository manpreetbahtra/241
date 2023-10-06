#include<stdio.h>
#include<math.h>


int primes(int n){  // computes the primes between 1 and n

    int i;
    int j;

    if (n>2){
        for (j=2; j<n; j++){
            for (i=0; i<=sqrt(j); i++) {
                if(j%i == 0){
                    continue;
                } else {
                    printf("%d\n",j);
                }
            }
        }
    }else if (n==2){
        printf("%d\n", n);
    } else{
        printf("%s\n", "no primes");
    }
    

    
    return 0;
}

int main(){

    int n;

    printf("Enter a positive integer:");
    scanf("%d",&n); // scanf() scans the integer entered by the user
    primes(n);

    //printf("The primes of %d is %d\n",n,primes(n));
    return 0;
}