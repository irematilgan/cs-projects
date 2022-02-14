#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* set_array(int*, int);			//Set array with random numbers
void quick_sort(int*, int, int);	//Quick sort algorithm
void swap(int*,int*);				//Swap two numbers
int* alt_way(int*, int);			//Alternative way for finding two elements that gives minimum difference in an array
int* linear_search(int*, int);		//Search whole array to find minimum difference between sequential numbers

int main()
{
	int length; 	//array length
	int* array;
	int* indices; 	//indices found for minimum difference
	
	printf("\nPlease enter the length of the array : ");scanf("%d",&length);
	
	array = (int*)malloc(sizeof(int)*length);
	
	if(length > 1) 
	{
		array = set_array(array,length);
		indices = alt_way(array,length);
		printf("MIN DIF = %d FOUND BETWEEN NUMBERS [%d] & [%d]",abs(array[indices[0]]-array[indices[1]]),array[indices[0]],array[indices[1]]);
		//free pointers from memory
		free(indices);
		free(array);
		
	}
	else
		printf("The array size should be more than 1");
		
	return 0;
}

int* set_array(int* arr, int length) {
	int i;
	srand(time(0)); //Use current time as seed for random generator
	for(i = 0; i < length ; i++) 
		arr[i] = rand();
	
	return arr;
}

//Alternative way for finding two elements that give minimum difference in an array
int* alt_way(int* arr, int length) 
{
	clock_t begin = clock();	//the time algorithm had started working
	quick_sort(arr,0,length-1);	//sort the array (in ascending order)
	printf("SORTED!..\n");		
	int* indices = linear_search(arr,length); 	//find indices that give minimum difference by calculating difference of two sequential elements
	printf("COMPLETED!..\n");
	clock_t end = clock();						//the time brute force algorithm has finished 
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC; //calculate the time passed since the algorithm had started
	printf("ALGORITHM PROGRESSING TIME = %lf\n",time_spent);
	
	return indices;
}

//Sort array with Quick Sort
void quick_sort(int* arr, int left, int right) {
	if(left < right) 
	{
		int i = left;
		int j = right;
		int pivot = left;
		while(i < j) 
		{
			while(arr[pivot] >= arr[i] && i < right)	i++;
			while(arr[pivot] < arr[j])	j--;
			if(i < j) {
				swap(&arr[i],&arr[j]); //swap the numbers which one of them is larger and one of them is smaller than pivot
			}
			
		}
		swap(&arr[pivot],&arr[j]); 	//set pivot to its new place
		
		//divide array into two parts and keep sorting
		quick_sort(arr,left,j-1);	
		quick_sort(arr,j+1,right);	
	}
}

//Swap array elements
void swap(int* x, int* y){
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

//Search whole array to find the indices that give minimum difference between two sequential elements
int* linear_search(int* arr, int length) {
	int i;
	int* indices = (int*)malloc(sizeof(int)*2); //indices found for minimum difference
	
	int dif;									//difference between two numbers
	int min_dif;								//minimum difference found
	
	//set the minimum difference with the difference of first two elements
	indices[0] = 0;
	indices[1] = 1;
	min_dif = abs(arr[0]-arr[1]); 
	
	for(i = 2; i < length; i++) 
	{
		dif = abs(arr[i]-arr[i-1]); //calculate the difference between two sequential numbers
		if(dif < min_dif) 
		{
			min_dif = dif;
			indices[0] = i;
			indices[1] = i-1;
		}
	} 
	
	return indices;
}



