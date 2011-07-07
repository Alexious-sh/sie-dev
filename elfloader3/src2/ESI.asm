        RSEG    CODE:CODE(2)
        PUBLIC  BXR1
BXR1:
        BX      R1
        
   

#ifdef NEWSGOLD

        THUMB
        RSEG    PATCH_NO_BROWSE_TXT:CODE:ROOT(1)
        ADDS    R0,R0,#8+0x28

	RSEG	PATCH_ESI1:CODE:ROOT(2)
	ARM
	LDR	PC,pESI
	RSEG	PATCH_ESI2:DATA(2)
pESI
	DCD	callESI

	EXTERN	ESI

	RSEG	CODE:CODE(2)
	ARM
callESI:
	BL	ESI
	MOV	R0, #1
	LDMFD	SP!, {R4-R8,PC}
	
	THUMB
	RSEG	PATCH_GET_PIT:CODE(1)
//MAINCODE:A0975D34 10 B5                       PUSH    {R4,LR}
//MAINCODE:A0975D36 04 1C                       ADD     R4, R0, #0
//MAINCODE:A0975D38 B1 F0 F2 EC                 BLX     sub_A0A27720
	LDR	R1,JJ_PIT
	BX	R1
	
	RSEG	PATCH_GET_PITJ:DATA(2)
JJ_PIT	DC32	J_PIT

	RSEG	CODE:CODE(2)
	EXTERN	PITgetN
	EXTERN	PatchGetPIT
	EXTERN	PITret
	ARM
J_PIT:
	STMFD	SP!,{R4,LR}
	MOV	R4,R0
	BL	PatchGetPIT
        CMP     R0, #0
	LDMNEFD	SP!,{R4,PC}
	MOV	R0,R4
	LDR	R12,=PITgetN
        LDR     R12, [R12]
	BLX	R12
	CMP	R0,R4
        LDR	R12,=PITret
        LDR     R12, [R12]
	BX	R12
#else      
; ==============================================   
	RSEG	PATCH_ESI:CODE:ROOT
	CODE16
	LDR	R3,=ESI_PATCH
	BX	R3
        
; ----------------------------------------------    
	RSEG	CODE:CODE:NOROOT
	CODE32
	EXTERN	ESI
        EXTERN  ESIOld
ESI_PATCH:
	MOV	R3,R0,LSR #28
	CMP	R3,#0x0A
        LDMEQFD SP!,{R4,LR}
        BEQ     ESI 
DO_LGP:
        LDR	R3,=ESIOld
        LDR     R3, [R3]
	MOV	R4,R1
	MOV	R1,R2
	LDR	R2,=0x2710
	CMP	R0,R2
	BX	R3
        
; ==============================================       
 	RSEG	PATCH_NO_UNK_EXT:CODE:ROOT
	CODE32

        BL      NoUnknownExt

        
; ----------------------------------------------   
        RSEG	CODE:CODE:NOROOT
	CODE32

NoUnknownExt:
        MOVS    R0, R4
        BXNE    LR
        STMFD   SP!,{LR}
        MOV     R4, #0x7FFFFFFF
        MOV     R0, R4
        SWI     0x1CF
        CMP     R0, #0
        MOVNE   R0, R4
        LDMFD   SP!,{PC}
        
      
        
; ==============================================               
	THUMB
	RSEG	PATCH_GET_PIT:CODE:ROOT(2)
//MAINCODE:A0975D34 10 B5                       PUSH    {R4,LR}
//MAINCODE:A0975D36 04 1C                       ADD     R4, R0, #0
//MAINCODE:A0975D38 B1 F0 F2 EC                 BLX     sub_A0A27720
	LDR	R1,JJ_PIT
	BX	R1
        
        RSEG	PATCH_GET_PITJ:DATA(2)
JJ_PIT	DC32	J_PIT
        
             
; ----------------------------------------------       
	RSEG	CODE:CODE(2)
	EXTERN	PITgetN
	EXTERN	PatchGetPIT
	EXTERN	PITret
	ARM
J_PIT:
	STMFD	SP!,{R4,LR}
	MOV	R4,R0
	BL	PatchGetPIT
        CMP     R0, #0
	LDMNEFD	SP!,{R4,PC}
	MOV	R0,R4
	LDR	R12,=PITgetN
        LDR     R12, [R12]
	BLX	R12
        CMP	R0,R4
	LDR	R12,=PITret
        LDR     R12, [R12]
	BX	R12
        
; ================================================

        THUMB 
        RSEG    PATCH_KEYB_MSG:CODE:ROOT(1)
        LDR     R2, JJ_KEYB
        BX      R2
        
        THUMB 
        RSEG	PATCH_KEYB_MSGJ:DATA(2)
JJ_KEYB DC32    J_KEYB

        
        RSEG    CODE:CODE:NOROOT
        CODE16
        EXTERN  PatchKeybMsg
        EXTERN  KEYBfunc
        EXTERN  KEYBret
J_KEYB: 
        LDR     R0, [R6,#4]
        LDRH    R1, [R6,#2]
        BL      PatchKeybMsg
        CMP     R0, #2
        BEQ     EXIT
        
        ADD     R1, R6, #0
        LDR     R0, [R5,#0x2C]
        LDR     R2, =KEYBfunc
        LDR     R2, [R2, #0]
        BLX     R2
        CMP     R0, #0
        BNE     EXIT
        
        LDR     R0, =KEYBret
        LDR     R0, [R0, #0]
        BX      R0
        
EXIT:   POP     {R3-R7,PC}

#endif

	END




