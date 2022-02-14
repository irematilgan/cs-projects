#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


typedef struct pgmImage{
	int row;
	int col;
	int max_gray;
	unsigned char **data;
}pgmImage;

int** createMatrix(int row,int col) {
	int **mat;
	int i;
	
	mat = (int**)malloc(sizeof(int*)*row);
	if(mat == NULL) {
		perror("Memory Allocation Failure");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < row; i++) {
		mat[i] = (int*)malloc(sizeof(int**)*col);
		if(mat[i] == NULL) {
			perror("Memory Allocation Failure");
			exit(EXIT_FAILURE);
		}
	}
	
	return mat;
}

void freeMatrix(unsigned char** matrix, int row) 
{
	int i;
	for(i = 0; i < row; i++) {
		free(matrix[i]);
	}
	
	free(matrix);
}

pgmImage* getPGMFile(char* file_name, pgmImage* img) {
	
	FILE* pgmFile;
	char version[3];
	char line[100];
	int i,j;
	
	pgmFile = fopen(file_name,"rb");
	if(pgmFile == NULL) {
		perror("Cannot Open the File");
		exit(EXIT_FAILURE);
	}
	fgets(version,sizeof(version),pgmFile);
	
	if(strcmp(version, "P5") && strcmp(version,"P2")) {
		fprintf(stderr,"Wrong file type!..\n");
		exit(EXIT_FAILURE);
	}
	printf("\nVERSION = %s\n",version);
	
	//width, height ve max gray-level bilgileri alýnýr
	fgets(line,sizeof(line),pgmFile);
	fscanf(pgmFile,"%d",&img->col);
	getc(pgmFile);
	fscanf(pgmFile,"%d",&img->row);
	fgets(line,sizeof(line),pgmFile);
	fscanf(pgmFile,"%d",&img->max_gray);
	
	printf("ROW = %d\nCOLUMN = %d\nMax Gray-Level = %d\n",img->row,img->col,img->max_gray);
	
	fgets(line,sizeof(line),pgmFile);
	
	img->data = (unsigned char**)malloc(sizeof(unsigned char*)*(img->row));
	for(i = 0; i < img->row; i++) {
		img->data[i] = (unsigned char*)malloc(sizeof(unsigned char)*(img->col));
	}
	
	//Resimdeki pikseller okunur
	for(i = 0; i < img->row; i++) {
		fread(img->data[i],sizeof(unsigned char),img->col,pgmFile);
	}

	fclose(pgmFile);
	
	return img;
}

int writeImage(int** newImage, int rows, int cols, int max_gray,char* file_name) 
{
	int i,j;
	int temp;
	
	strcat(file_name,"_sobel.pgm");
	FILE* pgmFile = fopen(file_name,"wb");
	
	if(pgmFile == NULL) {
		printf("File cannot opened!..");
		return 0;
	}
	
	//P2 formatýnda pgm dosyasýnýn bilgileri girilir
	fprintf(pgmFile,"P2\n");
	fprintf(pgmFile,"%d %d\n",cols,rows); //width ve height
	fprintf(pgmFile,"%d\n",max_gray);
	

	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
			temp = newImage[i][j];
			fprintf(pgmFile,"%d ", temp);
		}
		fprintf(pgmFile,"\n");
	}
	
	fclose(pgmFile);
	return 1;
}


