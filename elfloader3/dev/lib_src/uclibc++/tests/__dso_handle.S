 
        .section .bss
        .align 4

#ifndef CRT_LEGACY_WORKAROUND
        .hidden __dso_handle
#endif

        .globl __dso_handle
__dso_handle:
        .long 0
