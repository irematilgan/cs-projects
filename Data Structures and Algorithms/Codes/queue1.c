#include <stdio.h>
#include <stdlib.h>

typedef struct QNode{
	int value;
	struct QNode *next;
}QNode;


typedef struct QUEUE{
	QNode *front;
	QNode *rear;
}QUEUE;



int isEmpty(QUEUE *front)
{
	if(front == NULL)
		return 0;
	else
		return 1;
		
}

QUEUE* createQueue()
{
	QUEUE* q;
	q = (QUEUE*)malloc(sizeof(QUEUE));
	if(q == NULL)
		exit(0);
	else
	{
		q->front = q->rear = NULL;
		return q;
	}
}

void enqueue(QUEUE* q, int value)
{
	QNode *temp;
	temp = (QNode*)malloc(sizeof(QNode));
	if(temp == NULL)
		exit(0);
	
	temp->value = value;
	temp->next = NULL;
	if(q->front == NULL)
	{
		q->front = temp;
		q->rear = temp;
	}
	else{
		q->rear->next = temp;
		q->rear = temp;
	}
}


int main()
{
	QUEUE* q = createQueue();
	enqueue(q,3);
	enqueue(q,5);
	enqueue(q,7);
	return 0;
}
