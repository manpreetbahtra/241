/*Exercise 4: Write a C program that would sort an array of any size entered by a user. The program should first ask 
the user the size of the array they want to sort. Then the program should ask the user to enter the elements of the array.
Once the whole array is entered by the user, the program should output the sorted array. Use a dynamic array to store the 
integers entered by the user. You can use the scanf() function to read the numbers inputted by the user.*/

#include <stdio.h>
#include<stdlib.h>

// write the code for the sort() function that sorts an integer array in ascending order

void sort(int *arr, int size){ //bubble sort
    int i,j,tmp;
    for(i=0;i<size-1;i++){
        for(j=0;j<size-i-1;j++){
            if(arr[j] > arr[j+1]){
                tmp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=tmp;
            }
        }
    }
}




int main() {
    int n, i;
    int* x;  // pointer to store the address of the dynamically allocated memory block
    printf("Enter the size of array you want to input: ");
    scanf("%d", &n);

    x=(int*) malloc(n*sizeof(int));

    if(x==NULL){
        printf("Error: memory could not be allocated");
        exit(1);
    } else{
        printf("\nMemory successfully allocated\n");
        printf("Enter %d elements of array: ", n);
        for(i=0; i<n; i++){
            scanf("%d", &x[i]);
        }
        sort(x,n); // sort() function sorts the array x in ascending order
    }

    

    // sort(x,n); // sort() function sorts the array x in ascending order

    printf("The sorted array is as follows:\n");

    for(i=0; i<n; i++){
        printf("%d ",x[i]);
    }
    printf("\n");
    free(x);
    return 0;
}