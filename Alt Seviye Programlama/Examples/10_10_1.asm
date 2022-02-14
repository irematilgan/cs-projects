STEK SEGMENT PARA STACK 'STK'
        DW 30 DUP(?)
STEK ENDS

DSG  SEGMENT PARA 'DTS'
D0 DW 1,2,2,3,4,5
DIZI DW 1,2,2,3,4,5,5,6,7,8,8,9, 250 DUP(11)
ELEMAN DW 262
CIFT DW 0
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
        MOV CX, ELEMAN
     L1:
        TEST WORD PTR DIZI[SI], 1
        JNZ TEK
        INC CIFT
     TEK:
        ADD SI, 2
        LOOP L1      
        ;CODE
        RETF
BASLA ENDP
CSG  ENDS
        END BASLA