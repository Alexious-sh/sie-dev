
#if !defined(_PMB8876_UART_H_)
#define _PMB8876_UART_H_

/*
 * UART defs taken from the XC164-16 peripheral controller
 * data sheet.  Hopefully it's the same IP block in the PMB8876
 */

#define USART0_BASE		0xF1000000
#define USART1_BASE		0xF1800000

#define USART0_CLC		USART0_BASE		/* Clock Control Register */
#define USART0_ID		USART0_BASE + 0x08	/* Module Identification Register */
#define USART0_CON		USART0_BASE + 0x10	/* Control Register */
#define USART0_BG		USART0_BASE + 0x14	/* Baudrate Timer Reload Register */
#define USART0_FDV		USART0_BASE + 0x18	/* Fractional Divider Register */
#define USART0_TXB		USART0_BASE + 0x20	/* Transmit Buffer */
#define USART0_RXB		USART0_BASE + 0x24	/* Receive Buffer */
#define USART0_ABCON		USART0_BASE + 0x30	/* Autobaud control register */
#define USART0_ABSTAT		USART0_BASE + 0x34	/* Autobaud status register */
#define USART0_RXFCON		USART0_BASE + 0x40	/* Receive FIFO control register */
#define USART0_TXFCON		USART0_BASE + 0x44	/* Transmit FIFO control register */
#define USART0_FSTAT		USART0_BASE + 0x48	/* FIFO status register */
#define USART0_WHBCON		USART0_BASE + 0x50	/* Write hardware modified control register */
#define USART0_WHBABCON		USART0_BASE + 0x54	/* Write hardware modified autobaud control register */
#define USART0_WHBABSTAT	USART0_BASE + 0x58	/* Write hardware modified autobaud status register */
#define USART0_FCCON		USART0_BASE + 0x5C	/* Flowcontrol control register */
#define USART0_FCSTAT		USART0_BASE + 0x60	/* Flowcontrol status register */
#define USART0_IMSC		USART0_BASE + 0x64	/* Interrupt mask control register */
#define USART0_RIS		USART0_BASE + 0x68	/* Raw interrupt status register */
#define USART0_MIS		USART0_BASE + 0x6C	/* Masked interrupt status register */
#define	USART0_ICR		USART0_BASE + 0x70	/* Interrupt clear register */
#define USART0_ISR		USART0_BASE + 0x74	/* Interrupt set register */
#define USART0_TMO		USART0_BASE + 0x7C	/* Timeout detection control register */

#define USART1_CLC		USART1_BASE
#define USART1_PISEL		USART1_BASE + 0x04
#define USART1_ID		USART1_BASE + 0x08
#define USART1_CON		USART1_BASE + 0x10
#define USART1_BG		USART1_BASE + 0x14
#define USART1_FDV		USART1_BASE + 0x18
#define USART1_PMW		USART1_BASE + 0x1C
#define USART1_TXB		USART1_BASE + 0x20
#define USART1_RXB		USART1_BASE + 0x24
#define USART1_ABCON		USART1_BASE + 0x30
#define USART1_ABSTAT		USART1_BASE + 0x34
#define USART1_RXFCON		USART1_BASE + 0x40
#define USART1_TXFCON		USART1_BASE + 0x44
#define USART1_FSTAT		USART1_BASE + 0x48
#define USART1_WHBCON		USART1_BASE + 0x50
#define USART1_WHBABCON		USART1_BASE + 0x54
#define USART1_WHBABSTAT	USART1_BASE + 0x58
#define USART1_FCCON		USART1_BASE + 0x5C
#define USART1_FCSTAT		USART1_BASE + 0x60
#define USART1_IMSC		USART1_BASE + 0x64
#define USART1_RIS		USART1_BASE + 0x68
#define USART1_MIS		USART1_BASE + 0x6C
#define	USART1_ICR		USART1_BASE + 0x70
#define USART1_ISR		USART1_BASE + 0x74
#define USART1_TMO		USART1_BASE + 0x7C

