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
	DCD	0xA1697962+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA16977BA+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA1405EA0+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA11599B2+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA11599C8+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA114F10E+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA115B8D4+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA122ECF0
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA122EDD8       
        
        defadr	StoreErrInfoAndAbort,0xA113BE98
	defadr	StoreErrString,0xA113BCBC

	END
        