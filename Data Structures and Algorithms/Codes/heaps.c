#include <stdio.h>

void swap(int *x, int *y){
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void siftUp(int Heap[],int val,int n){
	
	while(n > 0 && val > Heap[(n-1)/2])
	{
		
		swap(&Heap[n],&Heap[(n-1)/2]);
		n = (n-1)/2;
		
	}
}

int findBiggestChild(int Heap[],int parent,int n)
{
	if(parent*2+2 < n)
	{
		if(Heap[2*parent+1] > Heap[2*parent+2])
			return 2*parent+1;
		
		return 2*parent+2;
	}
	else if(parent*2+1 < n)
		return parent*2+1;
	else
		return -1;
}

void siftDown(int Heap[],int n)
{
	int i = 0;
	int child = findBiggestChild(Heap,i,n);
	while(child != -1 && Heap[child] > Heap[i])
	{	
		swap(&Heap[i],&Heap[child]);
		i = child;
		child = findSmallestChild(Heap,i,n);
		
	}
}

int deleteRoot(int Heap[], int *n)
{
	int tmp = Heap[0];
	Heap[0] = Heap[(*n)-1];
	(*n)--;
	siftDown(Heap,*n);
	return tmp;
}


void insert(int Heap[],int val,int *n)
{
	Heap[*n] = val;
	siftUp(Heap,val,*n);
	(*n)++;
}

int findSmallestChild(int Heap[],int parent,int n)
{
	if(parent*2+2 < n)
	{
		if(Heap[2*parent+1] < Heap[2*parent+2])
			return 2*parent+1;
		
		return 2*parent+2;
	}
	else if(parent*2+1 < n)
		return parent*2+1;
	else
		return -1;
}

void siftDown2(int Heap[],int n)
{
	int parent = 0;
	int child = findSmallestChild(Heap,0,n);
	while(child != -1 && Heap[parent] > Heap[child])
	{
		swap(&Heap[parent],&Heap[child]);
		parent = child;
		child = findSmallestChild(Heap,parent,n);
	}
}

void heapSort(int Heap[],int n)
{
	int i;
	for(i = n-1; i > 0; i--)
	{
		swap(&Heap[i],&Heap[0]);
		siftDown(Heap,i);
	}
}


int main()
{
	int n = 5;
	int n2 = 6;
	int list[20] = {20,16,12,5,8};
	int list2[20] = {28,20,12,16,8,5};
	int list3[20] = {15,9,8,2,7};
	//insert(list,35,&n);
	//deleteRoot(list2,&n2);
	heapSort(list3,n);
	int i;
	for(i = 0; i < n; i++){
		printf("%d\t",list3[i]);
	}
	
	
	return 0;
}
