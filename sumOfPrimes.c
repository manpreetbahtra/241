/*computes sum of primes between 1 and n
part of exercise 5- Now, create the object files for each of these source files by typing

gcc -c sum.c sum_squares.c
We shall now link all the three object files that we have created so far to build our final executable file. This can be done using the following command

gcc main.o sum.o sum_squares.o -o numbers
This will create the a combined excutable file called "numbers" by linking the three object files. Try running the executable file and see what it does.

This whole process of building the executable file from its sources can be automated using makefiles. 
A makefile contains recipes for building the final executable file as well as for each of the intermediate files. 
Each recipe is written in the following format: */

#include<stdio.h>
#include<math.h>


int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int sumOfPrimes(int n) {
    int sum = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            sum += i;
        }
    }
    return sum;
}





