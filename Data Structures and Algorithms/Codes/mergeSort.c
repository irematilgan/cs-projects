#include <stdio.h>
#include <stdlib.h>
void mergeSort(int arr[],int p, int r){
	
	int q;
	if(p < r)
	{
		q = (p+r)/2;
		printf("%d\n",q);
		mergeSort(arr,p,q);
		mergeSort(arr,q+1,r);
		
		int i;
		int aux[50];
		
		
		
		int left_pointer,right_pointer,k;
		left_pointer = p;
		right_pointer = q+1;
		k = 0;
		
		while(left_pointer < q+1 && right_pointer <= r){
			if(arr[left_pointer] < arr[right_pointer])
				aux[k++] = arr[left_pointer++];
			else
				aux[k++] = arr[right_pointer++];
		}

		while(left_pointer < q+1){
			aux[k] = arr[left_pointer];
			k++;
			left_pointer++;
		}
	
		
		while(right_pointer <= r){
			aux[k] = arr[right_pointer];
			k++;
			right_pointer++;
		}
		
		k = 0;
		for(i = p; i <= r; i++)
			arr[i] = aux[k++];
		
	}
	
}

int main(){
	int arr[] = {3,7,1,2,8,4,6};
	int n = 7;
	int aux[7];
	
	mergeSort(arr,0,n-1);
	int i;
	for(i = 0; i < n; i++){
		printf("%d\t",arr[i]);
	}
	
	return 0;
}
