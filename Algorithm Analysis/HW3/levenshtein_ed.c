#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int** create2DMatrix(int dim1,int dim2)
{
	int i;
	int **matrix;
	matrix = calloc(dim1,sizeof(int*));
	for(i = 0; i < dim1; i++)
	{
		matrix[i] = calloc(dim2,sizeof(int));
	}
	
	return matrix;
}

int findDif(char a, char b)
{
	if(a > 96)	a = a - ('A'-'a');
	if(b > 96)	b = b - ('B'-'b');
	if(a-b)	return 1;
	return 0;
}

int findMin(int opt1, int opt2, int opt3)
{
	if(opt1 < opt2 && opt1 < opt3)	return opt1;
	else
	{
		if(opt2 < opt1 && opt2 < opt3)	return opt2;
		else	return opt3;
	}
}


int LED(char* ref, char* old)
{
	int i;
	int j;
	int flag = 0;
	int opt1,opt2,opt3;
	int M = strlen(ref);
	int N = strlen(old);
	
	int** mat = create2DMatrix(M+1,N+1);
	printf("%d\t%d\n",M,N);
	i = 0;
	while(i <= M && flag != N+1)
	{
		flag = 0;
		//printf("%d\t",i);
		for(j = 0; j <= N; j++)
		{
			if(i == 0 && j == 0)	
				mat[i][j] = 0;
			else if(j == 0)	
				mat[i][j] = mat[i-1][j] + 1;
			else if(i == 0)	
				mat[i][j] = mat[i][j-1] + 1;
			else
			{
				//printf("a");
				opt1 = mat[i-1][j] + 1;
				opt2 = mat[i][j-1] + 1;
				opt3 = mat[i-1][j-1] + findDif(ref[i-1],old[j-1]);
				mat[i][j] = findMin(opt1,opt2,opt3);
			}
			if(mat[i][j] > 5)
				flag++;
			
			//printf("%d-%d\t",j,mat[i][j]);
		}
		//printf("\n");
		
		i++;
	}
	
	printf("FLAG = %d\n",flag);
	if(flag >= N+1) printf("Kotayi astik!..");
	else
	{
		for(i = 0; i <= M; i++)
		{
			for(j = 0; j <= N; j++)
			{
				printf("%d\t",mat[i][j]);
			}
			printf("\n");
		}
		
	}
		
	
	return mat[M][N];
}

int main()
{
	char str1[10] = {"collisions"};
	char str2[10] = {"coold"};
	
	int distance = LED(str2,str1);
	printf("\nDISTANCE = %d",distance);
	
	return 0;
}
