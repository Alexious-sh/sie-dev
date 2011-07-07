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
	DCD	0xA02A78F2+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A76B0+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0A81EA8+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA07FF28E+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA07FF2A4+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA07F41FE+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA08011D0+1
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA07CA21C
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA07CA304
        
        defadr	StoreErrInfoAndAbort,0xA07CADB0
	defadr	StoreErrString,0xA07CABD4

	END