/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2025 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2025/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (SBK-M4L4)
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include <mtkernel/lib/libtm/libtm.h>

#ifdef MTKBSP_SBK_M4L4
#if TM_COM_SERIAL_DEV

/* UART register definition (Use USART0) */
#define	UART_BASE	(0x4006E000UL)		/* USART0 TYPE3 register base address */

#define UART_SWRST	(*(_UW*)(UART_BASE+0x0000))	/* Software reset register */
#define UART_CR0	(*(_UW*)(UART_BASE+0x0004))	/* Control register.0 */
#define UART_CR1	(*(_UW*)(UART_BASE+0x0008))	/* Control register.1 */
#define UART_CLK	(*(_UW*)(UART_BASE+0x000C))	/* Clock control register */
#define UART_BRD	(*(_UW*)(UART_BASE+0x0010))	/* Baud Rate register */
#define UART_TRANS	(*(_UW*)(UART_BASE+0x0014))	/* Transmit enable register */
#define UART_DR 	(*(_UW*)(UART_BASE+0x0018))	/* Data register */
#define UART_SR 	(*(_UW*)(UART_BASE+0x001C))	/* Status register */
#define UART_FIFOCLR 	(*(_UW*)(UART_BASE+0x0020))	/* FIFO clear register */
#define UART_ERR 	(*(_UW*)(UART_BASE+0x0024))	/* Error register */

#define CR0_SM  	(0x00000001)			/* Data length selection:8bit */
#define CR0_PE  	(0x00000000)			/* Parity enable:none */
#define CR0_SBLEN  	(0x00000000)			/* Stop bit length:1bit */

#define CR1_INTTXWE  	(0x00000040)			/* Transmission complete interrupt controler:enable */
#define CR1_INTRXWE  	(0x00000010)			/* Read complete interrupt controler:enable */
#define CR1_INTERR	(0x00000004)			/* Error interrupt controler:enable */

#define BRD_KEN  	(0x00800000)			/* Frequency division controler:enable */
#define BRD_BRK  	(0x00270000)			/* Baud rate setting K */
#define BRD_BRN  	(0x0000002B)			/* Baud rate setting N */

#define TRANS_TXE  	(0x00000002)			/* Enable sending */
#define TRANS_RXE  	(0x00000001)			/* Enable reception */

#define SR_TXRUN  	(0x00008000)			/* Transmit data register empty */
#define SR_TXEND  	(0x00004000)			/* Transmission complete */
#define SR_RXRUN  	(0x00000080)			/* Read data register empty */
#define SR_RXEND  	(0x00000040)			/* Read complete */


/* Communication speed */
#define UART_BAUD	(115200)			/* 115200 bps */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while ((UART_SR & SR_TXRUN) == 1 );
                UART_SR = SR_TXEND; /* Transmission complete flg clear */
		UART_DR = *b;
		while ((UART_SR & SR_TXEND) == 0 );
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while ((UART_SR & SR_RXRUN) == 1 );
                UART_SR = SR_RXEND; /* Read complete flg clear */
		*buf = UART_DR & 0xff;
		while ((UART_SR & SR_RXEND) == 0 );
	}
}

EXPORT	void	tm_com_init(void)
{
	/* Initialize serial communication. Disable all interrupt. */
        UART_CR0 = CR0_SM | CR0_PE | CR0_SBLEN;/* 8bit, Non parity, Stop bit 1 */
        UART_CR1 = CR1_INTTXWE | CR1_INTRXWE | CR1_INTERR;/* Trans read error interrupt setting */
	UART_BRD = BRD_KEN | BRD_BRK | BRD_BRN;	/* Set baud rate:115232bps  */

	UART_TRANS = TRANS_TXE | TRANS_RXE;	/* Start UART */
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* MTKBSP_SBK_M4L4 */
#endif /* USE_TMONITOR */	