/* Exercise 6 lab 1 Exercise 6: Fix the above program and ensure that it runs to completion (it should not produce any errors).

fixed- changed i<100000000 to i<5 since malloc only allocates memory to store 5 ints not 100000. this will result in a seg fault, since 
my_array is trying to access locations which it doesnt have access to(locations beyond 5). */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   int *my_array = (int *) malloc(5 * sizeof(int)); 
   int i;
   for (i = 0; i < 5; i++) { 
      my_array[i] = i;
   }
   free(my_array);
}