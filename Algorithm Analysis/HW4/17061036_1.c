#include <stdio.h>
#include <stdlib.h>

void printMatrix(int**, int); 	//Matris yazdirma fonksiyonu
int** getMatrix(int);			//Matris okuma fonksiyonu
int* swapRow(int*,int);			//Satir kaydirma fonksiyonu
int checkRows(int**,int,int);	//Ust satirlardaki cakismalarin kontrol edildigi fonksiyon
int swapMatrix(int**,int,int);	//Backtracking fonksiyonu

//Matris yazdirma fonksiyonu
void printMatrix(int** mat, int N)
{
	int i;
	int j;
	//sayilarin renk karsiliklarini tutan char matrisi
	char colors[8][20] = {"Kirmizi","Yesil","Mavi","Mor","Turuncu","Siyah","Beyaz","Sari"};
	printf("\n\t\t\t");
	for(i = 0 ;i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("%s\t\t",colors[mat[i][j]-1]);
		}
			
		printf("\n\t\t\t");
	}

}

//Matris okuma fonksiyonu
int** getMatrix(int N)
{
	int i;
	int j;
	int** mat;	//renklerin tutuldugu sayi matrisi
	mat = (int**)calloc(N,sizeof(int*));
	for(i = 0; i < N; i++)
		mat[i] = (int*)calloc(N,sizeof(int));
	
	//matris degerleri kullanicidan alinir
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("\n\t\t\t");
			printf("MAT[%d][%d] = ",i,j);
			scanf("%d",&mat[i][j]);
		}
	}
	return mat;	
}

//Satir kaydirma fonksiyonu
int* swapRow(int* row, int dim)
{
	int tmp = row[dim-1];	//son sutun elemani gecici degiskende tutulur
	int i;
	for(i = dim-1; i > 0; i--)
	{
		row[i] = row[i-1];	//son sutundan baslayarak kaydirma yapilir
	}
	row[i] = tmp;
	
	return row;
}

//Ust satirlardaki cakismalarin kontrol edildigi fonksiyon
int checkRows(int** mat, int index, int dim)
{
	int i;
	int j;
	//Mevcut satirdan bir önceki satirla baslanarak kontrol yapilir
	for(i = index-2; i >= 0; i--)
	{
		for(j = 0; j < dim; j++)
		{
			if(mat[i][j] == mat[index-1][j])
				return 1;	//Herhangi iki sutunda cakisma oldugu an 1 donulur
		}
	}
	return 0; //Cakisma hic olmadiysa 0 donulur
}

//Backtracking fonksiyonu
int swapMatrix(int** mat, int N, int rowIndex)
{
	int count;			//mevcut satirda kac kez saga kaydirilma islemi yapildigini tutar
	int result;			//ust satirlarla cakisma olup olmadiginin sonucunu tutar
	int swap_result;	//backtracking fonksiyonunun sonucunu tutar
	
	//Son satir gecilmemisse
	if(rowIndex <= N)
	{
		count = 0;
		//sutun sayisi kadar saga kaydirma islemi yapilmadiysa 
		while(count < N)
		{
			//ust satirlarla cakisma var mi diye kontrol edilir
			result = checkRows(mat, rowIndex, N);
			if(result)	//cakisma varsa
			{
				mat[rowIndex-1] = swapRow(mat[rowIndex-1],N);	//satir saga kaydirilir
			}	
			else	//cakisma yoksa
			{
				swap_result = swapMatrix(mat,N,rowIndex+1);		//bir sonraki satir icin fonksiyon cagirilir
				if(swap_result)	//eger matris basarili bir sekilde ayarlanabildiyse matris yazdirilir ve 1 donulur
				{ 
					printMatrix(mat,N);	
					return 1;
				}
				mat[rowIndex-1] = swapRow(mat[rowIndex-1],N);	//matris ayarlanamadiysa mevcut satir yeniden saga kaydirilir
			}
			count++;	//yapilan kaydirma islemi icin sayac bir artirilir
		}
		return 0;
		
	}
	else //Son satir gecilmisse matris basarili bir sekilde ayarlanmis demektir
	{
		return 1;
	}
		
}

int main()
{
	int N;		//Matris satir/sutun sayisi
	int result;	//Sutunlari cakismayan bir matrisin olusturulup olusturulamadiginin sonucunu tutan degisken
	int** mat;	//Renk degerlerinin tutuldugu sayi matrisi
	
	printf("\n\t\t\t");
	printf("=== KURALLAR ===");
	printf("\n\t\t\t");
	printf("1. Renk Listesi");
	printf("\n\t\t\t");
	printf("1 = Kirmizi\t2 = Yesil\t3 = Mavi\t4 = Mor\t5 = Turuncu\t6 = Siyah\t7 = Beyaz\t8 = Sari");
	printf("\n\t\t\t");
	printf("Matrisin boyutlarini girip matrisi doldurduktan sonra saga kaydirma islemi gerceklestirebilirsiniz.");
	printf("\n\t\t\t");
	printf("Renk matrisinin boyutlarini giriniz (3 <= N <= 8) :  ");
	scanf("%d",&N);
	
	mat = getMatrix(N); //Matris degerleri kullanicidan alinir
	
	
	printf("\n\t\t\tBASLANGIC MATRISI = ");
	printf("\n\t\t\t");
	printMatrix(mat,N);
	printf("\n\t\t\t");
	result = swapMatrix(mat,N,1);	//Backtracking fonksiyonu
	if(result)	//Eger istediklerimize uyan matris olusturulabildiyse yazdirilir
	{
		printf("\n\t\t\tSONUC MATRISI = ");
		printf("\n\t\t\t");
		printMatrix(mat,N);
	}
	else
		printf("Matris donusturulemiyor!..");
	
	return 0;
}
