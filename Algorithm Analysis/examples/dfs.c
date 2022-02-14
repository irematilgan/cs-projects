#include <stdio.h>
#include <stdlib.h>

typedef struct Q{
	int front;
	int rear;
	int capacity;
	int* list;
}Q;

int isFull(Q* queue)
{
	if(queue->front == queue->capacity || queue->front == queue->rear)
		return 0;
	return 1;
}

int isEmpty(Q* queue)
{
	if(queue->front == 0 || queue->front == queue->rear)
		return 0;
	return 1;
}

int enqueue(Q* queue, int element)
{
	if(isFull(queue))
	{
		printf("Kuyruga ekleme yapilamiyor!..\n");
		return -1;
	}
	printf("Ekleniyor!..%d\n",element);
	queue->list[(queue->rear)++] = element;
	return 1;
}

int dequeue(Q* queue)
{
	if(!isEmpty(queue))
	{
		return queue->list[(queue->front)++];
	}
	printf("Kuyruktan Cikarma Yapilamiyor!..\n");
	return -1;
}

Q* createQueue(int cap)
{
	Q* queue = (Q*)malloc(sizeof(Q));
	queue->front = 0;
	queue->rear = 0;
	queue->capacity = cap;
	queue->list = (int*)malloc(sizeof(int)*cap);
}

int main()
{
	Q* queue = createQueue(5);

	enqueue(queue,2);
	enqueue(queue,4);
	enqueue(queue,5);
	enqueue(queue,6);
	
	return 0;
}
