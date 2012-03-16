

        RSEG	SWILIB_FUNC2EE:CODE
        EXTERN	elfclose
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
        EXTERN	sub_elfclose
        
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
        DCD	elfclose
