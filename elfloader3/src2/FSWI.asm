;SWI dispatcher v1.1 (C)2006 by Rst7/CBSIE
;

        EXTERN  SWILIB
	EXTERN	pLIB_TOP
	EXTERN	FUNC_ABORT
	RSEG	FSWI_PATCH1:CODE:ROOT
	CODE32

	LDR	PC,main_jumper
        
        #ifndef NEWSGOLD
        RSEG	FSWI_PATCH3:DATA
	DC8     0
        #endif

	RSEG	FSWI_PATCH2:DATA(2)

main_jumper:
	DC32	main

	RSEG	CODE:CODE
	CODE32
nullsub:
	BX	LR
main:
	LDR	R1,[SP,#0]		; Берем CPSR
	TST	R1, #0x20		; Проверяем на тумбу
	ADREQ	R2,arm_jumper		; Берем адрес джампера
	BEQ	arm_mode
	CMP	R0,#199			; Служебный SWI для длинной библиотеки
	LDREQH	R0,[LR],#2		; Зацепляем второе слово (длинный номер) c пропуском DW
        STREQ   LR,[SP,#0x14]           ; Сохраняем временно адрес возврата (если захочем данные)
	ADD	LR,LR,#1		; Устанавливаем бит 0, если тумба
	ADR	R2,thumb_jumper		; Берем адрес джампера
arm_mode
	LDR	R1,=pLIB_TOP		; Указатель на таблицу адресов
	LDR	R1,[R1]
	CMP	R1,#0
	LDREQ	R1,=Library
	BIC	R3,R0,#0x8000
        CMP	R3,#4096
        BHI	exit
	TST	R0,#0x8000		; А не адрес нам надо получить?
	STMEQFD	SP!,{LR}		; Копируем адрес возврата из LR_svc только если вызов функции
	LDMEQFD	SP!,{LR}^		; в LR_usr, он будет использован вызываемой функцией
	LDR	R12,[R1,R3,LSL#2]	; Берем адрес функции
	STRNE	R12,[SP,#4]		; пишем адрес в R0(стек)
	BNE	exit
	CMP	R12,#0xFFFFFFFF
	LDREQ	R2,=FUNC_ABORT
	STREQ	R0,[SP,#4]
	STR	R2,[SP,#0x14]		; Пишем адрес джампера для возврата в стеке (PC)
exit:
	LDMFD	SP!,{R0}
	MSR	SPSR_cf,R0
	LDMFD	SP!,{R0-R3,PC}^
arm_jumper:
	BX	R12
	CODE16
thumb_jumper:
	BX	R12
	CODE32
;
; Собственно библиотека
;
	RSEG    LIBR:DATA(2)
	PUBLIC	Library
Library:

	END     ;main
