/*************************************************************************
*                                                                       
*               Copyright Mentor Graphics Corporation 2002              
*                         All Rights Reserved.                          
*                                                                       
* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  
* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS   
* SUBJECT TO LICENSE TERMS.                                             
*                                                                       
*************************************************************************/

/*************************************************************************
*                                                                      
* FILE NAME                                VERSION                        
*                                                                      
*      sd_defs.h                 Nucleus PLUS\ARM925\Code Composer 1.14.1 
*                                                                      
* COMPONENT                                                            
*                                                                      
*      SD - Serial Driver                                              
*                                                                      
* DESCRIPTION                                                          
*                                                                      
*      This file contains constant definitions and function macros     
*      for the Serial Driver module.                                   
*                                                                      
* DATA STRUCTURES                                                      
*                                                                      
*      SD_PORT     :   Structure to keep all needed info. about a port.
*                                                                      
* DEPENDENCIES                                                         
*                                                                      
*      none    
*
*                                                                      
*************************************************************************/
#ifndef SD_DEFS_H
#define SD_DEFS_H

/**************** User configurable section *************************/

/* The default system clock 48MHz */
#define UART_CLOCK            48000000

/* The base addresses for the seperate UART registers. */
#define SD_UART1_BASE      0xFFFB0000
#define SD_UART2_BASE      0xFFFB0800
                            
#define SD_UART1_VECTOR    46    
#define SD_UART2_VECTOR    47

/* Macros for specifying which UART to use. */
#define SD_UART1           1
#define SD_UART2           2

#define SD_MAX_UARTS        2

#define ICBASE              (0xFFFECB00)    /* Interrupt Controller Base */
#define IRQENABLE           ((volatile int *)(ICBASE + 0x18))
#define INT_MASK_OFFSET     0x04
#define INT_ITR_OFFSET      0x00

/* Defines for use by applications */
#define UART_INT_B          0x8000
#define UART_INT_A          0x4000
#define LEVEL_2_INT         0x0001

/* These use specific type names, putting the register name
   in the macro, because these macros are used by port specific
   sections of code and will most likely have different names
   on other UARTS. */
/*************************************/
/* Receive Holding Register - RHR (R)*/
/*************************************/
#define RHR_OFFSET          0x00

/**************************************/
/* Transmit Holding Register - THR (W)*/
/**************************************/
#define THR_OFFSET          0x00

/****************************************/
/* Interrupt Enable Register - IER (R/W)*/
/****************************************/
#define IER_OFFSET          0x04
#define IER_RX_HOLDING_REG  0x01          /* bit 0 - Recieve Holding Register Interrupt
                                             - Enabled When Set   */
#define IER_TX_HOLDING_REG  0x02          /* bit 1 - Transmit Holding Register Interrupt
                                             - Enabled When Set   */
#define IER_RX_LINE_STATUS  0x04          /* bit 2 - recieve Line Status Interrupt
                                             - Enabled When Set   */
#define IER_MODEM_STATUS    0x08          /* bit 3 - Modem Status Interrupt
                                             - Enabled When Set   */


/************************************/
/*  FIFO Control Register - FCR (W) */
/************************************/
#define FCR_OFFSET          0x08
#define FCR_FIFO_ENABLED    0x01          /* bit 0 - Enabled When Set */
#define FCR_RCVR_TRIG_LEVEL 0x00          /* bit 6:bit 7 - 8 Bytes Trigger Level */
#define FCR_FIFO_RESET      0x06          /* bit 1-2 - TX/RX FIFO Reset When Set */

/************************************************/
/* Interrupt Identification Register - IIR (IIR)*/
/************************************************/
#define IIR_OFFSET          0x08
#define IIR_TYPE_MASK       0x0000003E    /* bit 5:bit 1 */
#define IIR_PENDING         0x00000001    /* ISR pending bit - 0=pending, 1=not pending       */
#define IIR_RX_TIMEOUT      0x0000000C    /* 1 1 0 - RX time out                   Priority 2 */
#define IIR_RX_RDY          0x00000004    /* 0 1 0 - Received Data Ready           Priority 2 */
#define IIR_TX_RDY          0x00000002    /* 0 0 1 - Transmitter Holding Reg Empty Priority 3 */
#define IIR_RX_LINE_STAT    0x00000006    /* 0 1 1 - Receive Line Status             Priority 1 */

