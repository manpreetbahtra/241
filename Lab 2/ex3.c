/* Structs are not objects as they can't contain functions, have no mechanism for inheritance 
or polymorphism and do not support encapsulation.
Structs are used to implement complex data structures in C.

Ex3- Extend the above example to allow elements to be inserted and removed at the head of the list.
*/

#include <stdio.h>
#include <stdlib.h>
struct element {
  struct element * next;
  int data;
};
struct linked_list {
  struct element * head;
};

void append_int(struct linked_list * list, int val) {
  struct element * elem = malloc(sizeof(struct element));
  elem->data = val;
  elem->next = NULL; // Really important to explicitly set this to null. Malloc does not zero memory
  if (list->head == NULL) {
    // Empty list, we need to append to head
    list->head = elem;
  } else {
    // List has some elements, find the end and append to that
    struct element * tail = list->head;
    while (tail->next != NULL) {
      tail = tail->next;
    }
    tail->next = elem;
  }


}

void addToHead(struct linked_list * list, int val){
    struct element *newNode = malloc(sizeof(struct element));
      newNode->data = val;

      newNode->next = list->head;

      list->head = newNode;
}

void removeFromHead(struct linked_list * list){

    if (list->head == NULL) {
        return;
    }

    //temp is used to freeing the memory
    struct element *temp;
    temp = list->head;    //backup the head to free its memory
    list->head = list->head->next;
    free(temp);
    
}

int main() {
    struct linked_list list;
    list.head = NULL;

    append_int(&list, 10); // Append 10 to the end
    append_int(&list, 20); // Append 20 to the end
    removeFromHead(&list);
    append_int(&list, 30); // Append 30 to the end

    addToHead(&list, 5);   // Add 5 to the head

    // Print the linked list
    struct element* current = list.head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");

    return 0;
}