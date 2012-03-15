
#include "ep3.asm"


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
	DCD	0xA02A7586+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A732E+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0A14BA0+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA07B111A+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA07B1130+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA07A6046+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA07B305C+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA078074C
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA0780834       
        
        defadr	StoreErrInfoAndAbort,0xA0781274
	defadr	StoreErrString,0xA0781098

	END