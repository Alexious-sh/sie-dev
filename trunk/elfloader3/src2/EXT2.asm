;
; //Extend file extention
;
#ifdef NEWSGOLD

#ifdef	ELKA

	RSEG	PATCH_ENABLE_ALTOPEN:DATA:ROOT(2)
	DCD	0xE3570000

	ARM
	RSEG	PATCH_EXT2_R7:CODE:ROOT(2)
	BL	J_R7
	RSEG	PATCH_EXT2_R6:CODE:ROOT(2)
	BL	J_R6
	RSEG	PATCH_EXT2_R7_1:CODE:ROOT(2)
	BL	J_R7
	RSEG	PATCH_EXT2_R2:CODE:ROOT(2)
	BL	J_R2
	RSEG	CODE:CODE:NOROOT(2)
	EXTERN	EXT2_AREA
J_R7:
	LDR	R7,=EXT2_AREA
	LDR	R7,[R7]
	BX	LR
J_R6:
	LDR	R6,=EXT2_AREA
	LDR	R6,[R6]
	BX	LR
J_R2:
	LDR	R2,=EXT2_AREA
	LDR	R2,[R2]
	BX	LR

	RSEG	PATCH_EXT2_IMPL:CODE:ROOT(2)
	EXTERN	EXT2_REALLOC
	BL	EXT2_REALLOC
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

#else

	RSEG	PATCH_ENABLE_ALTOPEN:DATA:ROOT(1)
	DCW	0x2800

        THUMB
        RSEG    PATCH_EXT2_R4R7:CODE:ROOT(1)
	BLX	J_R4R7
        RSEG    PATCH_EXT2_R6R5:CODE:ROOT(1)
	BLX	J_R6R7
        RSEG    PATCH_EXT2_R5R1:CODE:ROOT(1)
	BLX	J_R5R1
	RSEG    PATCH_EXT2_R5R7:CODE:ROOT(1)
	BLX	J_R5R7
        RSEG    PATCH_EXT2_CLUST1:CODE:ROOT(1)
	MOVS	R0,R7
        RSEG    PATCH_EXT2_CLUST2:CODE:ROOT(1)
	MOVS	R0,R7

	RSEG	PATCH_EXT2_IMPL:CODE:ROOT(1)
	BLX	J_EXT2_REALLOC
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	RSEG	PATCH_EXT2_JUMPERS:CODE:NOROOT(2)
	ARM
J_R4R7
	LDR	PC,=I_R4R7
J_R6R7
	LDR	PC,=I_R6R7
J_R5R1
	LDR	PC,=I_R5R1
J_R5R7
	LDR	PC,=I_R5R7
J_EXT2_REALLOC
	EXTERN	EXT2_REALLOC
	LDR	PC,=EXT2_REALLOC

	RSEG	CODE:CODE:NOROOT(2)
	EXTERN	GET_EXT2_TABLE
I_R4R7
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R7,R0
	MOV	R4,#0
	LDMFD   SP!,{R0-R3, PC}
I_R6R7
	ADD	R5,R0,#0
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R6,R0
	LDMFD   SP!,{R0-R3, PC}
I_R5R1
	MOV	R1,#0
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R5,R0
	LDMFD   SP!,{R0-R3, PC}
I_R5R7
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R7,R0
	MOV	R5,#0
	LDMFD   SP!,{R0-R3, PC}
#endif	

#else
        ARM 
        RSEG    PATCH_GET_EXT:CODE:ROOT
        BL      J_GET_EXT
        
        ARM 
        RSEG    PATCH_GET_REGEXT1:CODE:ROOT
        BL      J_REGEXT1
        
        ARM 
        RSEG    PATCH_GET_REGEXT2:CODE:ROOT
        BL      J_REGEXT2
        
        ARM
	RSEG	REG_FILE:CODE:ROOT
        EXTERN  RegFile
        
        LDR     PC, =RegFile
        
        ARM
        RSEG    PATCH_BIG_ICON:CODE:ROOT
        EXTERN  GetBigIcon
        BL      GetBigIcon
        
        ARM
        RSEG    PATCH_REGEXPL:CODE:ROOT
        
        BL      J_REG_FILE
        
        ARM
	RSEG	CODE:CODE:NOROOT
        EXTERN  EXT2_AREA
        EXTERN  RegFile

        
J_GET_EXT
        LDR     R7, =EXT2_AREA
        LDR     R7, [R7]
        MOV     R8, #0x24
        MLA     R7, R8, R5, R7
        BX      LR

J_REGEXT1
        LDR     R12, =EXT2_AREA
        LDR     R12, [R12]        
        MOV     R2, #0x24
        MLA     R12, R2, R1, R12
        BX      LR
        
J_REGEXT2
        LDR     R0, =EXT2_AREA
        LDR     R0, [R0]
        MOV     R2, #0x24
        MLA     R0, R2, R1, R0
        BX      LR        
        
       
J_REG_FILE
        LDR     R5, [R4, #0x10]
        STR     R5, [SP, #0x10]
        B       RegFile
        
#endif

        RSEG	CODE:CODE:NOROOT(2)
        PUBLIC	__e_div //int __e_div(int delitelb, int delimoe);
__e_div:

    STMFD   SP!, {LR}
    SWI     0xB8
    MOV     R0, R1
    LDMFD   SP!, {PC}
    
    
    
    END
	