//CL61v00 db128 (p)Evgr

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
	DCD	0xA07F0518+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA07F02B8+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA0AF3EFC+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0198C5C
	
	PUBLIC	PITret
PITret:
	DCD	0xA047E808+1
	
	defadr	StoreErrInfoAndAbort,0xA03FACC8
	defadr	StoreErrString,0xA03FAB94


	END