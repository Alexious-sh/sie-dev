//C81v51
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
	DCD	0xA02FFCEE+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FFAD4+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA096D5C6+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A250A8
	
	PUBLIC	PITret
PITret:
	DCD	0xA096BA66+1


	defadr	StoreErrInfoAndAbort,0xA01CE3F8
	defadr	StoreErrString,0xA01CE2C4


	END
