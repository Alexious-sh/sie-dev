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
	DCD	0xA02D252E+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02D2316+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA07FCF2A+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA08A098C
	
	PUBLIC	PITret
PITret:
	DCD	0xA07FB3CA+1


	defadr	StoreErrInfoAndAbort,0xA018D9E8
	defadr	StoreErrString,0xA018D8B4


	END