int** sobelFilter(pgmImage* image) 
{
	int i,j;
	int g,gradientX = 0,gradientY = 0;
	int sobelX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int sobelY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	char gx_name[5] = {"gx"};
	char gy_name[5] = {"gy"};
	
	if(image->col < 3 || image->row < 3) {
		printf("Sobel Filter Cannot be Applied!..Picture size at least should be 3x3\n");
		exit(EXIT_FAILURE);
	}
	
	int** filteredImage = createMatrix(image->row-2,image->col-2);
	int** filteredX = createMatrix(image->row-2,image->col-2);
	int** filteredY = createMatrix(image->row-2,image->col-2);
	
	//min-max normalizasyonu için gerekli deðiþkenler
	int max = 0,x_max = 0,y_max = 0;
	int min,x_min = 255,y_min = 255;
	int gap,x_gap,y_gap;
	
	for(i = 1; i < (image->row)-1; i++) {
		for(j = 1; j < (image->col)-1; j++) 
		{
			gradientX = (image->data[i-1][j-1]*sobelX[0][0] + image->data[i][j-1]*sobelX[1][0] + image->data[i+1][j-1]*sobelX[2][0] + image->data[i-1][j]*sobelX[0][1] + image->data[i][j]*sobelX[1][1] + image->data[i+1][j]*sobelX[2][1] + image->data[i-1][j+1]*sobelX[0][2] + image->data[i][j+1]*sobelX[1][2] + image->data[i+1][j+1]*sobelX[2][2]);
			gradientY = (image->data[i-1][j-1]*sobelY[0][0] + image->data[i][j-1]*sobelY[1][0] + image->data[i+1][j-1]*sobelY[2][0] + image->data[i-1][j]*sobelY[0][1] + image->data[i][j]*sobelY[1][1] + image->data[i+1][j]*sobelY[2][1] + image->data[i-1][j+1]*sobelY[0][2] + image->data[i][j+1]*sobelY[1][2] + image->data[i+1][j+1]*sobelY[2][2]);
			g = sqrt(pow(gradientX,2) + pow(gradientY,2));
	
			filteredImage[i-1][j-1] = g;
			filteredX[i-1][j-1] = gradientX;
			filteredY[i-1][j-1] = gradientY;
			
			if(max < g) max = g;
			else if(min > g) min = g;
		
			if(x_max < gradientX) x_max = gradientX;
			else if(x_min > gradientX) x_min = gradientX;
			
			if(y_max < gradientY) y_max = gradientY;
			else if(y_min > gradientY) y_min = gradientY;

		}
	}
	
	gap = max-min;
	x_gap = x_max-x_min;
	y_gap = y_max-y_min;
	
	//min-max normalizasyonu uygulanýr
	for(i = 0; i < (image->row)-2; i++) {
		for(j = 0; j < (image->col)-2; j++) {
			filteredImage[i][j] = (((float)filteredImage[i][j]-(float)min)/(float)gap)*255.0;
			filteredX[i][j] = (((float)filteredX[i][j]-(float)x_min)/(float)x_gap)*255.0;
			filteredY[i][j] = (((float)filteredY[i][j]-(float)y_min)/(float)y_gap)*255.0;
		}
	}
	
	//X ve Y yönündeki filtrelerin sonucu yazdýrýlýr
	writeImage(filteredX,(image->row)-2,(image->col)-2,255,gx_name);
	writeImage(filteredY,(image->row)-2,(image->col)-2,255,gy_name);

	//padding iþlemi için yeni matris oluþturulur
	int** paddedImage = createMatrix(image->row,image->col);

	if(paddedImage == NULL) {
		printf("Memory Allocation Failure!..\n");
		exit(EXIT_FAILURE);
	}

	
	//Ýlk ve son satýrlarýn tamamý sýfýrlanýr
	for(i = 0; i < image->col; i++){
		paddedImage[0][i] = 0;
		paddedImage[(image->row)-1][i] = 0;
	}
	
	//Tüm satýrlarýn ilk ve son hücreleri sýfýrlanýr
	for(i = 1; i < image->row; i++) {
		paddedImage[i][0] = 0;
		paddedImage[i][(image->col)-1] = 0;
	}
	
	//Geriye kalan hücreler filtrelenen matrisle doldurulur
	for(i = 0; i < (image->row)-2; i++) {
		for(j = 0; j < (image->col)-2; j++) {
			paddedImage[i+1][j+1] = filteredImage[i][j];
		}
	}
	

	return paddedImage;
}


int main()
{
	int i,j;
	char file_name[50],full_file_name[50];
	
	pgmImage* image = (pgmImage*)malloc(sizeof(pgmImage));
	
	printf("\nFile name you want to filter = ");
	scanf("%s",&file_name);
	strcpy(full_file_name,file_name);
	strcat(full_file_name,".pgm");
	
	//PGM dosyasý okunur
	getPGMFile(full_file_name,image);
	
	int** outputImage;
	outputImage = sobelFilter(image);
	
	//Filtrelenen resim yazdýrýlýr
	if(writeImage(outputImage,(image->row), (image->col), image->max_gray,file_name)) {
		printf("Filtered Image Saved Successfully!..");
	} 
	else {
		printf("Filtered Image cannot be saved!..");
		exit(EXIT_FAILURE);
	}
	
	//Resimler yazdýrýldýktan sonra matrisler bellekten temizlenir
	for(i = 0; i < image->row; i++) {
		free(outputImage[i]);
	}
	
	freeMatrix(image->data,image->row);
	free(image);
	
	return 0;
}
