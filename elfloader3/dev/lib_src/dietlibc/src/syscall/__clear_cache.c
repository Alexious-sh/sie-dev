
#include <swilib.h>
#include <unistd.h>

__attribute__((naked))
void ExecuteIMB()
{
    asm(".arm   \n"
        "STR	LR,[SP, #-4]!\n"
        "MRS	R2,CPSR\n"

        "ldr     r0, =__sys_switab_addres \n"
        "ldr     r0, [r0, #0x1B5*4]       \n"
        "cmp     r0, #0    \n"
        "swieq   0x00      \n"
        "swine   0x04      \n"

        "MRS	R1,CPSR\n"		// Запрещаем прерывания
        "ORR	R1,R1,#0xC0\n"
        "MSR	CPSR_c,R1\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        // Выполняем необходимую требуху с кешем
    "clean_loop:\n"
        "MRC 	p15, 0, r15, c7, c10, 3\n" // clean entire dcache using test and clean
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "BNE	clean_loop\n"
        "MOV	R0,#0\n"
        "MCR	p15, 0, r0, c7, c10, 4\n" // drain write buffer
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "MCR	p15, 0, r0, c7, c5, 0\n" // invalidate icache
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        // Выходим
        "MSR	CPSR_c,R2\n"	// Восстанавливаем старый режим
        "LDR	PC,[SP], #+0x4\n");
}


void __clear_cache(void *begin, void *end)
{
    ExecuteIMB();
}

void libc_clear_cpu_cache()
{
    ExecuteIMB();
}