#define USART_57600		0x001901D8	/* 57600 / 0x1D8 = 122.033898 * 0x1a = 3172.881348 */
#define USART_115200		0x000C01D8	/* 115200 / 0x1D8 = 244.067797 * 13 = 3172.881361 */
#define USART_230400		0x000501B4	/* 230400 / 0x1B4 = 528.440367 * 0x6 = 3170.642202 */
#define USART_460800		0x00000092	/* 460800 / 0x92 = 3156.164384 */
#define USART_614400		0x000000C3	/* 614400 / 0xC3 = 3150.769231 */
#define USART_921600		0x00000127	/* 921600 / 0x127 = 3124.067797 */
#define USART_1228800		0x0000018A	/* 1228800 / 0x18A = 3118.781726 */
#define USART_1600000		0x00000000	/* 1600000 / 0x200 = 3125 */
#define USART_1500000		0x000001D0	/* 1500000 / 0x1D0 = 3232.758621 */

#define CLC_SMC_CLK_DIV(x)	((x << 16) & 0xFF0000)
#define CLC_RMC_CLK_DIV(x)	((x <<  8) & 0x00FF00)
#define CLC_FSOE		(1 << 5)	/* Fast shut off enable (1: enable; 0: disable) */
#define CLC_SBWE		(1 << 4)	/* Suspend bit write enable (1: enable; 0: disable) */
#define CLC_EDIS		(1 << 3)	/* External request disable (1: disable; 0: enable) */
#define CLC_SPEN		(1 << 2)	/* Suspend bit enable (1: enable; 0: disable) */
#define CLC_DISS		(1 << 1)	/* Disable status bit (1: disable; 0: enable) */
#define CLC_DISR		(1 << 0)	/* Disable request bit (1: enable; 0: disable) */

#define	CON_R			(1 << 15)	/* Baud rate generator run control (0: disable; 1: enable) */
#define CON_LB			(1 << 14)	/* Loopback mode (0: disable; 1: enable) */
#define CON_BRS			(1 << 13)	/* Baudrate selection (0: Pre-scaler /2; 1: Pre-scaler / 3) */
#define CON_ODD			(1 << 12)	/* Parity selection (0: even; 1: odd)  */
#define	CON_FDE			(1 << 11)	/* Fraction divider enable (0: disable; 1: enable) */
#define CON_OE			(1 << 10)	/* Overrun error flag */
#define CON_FE			(1 <<  9)	/* Framing error flag */
#define CON_PE			(1 <<  8)	/* Parity error flag */
#define CON_OEN			(1 <<  7)	/* Overrun check enable (0: ignore; 1: check) */
#define CON_FEN			(1 <<  6)	/* Framing error check (0: ignore; 1: check) */
#define CON_PEN			(1 <<  5)	/* Parity check enable (0: ignore; 1: check) */
#define CON_REN			(1 <<  4)	/* Receiver bit enable (0: disable; 1: enable) */
#define CON_STP			(1 <<  3)	/* Number of stop bits (0: 1 stop bit; 1: two stop bits) */
#define CON_MODE_MASK		(7)		/* Mask for mode control */

#define WHBCON_SETOE		(1 << 13)	/* Set overrun error flag */
#define WHBCON_SETFE		(1 << 12)	/* Set framing error flag */
#define WHBCON_SETPE		(1 << 11)	/* Set parity error flag */
#define WHBCON_CLROE		(1 << 10)	/* Clear overrun error flag */
#define WHBCON_CLRFE		(1 <<  9)	/* Clear framing error flag */
#define WHBCON_CLRPE		(1 <<  8)	/* Clear parity error flag */
#define WHBCON_SETREN		(1 <<  5)	/* Set receiver enable bit */
#define WHBCON_CLRREN		(1 <<  4)	/* Clear receiver enable bit */

#define RX_DMA_ENABLE		(1 <<  1)	/* Receive DMA enable (0: disable, 1: enable) */
#define TX_DMA_ENABLE		(1 <<  0)	/* Transmit DMA enable (0: disable, 1: enable) */

#define ISR_TMO			(1 <<  7)	/* RX timeout interrupt mask */
#define ISR_CTS			(1 <<  6)	/* CTS interrupt mask */
#define ISR_ABDET		(1 <<  5)	/* Autobaud detected interrupt mask */
#define ISR_ABSTART		(1 <<  4)	/* Autobaud start interrupt mask */
#define ISR_ERR			(1 <<  3)	/* Error interrupt mask */
#define ISR_RX			(1 <<  2)	/* Receive interrupt mask */
#define ISR_TB			(1 <<  1)	/* Transmit buffer interrupt mask */
#define ISR_TX			(1 <<  0)	/* Transmit interrupt mask */

