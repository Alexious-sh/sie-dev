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
	DCD	0xA02A7576+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A731E+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0A148D8+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA07B0F96+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA07B0FAC+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA07A5EC2+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA07B2ED8+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA07806B4
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA078079C       
        
        defadr	StoreErrInfoAndAbort,0xA07811DC
	defadr	StoreErrString,0xA0781000

	END