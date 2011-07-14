	RSEG	DATA_N
	RSEG	SWILIB_FUNC1B8_1BB:CODE
	EXTERN	EXT2_AREA
	EXTERN	pngtop
	EXTERN	pLIB_TOP
	DCD	EXT2_AREA
	DCD	pngtop
	DCD	pLIB_TOP
	DCD	SFE(DATA_N)
        
        
        RSEG	SWILIB_FUNC2EE_2F5:CODE
	EXTERN	elfclose
	EXTERN	dlopen
	EXTERN	dlsym
        EXTERN	dlclose
        EXTERN	setenv
        EXTERN	unsetenv
        EXTERN	getenv
        EXTERN	clearenv
        
	DCD	elfclose
	DCD	dlopen
	DCD	dlsym
        DCD     dlclose
        DCD	setenv
	DCD	unsetenv
        DCD     getenv
        DCD     clearenv
        

defadr	MACRO	a,b
	PUBLIC	a
a	EQU	b
	ENDM

        RSEG	CODE:CODE
        

	PUBLIC	OldOnClose
OldOnClose:
	DCD	0xA02FF192+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02FEF78+1
	
	PUBLIC	OldShowMsg
OldShowMsg:
	DCD	0xA0977CF2+1
	
	PUBLIC	PITgetN
PITgetN:
	DCD	0xA0A280B0
	
	PUBLIC	PITret
PITret:
	DCD	0xA0976192+1


	defadr	StoreErrInfoAndAbort,   0xA01CEE50
	defadr	StoreErrString,         0xA01CED1C
        defadr  NU_Sleep,               0xA00A275C
        defadr  NU_Suspend_Task,        0xA00A26FC
        defadr  NU_Resume_Task,         0xA00A26D8
        defadr  NU_Current_Task_Pointer,0xA00A2030

	END