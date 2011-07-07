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
	DCD	0xA17143F0+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA171422C+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA1559B4C+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA0CB75A6+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA0CB75BC+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA0CACD02+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA0CB8F60+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA113B364
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA113B44C       
        
        defadr	StoreErrInfoAndAbort,0xA113BE98
	defadr	StoreErrString,0xA113BCBC

	END