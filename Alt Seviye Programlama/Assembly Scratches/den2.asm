
DSG SEGMENT PARA 'DATA'
	artim DW 0002H
	comp DW 000AH
DSG ENDS

CDS SEGMENT PARA 'CODE'
	ASSUME CS : CDS, DS : DSG
BASLA PROC FAR
	XOR AX,AX
	MOV SI,0001H
	L1:
		ADD AX,SI
		ADD SI,artim
		CMP SI,comp
		JA nextblock
		JMP L1
	nextblock:
BASLA ENDP
CDS ENDS
	END BASLA