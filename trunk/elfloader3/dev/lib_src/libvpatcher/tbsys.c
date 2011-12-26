

__attribute__((naked))
void EnableINT(int fi)
{
    asm( "MRS     R1, CPSR\n"
         "BIC     R1, R1, #0xC0\n"
         "ORR     R1, R1, R0\n"
         "AND     R2, R1, #0x1F\n"
         "CMP     R2, #0x10\n"
         "blne    eno        \n"

         "ldr     r0, =__sys_switab_addres \n"
         "ldr     r0, [r0, #0x1B5*4]       \n"
         "cmp     r0, #0    \n"
         "swieq   0x00      \n"
         "swine   0x04      \n"

"eno:\n"
         "MSR     CPSR_c, R1\n"
         "BX      LR\n");
}



__attribute__((naked))
int  DisableINT()
{
    asm("MRS     R0, CPSR\n"
        "ORR     R1, R0, #0xC0\n"
        "AND     R2, R0, #0x1F\n"
        "CMP     R2, #0x10\n"
        "blne    dno       \n"

        "push    {r0}      \n"
        "ldr     r0, =__sys_switab_addres \n"
        "ldr     r0, [r0, #0x1B5*4]       \n"
        "cmp     r0, #0    \n"
        "swieq   0x00      \n"
        "swine   0x04      \n"
        "pop     {r0}      \n"

"dno:\n"
        "MSR     CPSR_c, R1\n"
        "AND     R0, R0, #0xC0\n"
        "BX      LR\n");
}


__attribute__((naked))
void CleanTLB()
{

    asm( "STR	   LR, [SP, #-0x04]!\n"
         "MRS	   R2,  CPSR\n"

         "ldr     r0, =__sys_switab_addres \n"
         "ldr     r0, [r0, #0x1B5*4]       \n"
         "cmp     r0, #0    \n"
         "swieq   0x00      \n"
         "swine   0x04      \n"

         "CTLB_loop:\n"

         "MRC      p15, 0, PC, c7, c14, 3\n"
         "BNE      CTLB_loop\n"

         "MCR      p15, 0, R0, c7, c10, 4\n"
         "MCR      p15, 0, R0, c7, c5, 0\n"
         "MCR      p15, 0, R0, c8, c7, 0\n"


         "MRS      R1, CPSR\n"
         "ORR      R1, R1,#0xC0\n"
         "MSR      CPSR_c, R1\n"

         "MSR	   CPSR_c,R2\n"
         "LDR	   PC,[SP], #+0x04\n");
}


__attribute__((naked))
void CleanCache(void *vaddress)
{
    asm("STR	   LR, [SP, #-0x04]!\n"
        "MRS	   R2,  CPSR\n"

        "push    {r0}      \n"
        "ldr     r0, =__sys_switab_addres \n"
        "ldr     r0, [r0, #0x1B5*4]       \n"
        "cmp     r0, #0    \n"
        "swieq   0x00      \n"
        "swine   0x04      \n"
        "pop     {r0}      \n"

        "MOV      R1, #0x00\n"
        "MCR      p15, 0, R1, c8, c7, 0\n"

        "MCR      p15, 0, R0, c7, c10, 1\n"
        "MCR      p15, 0, R0, c7, c10, 4\n"
        "MCR      p15, 0, R0, c7, c5, 1\n"
        "MCR      p15, 0, R0, c7, c13, 1\n"

        "MCR      p15, 0, R0, c8, c7, 1\n"

        "MRS      R1, CPSR\n"
        "ORR      R1, R1,#0xC0\n"
        "MSR      CPSR_c, R1\n"

        "MSR	   CPSR_c,R2\n"
        "LDR	   PC,[SP], #+0x04\n");
}



__attribute__((naked))
int *GetTBaseAddr()
{

    asm("STR	   LR, [SP, #-0x04]!\n"
        "MRS	   R2,  CPSR\n"

        "ldr     r0, =__sys_switab_addres \n"
        "ldr     r0, [r0, #0x1B5*4]       \n"
        "cmp     r0, #0    \n"
        "swieq   0x00      \n"
        "swine   0x04      \n"

        "MRS      R1, CPSR\n"
        "ORR      R1, R1,#0xC0\n"
        "MSR      CPSR_c, R1\n"

        "MOV      R0, #0\n"
        "MRC      p15, 0, R0, c2, c0\n"

        "MSR	   CPSR_c,R2\n"
        "LDR	   PC,[SP], #+0x04\n");
}


__attribute__((naked))
void UnLockAccess()
{
    asm("STR	   LR, [SP, #-0x04]!\n"
        "MRS	   R2,  CPSR\n"

        "ldr     r0, =__sys_switab_addres \n"
        "ldr     r0, [r0, #0x1B5*4]       \n"
        "cmp     r0, #0    \n"
        "swieq   0x00      \n"
        "swine   0x04      \n"

        "MRS      R1, CPSR\n"
        "ORR      R1, R1,#0xC0\n"
        "MSR      CPSR_c, R1\n"

        "MOV      R0, #0xFFFFFFFF\n"
        "MCR      p15, 0, R0, c3, c0\n"

        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"

        "MSR	   CPSR_c,R2\n"
        "LDR	   PC,[SP], #+0x04\n");
}





