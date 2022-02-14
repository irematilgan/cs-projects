#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void SteganografiBul(int n, int resimadres_org, int resimadres_ste, int steganografi_adres);

int main(void) {
	int M, N, Q, i, j;
	bool type;
	int efile;
	char org_resim[100], ste_resim[100], steganografi[100];
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	do {
		printf("Steganografik resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &ste_resim);
		system("CLS");
		efile = readImageHeader(ste_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_ste = resimOku(ste_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);
	printf("SteganografiK Resim Yolu: \t\t%s\n", ste_resim);

	short *resimdizi_org, *resimdizi_ste;
	resimdizi_org = (short*) malloc(N*M * sizeof(short));
	resimdizi_ste = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++) 
		for (j = 0; j < M; j++) {
			resimdizi_org[i*N + j] = (short) resim_org[i][j];
			resimdizi_ste[i*N + j] = (short) resim_ste[i][j];
		}


	int resimadres_org = (int) resimdizi_org;
	int resimadres_ste = (int) resimdizi_ste;
	int steganografi_adres = (int) steganografi;
	
	

	SteganografiBul(N*M, resimadres_org, resimadres_ste, steganografi_adres);
	
	printf("\nResim icerisinde gizlenmis kod: \t%s\n", steganografi);
	
	system("PAUSE");
	return 0;
}

void SteganografiBul(int n, int resim_org, int resim_ste, int steganografi_adres) {
	
	__asm {
		
		XOR EDI, EDI
		XOR ESI, ESI
		XOR ECX, ECX
		XOR EBX, EBX
		XOR AX,AX

		MOV ECX,n 
		
		MOV EDX, resim_ste  

	L1:
		MOV EBX, resim_org	
		MOV AL, BYTE PTR[EBX + EDI] 
		MOV EBX, resim_ste  
		MOV AH, BYTE PTR[EBX + EDI] 
		
		CMP AH,AL 
		JZ L2 
		
		
		SUB AH,AL 
		JG POSITIVE
		ADD AH,256
		
	POSITIVE:
		
		MOV EDX, steganografi_adres	
		MOV BYTE PTR [EDX+ESI], AH
		INC ESI
		

	L2:
		ADD EDI,2
		LOOP L1
	
	
	MOV BYTE PTR[EDX + ESI], ' '
	MOV BYTE PTR[EDX + ESI + 1], '-'
	MOV BYTE PTR[EDX + ESI + 2], ' '
	MOV BYTE PTR[EDX + ESI + 3], '1'
	MOV BYTE PTR[EDX + ESI + 4], '7'
	MOV BYTE PTR[EDX + ESI + 5], '0'
	MOV BYTE PTR[EDX + ESI + 6], '6'
	MOV BYTE PTR[EDX + ESI + 7], '1'
	MOV BYTE PTR[EDX + ESI + 8], '0'
	MOV BYTE PTR[EDX + ESI + 9], '3'
	MOV BYTE PTR[EDX + ESI + 10], '6'
	MOV BYTE PTR [EDX + ESI + 11], 0h

		
	}

	
}
