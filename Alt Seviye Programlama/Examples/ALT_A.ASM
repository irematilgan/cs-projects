        PAGE 60,80
        TITLE ornek____

PUBLIC _GCD_PROC  

CDESG  SEGMENT PARA 'CODE'
        ASSUME CS:CDESG

_GCD_PROC PROC FAR

        PUSH BP
        MOV BP, SP
	PUSH AX
	; alternatif3 PUSH AX olmayacak
	PUSH BX
	PUSH DX

        MOV BX, [BP+6]
        MOV AX, [BP+8]
        CMP AX, BX
        JAE TEKRAR
        XCHG AX, BX
TEKRAR:	CMP BX, 0
        JA L1
        JMP EXIT 
L1:     XOR DX, DX
        DIV BX
        MOV AX, BX
        MOV BX, DX
        JMP TEKRAR 

EXIT:   
	MOV [BP+8], AX
	POP DX
	POP BX
	POP AX ; alternatif1, alternatif2
	; alternatif3 POP AX olmayacak
	POP BP
        RETF ; alternatif1
	;RETF2 ;alternatif2
	;RETF 4 ; alternatif3

_GCD_PROC ENDP

CDESG  ENDS
        END