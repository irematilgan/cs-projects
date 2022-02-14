        PAGE 60,80
        TITLE ornek____
    
EXTRN _GCD_PROC:FAR

STACKSG SEGMENT PARA STACK 'STACK'
        DW 32 DUP(?)
STACKSG ENDS

DATASG  SEGMENT PARA 'DATA'
ASAYISI    DW 12
BSAYISI    DW 8
SONUC      DW ?
DATASG ENDS

CODESG  SEGMENT PARA 'CODE'
        ASSUME CS:CODESG, DS:DATASG, SS:STACKSG

BASLA   PROC FAR
        PUSH DS
        XOR AX,AX
        PUSH AX

        MOV AX, DATASG
        MOV DS, AX
        
        
        PUSH ASAYISI
        PUSH BSAYISI

        CALL _GCD_PROC

        ;POP AX          ; alternatif1
        ;POP AX          ; alternatif1
        ;MOV SONUC, AX   ; alternatif1


        ;POP AX          ; alternatif2
        ;MOV SONUC, AX   ; alternatif2

        MOV SONUC, AX   ; alternatif3        
        

        RETF
BASLA   ENDP

CODESG  ENDS
        END BASLA