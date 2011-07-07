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
	DCD	0xA02A78EE+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A7726+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0A8329C+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA0800046+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA080005C+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA07F4F86+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA0801F88+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA07CAEDC
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA07CAFC4
        
        defadr	StoreErrInfoAndAbort,0xA07CBA70
	defadr	StoreErrString,0xA07CB894

	END