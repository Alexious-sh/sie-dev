

  .global Tx_byte
Tx_byte:

@ void tx_byte(r0=byte)
@ uses r0, r1, r2

        mov r2, #0xf1000000 @USART0_CLC
        ldr r1, [r2, #0x20] @USART0_TXB
        bic r1, r1, #0xff
        orr r1, r0, r1
        str r1, [r2, #0x20] @USART0_TXB
tx_w:
        ldr r1, [r2, #0x68] @USART0_FCSTAT
        ands r1, r1, #0x02
        beq tx_w

        ldr r1, [r2, #0x70] @USART0_ICR
        orr r1, r1, #2
        str r1, [r2, #0x70]

        @b serve_watchdog
        bx lr

