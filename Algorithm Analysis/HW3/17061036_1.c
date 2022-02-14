#include <stdio.h>
#include <stdlib.h>

//Matris olusturma fonksiyonu
double** createMatrix(int dim)
{
	int i;
	double **matrix;
	matrix = calloc(dim,sizeof(double*));
	for(i = 0; i < dim; i++)
	{
		matrix[i] = calloc(dim,sizeof(double));
	}
	
	return matrix;
}

//Dinamik programlama yolu ile olasilik hesaplayan fonksiyon
double calcProb(double prob, int win) {
	int i,j;
	double **matrix;
	matrix = createMatrix(win+1); //Matris olusturulur	
	//A takiminin kazanmasi gereken mac kalmadiysa P(0,j) seriyi kazanma olasiligi 1'dir		  
	for(i = 0; i <= win; i++)
		matrix[0][i] = 1;
	
	//B takiminin kazanacak maci kalmadiysa seriyi kazanmislardir
	//Bu durumda A'nin seriyi kazanma olasiligi sifirdir	
	for(i = 0; i <= win; i++)
		matrix[i][0] = 0;
		
	//2. satir ve 2. sutundan itibaren (ilk satir yukarida dolduruldu) hucrelerin degeri hesaplanir 
	for(i = 1; i <= win; i++)
	{
		for(j = 1; j <= win; j++)
		{
			//P(i,j) = probA * P(i-1,j) + probB * P(i,j-1) recursive fonksiyonu kullanilir
			matrix[i][j] = matrix[i-1][j]*(prob) + matrix[i][j-1]*(1-prob);	
		}	
	}
	printf("\nMatrix\n--------\n");
	for(i = 0; i <= win; i++)
	{
		for(j = 0; j <= win; j++)
		{
			printf("%lf\t",matrix[i][j]);
		}	
		printf("\n");
	}
	
	return matrix[win][win];
}



int main()
{
	int num_games;	//Serideki toplam oyun sayisi
	int win;		//Seriyi kazanmak icin kazanilmasi gereken oyun sayisi
	double result;	//Sonuc olasiligin tutuldugu degisken
	double p;		//A takiminin bir maci kazanma olasiligi
	printf("Total # of match in series = ");
	scanf("%d",&num_games);
	printf("Possibility of winning one match for A team = ");
	scanf("%lf",&p);
	win = num_games/2 + 1;
	
	result = calcProb(p,win);
	printf("\nPossibility of winning series for A is %lf%%\n",result*100);
	
	return 0;
}
