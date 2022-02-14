
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue{
	int front;
	int rear;
	int* items;
	int size;
}Queue;

Queue createQueue(int item_size){
	Queue* q = (Query*)malloc(sizeof(Queue));
	q->front = -1;
	q->rear = -1;
	q->items = (int*)malloc(sizeof(int)*item_size);
	q->size = item_size;
	
	return q
}

int enqueue(Queue* q,int val) {
	if(isFull(q)) return 0;
	else if(isEmpty(q))
		q->front = 0;

	q->rear++;
	q->items[q->rear] = val;
	return 1;
}

int dequeue(Queue* q) {
	if(isEmpty(q)) return -1;
	return q->items[q->front++] ;
}
