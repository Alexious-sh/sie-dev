
	RSEG	SWILIB_FUNC2EE_2F5:CODE
	EXTERN	sub_elfclose
	EXTERN	dlopen
	EXTERN	dlsym
        EXTERN	dlclose
        EXTERN	setenv
        EXTERN	unsetenv
        EXTERN	getenv
        EXTERN	clearenv
        EXTERN  environ
	EXTERN  dlerror
        EXTERN  dlclean_cache
        EXTERN  SHARED_TOP
        
	DCD	sub_elfclose
	DCD	dlopen
	DCD	dlsym
        DCD     dlclose
        DCD	setenv
	DCD	unsetenv
        DCD     getenv
        DCD     clearenv
        DCD	environ
        DCD	dlerror
        DCD	dlclean_cache
	DCD	SHARED_TOP
	

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
	DCD	0xA08C39FE+1
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA08C37C4+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA1567830+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA0CC9B76+1
	
	PUBLIC	PITret
PITret:
	DCD	0xA0CC9B8C+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA0CBEA8A+1  
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA0CCAD40+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA112FE80
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA112FF68
        
        defadr	StoreErrInfoAndAbort,0xA11309A8
	defadr	StoreErrString,0xA11307CC

	END