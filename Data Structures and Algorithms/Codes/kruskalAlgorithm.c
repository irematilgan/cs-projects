#include <stdio.h>
#include <stdlib.h>

struct node{
	int vertex;
	struct node* next;	
};

struct Graph{
	int numVertices;
	struct node** adjLists;
};

typedef struct Edge{
	int u,v;
	int weight;
}Edge;

struct node* createNode(int v){
	
	struct node* newNode = malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	
	return newNode;
}

struct Graph* createGraph(int vertices){
	
	int i;
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->numVertices = vertices;
	graph->adjLists = malloc(sizeof(struct node)*vertices);
	
	for(i = 0; i < vertices;i++){
		graph->adjLists[i] = NULL;
	}
	
	return graph;
}


void addEdge(struct Graph* graph, int src, int dst){
	
	struct node* newNode = createNode(src);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;
	newNode = createNode(dst);
	newNode->next = graph->adjLists[dst];
	graph->adjLists[dst] = newNode;
	
	
}

void unite(int label[],int nv, int uu, int vv){
	int i;
	for(i = 0; i < nv; i++){
		if(label[i] == label[uu])
			label[i] = label[vv];
	}
}

void kruskal(Edge gr[],Edge mst[],int nv){
	int labelNo = 1, i,j;
	int label[nv];
	for(i = 0; i < nv; i++)
		label[i] = 0;
	
	i = j = 0;
	int uu,vv;
	while(i < nv-1){
		uu = gr[j].u;
		vv = gr[j].v;
		if(label[uu] + label[vv] == 0)
		{
			mst[i].u = uu;
			mst[i].v = vv;
			mst[i].weight = gr[j].weight;
			label[uu] = label[vv] = labelNo++;
			i++;
		}
		if(label[uu] != label[vv]){
			mst[i].u = uu;
			mst[i].v = vv;
			mst[i].weight = gr[j].weight;
			i++;
			if(!label[uu])
				label[uu] = label[vv];
			else if(!label[vv])
				label[vv] = label[uu];
			else
				unite(label,nv,uu,vv);
		}
		j++;
	}
	
}



int main(){
	
	int n = 8;
	struct Graph* graph = createGraph(n);
	addEdge(graph,3,5);
	addEdge(graph,1,7);
	addEdge(graph,6,7);
	addEdge(graph,0,2);
	addEdge(graph,0,7);
	addEdge(graph,0,1);
	addEdge(graph,3,4);
	addEdge(graph,4,5);
	addEdge(graph,7,4);
	addEdge(graph,0,6);
	addEdge(graph,6,4);
	addEdge(graph,0,5);
	
	

	
	
	
	addEdge(graph,4,5);
	printf("Eklendi");
	
	int us[] = {3,1,6,0,0,0,3,4,7,0,6,0};
	int vs[] = {5,7,7,2,7,1,4,5,4,6,4,5};
	
	int i;
	Edge edge[12];
	for(i = 0; i < 12; i++){
		edge[i].u = us[i];
		edge[i].v = vs[i];
	}
	
	Edge minimumSpanningTree[n-1];
	printf("\nKruskal\n");
	kruskal(edge,minimumSpanningTree,n);
	
	for(i = 0; i < n-1; i++){
		printf("%d - %d\t",minimumSpanningTree[i].u,minimumSpanningTree[i].v);
	}
	
	return 0;
}
