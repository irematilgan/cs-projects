STEK SEGMENT PARA STACK 'STACK'
	DW 50 DUP(?)
STEK ENDS

DSG SEGMENT 'DATAS'
	Dizi DB 1,2,3,4,5,6,7
	eleman DB 3
DSG ENDS

CSG SEGMENT PARA 'CODE'
	ASSUME DS:DSG, CS:CSG, SS: STEK
BASLA PROC FAR
	
		PUSH DS
		XOR AX,AX
		MOV AX,DSG
		MOV DS,AX
		
		;CODE
		XOR CX,CX
		XOR SI,SI
		MOV CL,eleman
		
		
L1 : MOV AL,Dizi[SI]
	 INC SI  
     LOOP L1    
		  
    RETF
BASLA ENDP
CSG ENDS
    END BASLA
		  
		 