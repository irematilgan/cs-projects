STEK SEGMENT PARA STACK 'STK'
        DW 30 DUP(?)
STEK ENDS

DSG  SEGMENT PARA 'DTS'
D0 DW 1,2,2,3,4,5
DIZI DB 1,2,2,3,4,5,5,6,7,8,8,9, 200 DUP(11)
ELEMAN DB 212
ORT DB 0
DSG ENDS  

CSG  SEGMENT PARA 'CODE'
        ASSUME CS:CSG, DS:DSG, SS:STEK
BASLA PROC FAR        
        
        PUSH DS
        XOR AX,AX
        PUSH AX
        
        MOV AX, DSG
        MOV DS, AX 
        ;CODE
        XOR SI, SI
        XOR CX, CX
        MOV CL,ELEMAN
        XOR AX, AX
        
     L1:
        XOR BX, BX
        MOV BL, DIZI[SI] 
        ADD AX, BX
        INC SI
     
        LOOP L1
        
        DIV ELEMAN 
        MOV ORT, AL
         
        
        ;CODE
        RETF
BASLA ENDP
CSG  ENDS
        END BASLA