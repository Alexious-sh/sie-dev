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
	DCD	0xA02A7A68+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A781C+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0A84760+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA0800EDE+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA0800EF4+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA07F5E0A+1  
   
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA0802E20+1  
     
        PUBLIC	FReadOld
FReadOld:
	DCD	0xA07CBA7C
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA07CBB64       
        
        defadr	StoreErrInfoAndAbort,0xA07CC610
	defadr	StoreErrString,0xA07CC434

	END