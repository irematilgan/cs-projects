#include <stdio.h>

void swap(int* x, int* y){
	int tmp = *x;
	*x = *y;
	*y = tmp;
}


void quickSort(int A[],int l,int r){
	if(l<r){
		int pivot = l;
		int i = l, j = r;
		while(i < j){
			while(A[i] <= A[pivot] && i < r)
				i++;
			
			while(A[j] > A[pivot] && j > l)
				j--;
			
			if(i < j)
				swap(&A[i],&A[j]);	
			
		}
		
		
		swap(&A[pivot],&A[j]);
		
		for(i = 0; i < 8; i++){
			printf("%d\t",A[i]);
		}
		printf("\n");
		quickSort(A,l,j-1);
		quickSort(A,j+1,r);
	}
}




int main()
{
	int A[] = {8,3,2,4,7,1,5,9};
	int N = 8;
	quickSort(A,0,N-1);
	int i;
	for(i = 0; i < N; i++){
		printf("%d\t",A[i]);
	}
	return 0;
}
