STEK SEGMENT PARA STACK 'STK'
        DW 30 DUP(?)
STEK ENDS

DSG  SEGMENT PARA 'DTS'
ASAYISI DW 0C123H
BSAYISI DW 9876H
SONUC DD 0
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
        LEA SI, SONUC
        MOV AX, ASAYISI
        ADD AX, BSAYISI
        
        MOV [SI], AX
        ADC WORD PTR [SI+2],0 
         
        
        ;CODE
        RETF
BASLA ENDP
CSG  ENDS
        END BASLA