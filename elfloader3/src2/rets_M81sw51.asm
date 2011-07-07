//M81v51
fnm 	RSEG	DATA_N
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
	DCD	0xA02FFD12+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FFAF8+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA096D5EA+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A250CC
	
	PUBLIC	PITret
PITret:
	DCD	0xA096BA8A+1


	defadr	StoreErrInfoAndAbort,0xA01CE44C
	defadr	StoreErrString,0xA01CE318


	END