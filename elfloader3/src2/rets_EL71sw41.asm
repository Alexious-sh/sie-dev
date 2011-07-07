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
	DCD	0xA066E5D6+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA066E374+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA09554F2+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A38D44
	
	PUBLIC	PITret
PITret:
	DCD	0xA095393C+1

	defadr	StoreErrInfoAndAbort,0xA04D2304
	defadr	StoreErrString,0xA04D21D0


	END