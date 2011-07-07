        RSEG	CODE:CODE(2)

	PUBLIC	ExecuteIMB

ExecuteIMB:
	STR	LR,[SP, #-4]!
	MRS	R2,CPSR
        #ifdef NEWSGOLD
	SWI	4		; Переключаемся в системный режим
        #else
	SWI	0		; Переключаемся в системный режим
        #endif
	MRS	R1,CPSR		; Запрещаем прерывания
	ORR	R1,R1,#0xC0
	MSR	CPSR_c,R1
	NOP
	NOP
	NOP
;Выполняем необходимую требуху с кешем
clean_loop:
	MRC 	p15, 0, r15, c7, c10, 3 ; clean entire dcache using test and clean
	NOP
	NOP
	NOP
	BNE	clean_loop
	MOV	R0,#0
	MCR	p15, 0, r0, c7, c10, 4 ; drain write buffer
	NOP
	NOP
	NOP
	NOP
	MCR	p15, 0, r0, c7, c5, 0 ; invalidate icache
	NOP
	NOP
	NOP
	NOP
;Выходим
	MSR	CPSR_c,R2	; Восстанавливаем старый режим
	LDR	PC,[SP], #+0x4
	
	END


