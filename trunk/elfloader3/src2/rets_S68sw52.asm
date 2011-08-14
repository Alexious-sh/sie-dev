//(p) Hotter

        RSEG	SWILIB_FUNC2EE_2F5:CODE
	EXTERN	sub_elfclose
	EXTERN	dlopen
	EXTERN	dlsym
        EXTERN	dlclose
        EXTERN	setenv
        EXTERN	unsetenv
        EXTERN	getenv
        EXTERN	clearenv
        
	DCD	sub_elfclose
	DCD	dlopen
	DCD	dlsym
        DCD     dlclose
        DCD	setenv
	DCD	unsetenv
        DCD     getenv
        DCD     clearenv


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
	DCD	0xA02D29B2+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02D2792+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA07FD766+1

	PUBLIC	PITgetN
PITgetN:
	DCD	0xA08A12A4
	
	PUBLIC	PITret
PITret:
	DCD	0xA07FBC06+1


	defadr	StoreErrInfoAndAbort,0xA018DB5C
	defadr	StoreErrString,0xA018DA28
	defadr  NU_Sleep,               0xA00A1FB8
        defadr  NU_Suspend_Task,        0xA00A1F58
        defadr  NU_Resume_Task,         0xA00A1F34
        defadr  NU_Current_Task_Pointer,0xA00A188C


	END