/**************************************/
/*  Latch Control Register - LCR (R/W)*/
/**************************************/
#define LCR_OFFSET          0x0C
#define LCR_5_BIT_WORD      0x00          /* 0 0  - 5 Bit Word   */
#define LCR_6_BIT_WORD      0x01          /* 0 1  - 6 Bit Word   */
#define LCR_7_BIT_WORD      0x02          /* 1 0  - 7 Bit Word   */
#define LCR_8_BIT_WORD      0x03          /* 1 1  - 8 Bit Word   */

#define LCR_STOP_BIT_1      0x00          /* 1 stop bit */
#define LCR_STOP_BIT_2      0x04          /* 2 stop bit */

#define LCR_PARITY_ENABLE   0x08          /* bit 3 - Enable Parity Bit Generation and Check
                                             - Enabled When Set */
#define LCR_PARITY_DISABLE  0x00          /* bit 3 - Enable Parity Bit Generation and Check
                                             - Enabled When Set */
#define LCR_PARITY_EVEN     0x10          /* bit 4 - Odd/Even Parity Generation and Check
                                             - Even When Set    */
#define LCR_PARITY_ODD      0x00          /* bit 4 - Odd/Even Parity Generation and Check
                                             - Odd When Set    */
#define LCR_BREAK_SET       0x40          /* bit 6 - Force Break Control ( Tx o/p low)
                                             - Forced When Set  */
#define LCR_NO_BREAK        0x00          /* bit 6 - Normal operating conditions */
#define LCR_DIV_EN          0x80          /* Enable access to DLL and DLH */

/*************************************/
/* Modem Control Register - MCR (R/W)*/
/*************************************/
#define MCR_OFFSET          0x10

#define MCR_DTR_LOW         0x01          /* bit 0 - Set DCD Signal Low/High - DCD Low when Set */
#define MCR_RTS_LOW         0x02          /* bit 1 - Set RTS Signal Low/High - RTS Low when Set */
#define MCR_NORMAL_MODE     0x00          /* bit 4 - normal operating mode */
#define MCR_LOOP_BACK       0x10          /* bit 4 - enable loopback mode */
#define MCR_TCR_TLR         0x40          /* bit 6 - enable access to TCR and TLR */
#define MCR_CLKSEL          0x80          /* bit 7 - enable clk/4 */
#define MCR_Not_Used        0x0C          /* bit 2,bit 3 - not used */


/* The Following Registers are Status Registers which Report conditions within the    */
/* UART/PPP during operation. The defined values are masks to ensure that the register*/
/* flags are correctly accessed */

/*********************************/
/* Line Status Register - LSR (R)*/
/*********************************/
#define LSR_OFFSET          0x14
#define LSR_RX_DATA_READY   0x01          /* bit 0 - Data Received and Saved in Holding Reg
                                             - Set when Valid */
#define LSR_OVERRUN_ERROR   0x02          /* bit 1 - Overrun Error Occured
                                             - Set When Valid */
#define LSR_PARITY_ERROR    0x04          /* bit 2 - Parity Error Occured
                                             - Set When Valid */
#define LSR_FRAMING_ERROR   0x08          /* bit 3 - Framing Error Occured
                                             - Set When Valid */
#define LSR_BREAK_ERROR     0x10          /* bit 4 - Break Error Occured
                                             - Set When Valid */
#define LSR_TX_HOLD_EMPTY   0x20          /* bit 5 - Tx Holding Register is empty and ready
                                             - Set When Valid */
#define LSR_TX_HOLD_FULL    0x00          /* bit 5 - Tx Holding Register is Full */

#define LSR_TX_EMPTY        0x40          /* bit 6 - 1= Tx Holding and shift registers are empty */
#define LSR_TX_FULL         0x00          /* bit 6 - 0= Tx Holding and shift registers are full */

#define LSR_FIFO_ERROR      0x80          /* bit 7 - At Least one of b4 - b2 has occurred
                                               - Set When Valid */

/**********************************/
/* Modem Status Register - MSR (R)*/
/**********************************/
#define MSR_OFFSET          0x18

/******************************************/
/* Supplementary Status Register - SSR (R)*/
/******************************************/
#define SSR_OFFSET          0x44          /* Supplementary Status Reg Offset */
#define SSR_TX_FIFO_FULL    0x01          /* bit 0 - Tx FIFO full - Set when full */

