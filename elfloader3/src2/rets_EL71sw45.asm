//EL71v45
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
	DCD	0xA066F69A+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA066F438+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA09587DE+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A3C030
	
	PUBLIC	PITret
PITret:
	DCD	0xA0956C28+1

	defadr	StoreErrInfoAndAbort,0xA04D319C
	defadr	StoreErrString,0xA04D3068


	END