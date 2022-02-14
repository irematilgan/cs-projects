#include <stdio.h>
#include <stdlib.h>

void printArray(int* arr, int N)
{
	int i;
	for(i = 0; i < N; i++)
	{
		printf("%d\t",arr[i]);
	}
	
	return;
}

int* insertionSort(int* arr, int N)
{
	int i,tmp,j;
	for(i = 1; i < N; i++)
	{
		tmp = arr[i];
		j = i;
		while(j > 0 && arr[j-1] > tmp)
		{
			arr[j] = arr[j-1];
			j--;
		}
		arr[j] = tmp;
		printArray(arr,N);
		printf("\n");
	}
	return arr;
}

void mergeElements(int* arr, int ind1, int mid, int ind2)
{

	int* merged = (int*)malloc(sizeof(int)*9);
	int i = ind1,j = mid+1;
	int k = ind1;
	while(i <= mid && j <= ind2)
	{
		if(arr[i] > arr[j])
		{
			merged[k] = arr[j];
			j++;
		}
		else
		{
			merged[k] = arr[i];
			i++;
		}
		k++;
	}
	if(i > mid)
	{
		while(j <= ind2)
			merged[k++] = arr[j++];
	}
	else if(j > ind2)
	{
		while(i <= mid)
			merged[k++] = arr[i++];
	}
	//printf("MergeElements\n");

	for(i = ind1; i < k; i++)
		arr[i] = merged[i];
	
	printArray(arr,9);
	printf("\n");
}

void mergeSort(int* arr, int l, int r){
	//printf("L = %d\tR = %d\n",l,r);
	if(l >= r)	return;
	else
	{
		int mid = (l+r)/2;
		mergeSort(arr,l,mid);
		mergeSort(arr,mid+1,r);
		mergeElements(arr,l,mid,r);	
	} 
	
}

int quickSort(int* arr,int N){
	return 0;
}


int main()
{
	int A[] = {4,2,1,5,6,3,8,4,7};	
	int N = 9;
	//insertionSort(A,N);
	mergeSort(A,0,N-1);
	printArray(A,N);
	return 0;
}
