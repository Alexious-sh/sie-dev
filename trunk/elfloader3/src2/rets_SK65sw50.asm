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
	DCD	0xA02A5F74+1

	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02A5D6E+1

	PUBLIC	ESIOld
ESIOld:
	DCD	0xA0BA55E0+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA0739232+1

	PUBLIC	PITret
PITret:
	DCD	0xA0739248+1

        PUBLIC	KEYBret
KEYBret:
	DCD	0xA072E98E+1

        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA073A3DC+1

	PUBLIC	FReadOld
FReadOld:
	DCD	0xA0945A60

	PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA0945B48

	defadr	StoreErrInfoAndAbort,0xA0946598
	defadr	StoreErrString,0xA09463BC


	END
