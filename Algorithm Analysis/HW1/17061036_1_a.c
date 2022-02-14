#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* brute_force(int*, int);	//Brute force solution
int* set_array(int*, int);		//Set array with random numbers

int main()
{
	int length;		//array length
	int* array;	
	int* indices;	//indices found for minimum difference
	
	printf("\nPlease enter the length of the array : ");scanf("%d",&length);
	
	array = (int*)malloc(sizeof(int)*length);
	
	if(length > 1) 
	{
		array = set_array(array,length);
		indices = brute_force(array,length); //find the indices that give minimum difference
		printf("MIN DIF = %d FOUND BETWEEN INDICES %d [%d] & %d [%d]",abs(array[indices[0]]-array[indices[1]]),indices[0],array[indices[0]],indices[1],array[indices[1]]);
		
		//free pointers from memory
		free(indices);
		free(array);		
	}
	else
		printf("The array size should be more than 1");
		
	return 0;
}

int* set_array(int* arr, int length) 
{
	int i;
	srand(time(0)); //Use current time as seed for random generator
	
	for(i = 0; i < length ; i++) 
		arr[i] = rand();
	
	return arr;
}


int* brute_force(int* arr, int length) 
{
	clock_t begin = clock();	//the time algorithm had started working
	int i,j;
	int dif;					//difference between two numbers
	int min_dif;				//minimum difference found
	int* indices = (int*)malloc(sizeof(int)*2); //indices found for minimum difference
	
	//set the minimum difference with the difference of first two elements
	indices[0] = 0;
	indices[1] = 1;
	min_dif = abs(arr[0] - arr[1]);
	for(i = 0; i < length-1; i++) 
	{
		for(j = 1; j < length; j++) 
		{
			if(i != j) //cannot be the difference of same number
			{
				dif = abs(arr[i] - arr[j]); 
				if(min_dif > dif)
				{
					min_dif = dif;
					indices[0] = i;
					indices[1] = j;
				}
			}
		}
	}
	clock_t end = clock();		//the time brute force algorithm has finished 
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC;	//calculate the time passed since the algorithm had started
	printf("ALGORITHM PROGRESSING TIME = %lf\n",time_spent);
	return indices;
}
