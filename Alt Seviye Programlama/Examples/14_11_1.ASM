        PAGE 60,80
        TITLE ornek____
        
CODESG  SEGMENT PARA 'CODE'
        ORG 100H
        ASSUME CS:CODESG, SS:CODESG, DS:CODESG, ES:CODESG

BASLA   PROC NEAR

        LEA DX, MSG
        MOV AH, 09H

        INT 21H


        RET
BASLA   ENDP

MSG     DB 'DENEME', '$'

CODESG  ENDS
        END BASLA
