	RSEG	DATA_N
	RSEG	SWILIB_FUNC1B8_1BB:CODE
	EXTERN	EXT2_AREA
	EXTERN	pngtop
	EXTERN	pLIB_TOP
	DCD	EXT2_AREA
	DCD	pngtop
	DCD	pLIB_TOP
	DCD	SFE(DATA_N)

defadr	MACRO	a,b
	PUBLIC	a
a	EQU	b
	ENDM

        RSEG	CODE:CODE
        

	PUBLIC	OldOnClose
OldOnClose:
	DCD	0xA02FCD7A+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FCB52+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA097789E+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A27720
	
	PUBLIC	PITret
PITret:
	DCD	0xA0975D3E+1


	defadr	StoreErrInfoAndAbort,0xA01CD2E8
	defadr	StoreErrString,0xA01CD1B4


	END