//Bubble sort in C 
#include <stdio.h>

// write the code for the sort() function that sorts an integer array in ascending order

void swap(int *val_a, int *val_b){
    int temp;
    temp = *val_a;
    *val_a=*val_b;
    *val_b=temp;

}

void sort(int *array, int arraySize){
    int i;
    int j;
    for(i=0; i<arraySize; i++){
        for(j=i+1; j<arraySize; j++){
            if (array[i] > array[j]){
                swap(&array[i], &array[j]);
            }
        }
    }
}




int main() {
    int x[]={4,1,4,3,10,5};
    int i;

    sort(x,6); // sort() function sorts the array x in ascending order

    printf("The sorted array is as follows:\n");

    for(i=0; i<6; i++){
        printf("%d ",x[i]);
    }
    printf("\n");
    return 0;
}