#define ICR_TMO			(1 <<  7)	/* RX timeout interrupt mask */
#define ICR_CTS			(1 <<  6)	/* CTS interrupt mask */
#define ICR_ABDET		(1 <<  5)	/* Autobaud detected interrupt mask */
#define ICR_ABSTART		(1 <<  4)	/* Autobaud start interrupt mask */
#define ICR_ERR			(1 <<  3)	/* Error interrupt mask */
#define ICR_RX			(1 <<  2)	/* Receive interrupt mask */
#define ICR_TB			(1 <<  1)	/* Transmit buffer interrupt mask */
#define ICR_TX			(1 <<  0)	/* Transmit interrupt mask */

#define FCSTAT_RTS		(1 <<  1)	/* RTS Status (0: inactive; 1: active) */
#define FCSTAT_CTS		(1 <<  0)	/* CTS Status (0: inactive; 1: active) */
#define FCCON_RTS_TRIGGER(x)	((x << 8) & 0x3F00) /* RTS receive FIFO trigger level */
#define FCCON_RTS		(1 <<  4)	/* RTS control bit */
#define FCCON_CTSEN		(1 <<  1)	/* CTS enable (0: disable; 1: enable) */
#define FCCON_RTSEN		(1 <<  0)	/* RTS enbled (0: disable; 1: enable) */

#define ABCON_RXINV		(1 << 11)	/* Receive invert enable (0: disable; 1: enable) */
#define ABCON_TXINV		(1 << 10)	/* Transmit invert enable (0: disable; 1: enable) */
#define ABCON_ABEM_ECHO_DET	(1 <<  8)	/* Autobaud echo mode enabled during detection */
#define ABCON_ABEM_ECHO_ALWAYS	(1 <<  9)	/* Autobaud echo mode always enabled */
#define ABCON_FCDETEN		(1 <<  4)	/* Fir char of two byte frame detect */
#define ABCON_ABDETEN		(1 <<  3)	/* Autobaud detection interrupt enable (0: dis; 1: en) */
#define ABCON_ABSTEN		(1 <<  2)	/* Start of autobaud detect interrupt (0: dis; 1: en) */
#define ABCON_AUREN		(1 <<  1)	/* Auto control of CON.REN (too complex for here) */
#define ABCON_ABEN		(1 <<  0)	/* Autobaud detection enable */

#define ABSTAT_DETWAIT		(1 <<  4)	/* Autobaud detect is waiting */
#define ABSTAT_SCCDET		(1 <<  3)	/* Second character with capital letter detected */
#define ABSTAT_SCSDET		(1 <<  2)	/* Second character with small letter detected */
#define ABSTAT_FCCDET		(1 <<  1)	/* First character with capital letter detected */
#define ABSTAT_FCSDET		(1 <<  0)	/* First character with small letter detected */

#define RXFCON_RXFITL(x)	((x & 8) <<  8)	/* Receive FIFO interrupt trigger level */
#define RXFCON_RXTMEN		(1 <<  2)	/* Receive FIFO transparent mode enable */
#define RXFCON_RXFFLU		(1 <<  1)	/* Receive FIFO flush */
#define RXFCON_RXFEN		(1 <<  0)	/* Receive FIFO enable */

#define TXFCON_TXFITL		((x & 8) <<  8)	/* Transmit FIFO interrupt trigger level */
#define TXFCON_TXTMEN		(1 <<  2)	/* Transmit FIFO transparent mode enable */
#define TXFCON_TXFFLU		(1 <<  1)	/* Transmit FIFO flush */
#define TXFCON_TXFEN		(1 <<  0)	/* Transmit FIFO enable */

#define FSTAT_TXFFL		(0xF <<  8)	/* Transmit FIFO filling level mask */
#define FSTAT_RXFFL		(0xF)		/* Receive FIFO filling level mask */

void uart_set_speed(unsigned int uart, unsigned int speed);
unsigned char uart_poll_rx_byte(unsigned int uart);
void uart_poll_tx_byte(unsigned int uart, unsigned char byte);
void uart_poll_tx_string(unsigned int uart, const char *msg);

#endif	/* !defined(_PMB8876_UART_H_) */
