STEK SEGMENT PARA STACK 'STK'
	DW 30 DUP(?)
STEK ENDS

DATS SEGMENT PARA 'DTS'
	
DATS ENDS

CSG SEGMENT PARA 'CODE'
	ASSUME CS : CSG SS : STEK DS : DATS
BASLA PROC FAR
	CMP AX,0
	JNZ NOTSIFIR
	MOV BX,1
	NOTSIFIR:
		MOV CX,5
BASLA ENDP
CSG ENDS
	END BASLA