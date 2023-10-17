/*Exercise 4: Using structures, implement the queue data structure with functions to 
check if the queue is empty (isempty), to insert a node to the tail end of the queue
 (enqueue), and to remove a node from the head of the queue (dequeue).*/


#include <stdio.h>
#include <stdlib.h>

struct QNode {
    int key;
    struct QNode* next;
};

struct Queue {
    struct QNode *front, *rear;
};

struct QNode* newNode(int k)
{
    struct QNode* temp
        = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

struct Queue* createQueue()
{
    struct Queue* q
        = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}


int isEmpty(struct Queue* q){
    if (q->front == NULL){
        return 1;
    }
    return 0;
}

void enqueue(struct Queue* q, int k){
    struct QNode* temp = newNode(k);
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
     // Add the new node at the end of queue and change rear pointer 
    q->rear->next = temp;
    q->rear = temp;
}


void dequeue(struct Queue* q){
    //dequeuing from an empty should return null
    if(q->front == NULL){
        return;
    }

    //if queue contains something, it should remove front,- make front point to current head+1
    struct QNode* temp = q->front; //i am storing current head in temp to free that memory
    q->front = q->front->next;

    if (q->front == NULL){
        q->rear = NULL;
    }
    free(temp);

}

int main()
{
    struct Queue* q = createQueue();
    enqueue(q, 10);
    enqueue(q, 20);
    dequeue(q);
    dequeue(q);
    enqueue(q, 30);
    enqueue(q, 40);
    enqueue(q, 50);
    dequeue(q);
    isEmpty(q);
    printf("Queue Front : %d \n", ((q->front != NULL) ? (q->front)->key : -1));
    printf("Queue Rear : %d \n", ((q->rear != NULL) ? (q->rear)->key : -1));
    return 0;
}