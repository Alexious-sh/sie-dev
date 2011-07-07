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

       
#ifdef NEWSGOLD
#ifdef	ELKA

	PUBLIC	OldOnClose
OldOnClose:
	DCD	0xA066D3B8+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA066D178+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA094EA78+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A32284
	
	PUBLIC	PITret
PITret:
	DCD	0xA094CEA4+1
	
	defadr	StoreErrInfoAndAbort,0xA04CCDD4
	defadr	StoreErrString,0xA04CCCA0

#else

	PUBLIC	OldOnClose
OldOnClose:
	DCD	0xA02FCD7A+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FCB52+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA097789E+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A27720
	
	PUBLIC	PITret
PITret:
	DCD	0xA0975D3E+1


	defadr	StoreErrInfoAndAbort,0xA01CD2E8
	defadr	StoreErrString,0xA01CD1B4

#endif
#else
	
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
	DCD	0xA0CB602E+1  
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA113B364
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA113B44C       
        
        defadr	StoreErrInfoAndAbort,0xA113BE98
	defadr	StoreErrString,0xA113BCBC

#endif

	END