

    .section .text
    .arm
    .global dump_registers
dump_registers:

    push    {r12}
    ldr     r12, =registers

    str     r0, [r12, #0]
    str     r1, [r12, #4];
    str     r2, [r12, #8];
    str     r3, [r12, #12];
    @str     r4, [r12, #16];
    @str     r5, [r12, #20];
    @str     r6, [r12, #24];
    @str     r7, [r12, #28];
    @str     r8, [r12, #32];
    @str     r9, [r12, #36];
    @str     r10, [r12, #40];
    @str     r11, [r12, #44];
    @str     r12, [r12, #48]; @ FIXME
    @str     r13, [r12, #52];
    @str     r14, [r12, #56];
    @str     r15, [r12, #60];
    pop     {r12}
    bx      lr
