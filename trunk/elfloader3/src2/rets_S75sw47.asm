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
	DCD	0xA02FEDFA+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FEBE4+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA0977222+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A27504
	
	PUBLIC	PITret
PITret:
	DCD	0xA09756C2+1


	defadr	StoreErrInfoAndAbort,0xA01CEDBC
	defadr	StoreErrString,0xA01CEC88


	END