/***************************************/
/* Mode Definition Register - MDR (R/W)*/
/***************************************/
#define MDR_OFFSET          0x20
#define MDR_UART_MODE       0x00          /* bit 2:bit 0 - 0 0 0 - Uart Mode        */
#define MDR_AUTO_MODE       0x02          /* bit 2:bit 0 - 0 1 0 - AutoBaud Mode    */
#define MDR_RESET_MODE      0x07          /* bit 2:bit 0 - 1 1 1 - Reset Mode       */

/***********************************************/
/* Divisor for baud-rate generation - DLH (R/W)*/
/***********************************************/
#define DLH_OFFSET          0x04

/***********************************************/
/* Divisor for baud-rate generation - DLL (R/W)*/
/***********************************************/
#define DLL_OFFSET          0x00


#define OSC_12M_OFFSET      0x4C            /* Division factor offset */
#define OSC_12M_SEL         0x01            /* Selects 6.5 division factor. */

/***************************************/
/* Enable UART Clock and Peripherals   */
/***************************************/
#define EPLD_PWR_CNTRL      0x08000005      

#define EPLD_PWR_UART2      0x40            /* Turn-on power to UART 2 through the 
                                               EPLD (FPGA memory-mapped registers) */
#define EPLD_PWR_BT_MOD     0x10            /* Make sure BT Module is Off */

#define EPLD_PWR_UART1      0x20            /* Turn-on power to UART 1 through the 
                                               EPLD (FPGA memory-mapped registers) */


#define MPU_CLOCK_BASE      0xFFFECE00      /* Clock Base of MPU reg */

#define MPU_IDLECT2_OFFSET  0x08            /* Idle Mode Entry 2 Offset */
#define MPU_EN_XORPCK       0x0002          /* Enables clock of OS timer connected 
                                               to MPU TIPB */
#define MPU_EN_PERCK        0x0004          /* Enable peripheral clock */

#define MPU_RSTCT2_OFFSET   0x14            /* Reset Control 2 Offset */
#define MPU_RSTCT2_PER_EN   0x0001          /* Enable UART clock, make sure to release 
                                               peripheral reset */


#define ILR_BASE_2          0xFFFE0000      /* Base address of 2nd Interrupt Level Register(ILR)*/

#define ILR_CONTROL_OFFSET  0x18            /* Control offset of ILR */

/* Determine the clock source of UART1 and UART2 */
#define FUNC_MUX_CTRL_0     0xFFFE1000      /* Base configuration register */
#define MOD_CONF_CTRL_0     0x80            /* Module Configuration Control 0 Offset */
#define UART_CLOCK_SET      0x60000000      /* Determin the clock source of UART1 and UART2 */

/* These use generic type names, leaving off the register name
   in the macro, because they are used by generic sections of
   code which will not require changes for other UARTS. Only the
   bits these correspond to should change. */

/* UART Line Control Register Bits */
#define SD_PARITY_NONE              LCR_PARITY_DISABLE
#define SD_PARITY_EVEN              LCR_PARITY_EVEN
#define SD_PARITY_ODD               LCR_PARITY_ODD

#define SD_DATA_BITS_5              LCR_5_BIT_WORD
#define SD_DATA_BITS_6              LCR_6_BIT_WORD
#define SD_DATA_BITS_7              LCR_7_BIT_WORD
#define SD_DATA_BITS_8              LCR_8_BIT_WORD

#define SD_STOP_BITS_1              LCR_STOP_BIT_1
#define SD_STOP_BITS_2              LCR_STOP_BIT_2

#define SD_MODE_NORMAL              MCR_NORMAL_MODE
#define SD_MODE_AUTO_ECHO           MCR_NOT_USED
#define SD_MODE_LOCAL_LOOP          MCR_LOOP_BACK
#define SD_MODE_REMOTE_LOOP         MCR_NOT_USED

/* Define default Serial Driver settings for this board */
#define     DEFAULT_UART_PORT       UART1
#define     DEFAULT_PPP_BAUD        57600
#define     DEFAULT_UART_BAUD       115200
#define     DEFAULT_UART_DATA       DATA_BITS_8
#define     DEFAULT_UART_STOP       STOP_BITS_1
#define     DEFAULT_UART_PARITY     PARITY_NONE
#define     DEFAULT_UART_MODE       MODE_NORMAL
#define     DEFAULT_UART_BUFFER     100

/* Define data structures for management of a serial port. */

