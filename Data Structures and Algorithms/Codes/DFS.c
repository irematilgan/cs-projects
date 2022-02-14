#include <stdio.h>
#include <stdlib.h>
#define SIZE 40

typedef struct Node{
	int val;
	struct Node* next;
}NODE;

typedef struct queue{
	int items[SIZE];
	int front,rear;
}QUEUE;

typedef struct Graph{
	int* visited;
	NODE** adjList;
	int numOfVertices;
	
}Graph;

QUEUE* createQueue()
{
	QUEUE* q = malloc(sizeof(QUEUE));
	q->front = -1;
	q->rear = -1;
	return q;
}

int isEmpty(QUEUE* q)
{
	if(q->front == -1)
		return 1;
	
	return 0;
}

int isFull(QUEUE* q)
{
	printf("aa");
	if(q->rear == SIZE-1)
		return 1;
		
	return 0;
}

void enqueue(QUEUE* q,int val)
{
	if(isFull(q)){
		printf("Queue is full!..");
		return;
	}
	if(isEmpty(q))
		q->front = 0;
	q->rear++;
	q->items[q->rear] = val;
	
}

int dequeue(QUEUE* q)
{
	if(isEmpty(q))
		return -1;
	return q->items[q->front++];
}

NODE* createNode(int v)
{
	NODE* node = malloc(sizeof(NODE));
	node->val = v;
	node->next = NULL;
}

Graph* initGraph(int numVertices)
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numOfVertices = numVertices;
	graph->visited = malloc(sizeof(int)*numVertices);
	graph->adjList = malloc(sizeof(NODE*)*numVertices);
	
	int i;
	for(i = 0; i < numVertices; i++)
	{
		graph->adjList[i] = NULL;
		graph->visited[i] = 0;
	}
	
	return graph;
} 

void addEdge(Graph* graph,int src,int dst)
{
	NODE* node = createNode(dst);
	
	node->next = graph->adjList[src];
	graph->adjList[src] = node;
	
	node = createNode(src);
	node->next = graph->adjList[dst];
	graph->adjList[dst] = node;
}

void printGraph(Graph* graph)
{
	int i;
	NODE* temp;
	for(i = 0; i < graph->numOfVertices; i++)
	{
		temp = graph->adjList[i];
		while(temp != NULL)
		{
			printf("%d\t",temp->val);
			temp = temp->next;
		}
		printf("\n");
	}
}

void BFS(Graph* graph, int startVertex)
{
	QUEUE* q = createQueue();
	int v;
	enqueue(q,startVertex);
	graph->visited[startVertex] = 1;
	while(!isEmpty(q))
	{
		v = dequeue(q);
		NODE* tmp = graph->adjList[v];
		while(tmp != NULL)
		{
			if(!graph->visited[tmp->val]){
			
				enqueue(q,tmp->val);
				graph->visited[tmp->val];
			}
			tmp = tmp->next;
		}
		
	}
}

void DFS(Graph* graph, int vertex)
{
	NODE* node = graph->adjList[vertex];
	NODE* tmp = node;
	graph->visited[vertex] = 1;
	printf("\nVisited -> %d",vertex);
	while(tmp != NULL)
	{
		int connectedVertex = tmp->val;
		if(graph->visited[connectedVertex] == 0)
			DFS(graph,connectedVertex);
		tmp= tmp->next;
	}
	
}

void printQueue(QUEUE* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d ", q->items[i]);
    }
  }
}


int main()
{
	Graph* graph = initGraph(4);
	/*
	addEdge(graph,0,1);
	addEdge(graph,0,5);
	addEdge(graph,2,0);
	addEdge(graph,2,3);
	addEdge(graph,3,2);
	addEdge(graph,3,5);
	addEdge(graph,4,2);
	addEdge(graph,4,3);
	addEdge(graph,5,4);
	addEdge(graph,6,4);
	addEdge(graph,6,0);
	addEdge(graph,6,9);
	addEdge(graph,7,6);
	addEdge(graph,7,8);
	addEdge(graph,8,7);
	addEdge(graph,8,9);
	addEdge(graph,9,10);
	addEdge(graph,9,11);
	addEdge(graph,10,12);
	addEdge(graph,11,4);
	addEdge(graph,11,12);
	addEdge(graph,12,9);
	*/
	addEdge(graph, 0, 1);
  	addEdge(graph, 0, 2);
  	addEdge(graph, 1, 2);
  	addEdge(graph, 2, 3);
	printGraph(graph);
	DFS(graph,0);
	//BFS(graph,6);
	
	return 0;
}