typedef struct SD_INIT_STRUCT
{
    UINT32        data_mode;
    UINT32        base_address;

    /* The following elements should generic accross other
       platforms. */
    NU_SEMAPHORE    *sd_semaphore;
    UINT32        com_port;
    UINT32        data_bits;
    UINT32        stop_bits;
    UINT32        parity;
    UINT32        baud_rate;
    UINT32        vector;
    UINT32        driver_options;
    UINT32        sd_buffer_size;

    UINT32        parity_errors;
    UINT32        frame_errors;
    UINT32        overrun_errors;
    UINT32        busy_errors;
    UINT32        general_errors;

    CHAR            *rx_buffer;
    INT             rx_buffer_read;
    INT             rx_buffer_write;
    volatile INT    rx_buffer_status;

    /* All of the following elements are required by PPP, do not modify. */
    UINT32        communication_mode;
    CHAR            *tx_buffer;
    INT             tx_buffer_read;
    INT             tx_buffer_write;
    volatile INT    tx_buffer_status;

} SD_PORT;

/* Defines to be used by application */
#define MODE_NORMAL       SD_MODE_NORMAL
#define MODE_AUTO_ECHO    SD_MODE_AUTO_ECHO
#define MODE_LOCAL_LOOP   SD_MODE_LOCAL_LOOP
#define MODE_REMOTE_LOOP  SD_MODE_REMOTE_LOOP

#define STOP_BITS_1       SD_STOP_BITS_1
#define STOP_BITS_2       SD_STOP_BITS_2

#define UART1             SD_UART1
#define UART2             SD_UART2

/* Defines to determine communication mode */
#define SERIAL_MODE                 0
#define SERIAL_MOUSE                3
/* MDM_NETWORK and MDM_TERMINAL do not need to be defined here
   since they are defined in PPP.  */

/***********************************************************************
 Note: everything below should be genric.
*/

#define NU_SERIAL_PORT          SD_PORT
#define PARITY_NONE             SD_PARITY_NONE
#define PARITY_EVEN             SD_PARITY_EVEN
#define PARITY_ODD              SD_PARITY_ODD

#define DATA_BITS_6             SD_DATA_BITS_6
#define DATA_BITS_7             SD_DATA_BITS_7
#define DATA_BITS_8             SD_DATA_BITS_8

#define NU_SD_Put_Char          SDC_Put_Char
#define NU_SD_Get_Char          SDC_Get_Char
#define NU_SD_Put_String        SDC_Put_String
#define NU_SD_Init_Port         SDC_Init_Port
#define NU_SD_Data_Ready        SDC_Data_Ready

#define NU_UART_SUCCESS         0
#define NU_INVALID_PARITY       -1
#define NU_INVALID_DATA_BITS    -2
#define NU_INVALID_STOP_BITS    -3
#define NU_INVALID_BAUD         -4
#define NU_INVALID_COM_PORT     -5
#define NU_INVALID_DATA_MODE    -6
#define NU_UART_LIST_FULL       -7
#define NU_INVALID_MOUSE_MODE   -8

#define NU_BUFFER_FULL          1
#define NU_BUFFER_DATA          2
#define NU_BUFFER_EMPTY         3

/* Deifine IO macros. */

/* 8 bit access */
#define SD_OUTBYTE(reg, data)   ( (*( (UINT8 *) (reg) ) ) = (UINT8) (data) )

#define SD_INBYTE(reg)          (  *( (UINT8 *) (reg) ) )

/* 16 bit access */
#define SD_OUTWORD(reg, data)   ( (*( (UINT16 *) (reg) ) ) = (data) )

#define SD_INWORD(reg)          (  *( (UINT16 *) (reg) ) )

/* 32 bit access */
#define SD_OUTDWORD(reg, data)  ( (*( (UINT32 *) (reg) ) ) = (data) )

#define SD_INDWORD(reg)         (  *( (UINT32 *) (reg) ) )

/*Macro used for converting URT to SD_PORT. This is for PPP serial driver
  backwards compatability. */
#define URT_LAYER                       SD_PORT

#define URT_TX_BUFFER_SIZE              uart->sd_buffer_size
#define URT_Get_Char                    SDC_Get_Char
#define URT_Put_Char                    SDC_Put_Char
#define URT_Reset                       SDC_Reset
#define URT_Change_Communication_Mode   SDC_Change_Communication_Mode
#define URT_Carrier                     SDC_Carrier


#endif /* ifndef SD_DEFS_H */





