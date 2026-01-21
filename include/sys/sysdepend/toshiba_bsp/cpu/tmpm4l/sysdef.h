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
 *	sysdef.h
 *
 *	System dependencies definition (TMPM4L depended)
 *	Included also from assembler program.
 */

#ifndef _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_
#define _MTKBSP_SYS_SYSDEF_DEPEND_CPU_H_

#include <sys/machine.h>

/* CPU Core-dependent definition */
#include <sys/sysdepend/toshiba_bsp/cpu/core/armv7m/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */
/* TMPM4L SRAM RAM   0x2000_0000 - 0x2000_7FFF  (Size 32KB) */
#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       0x00008000

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 */

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define	N_SYSVEC		16	/* Number of System Exceptions */
#define N_INTVEC		106	/* Number of Interrupt vectors */

/*
 * Exception vector table alignment
*/
#define	EXCTBL_ALIGN		1024

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		4

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		15	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		1
#define CPU_HAS_DSP		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif

/*
 * RCC (Reset & Clock control) registers (Used from physical timer)
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	MTK_PORTA_BASE	0x40080000
#define	MTK_PORTB_BASE	0x40080100
#define	MTK_PORTC_BASE	0x40080200
#define	MTK_PORTD_BASE	0x40080300
#define	MTK_PORTE_BASE	0x40080400
#define	MTK_PORTF_BASE	0x40080500
#define	MTK_PORTG_BASE	0x40080600
#define	MTK_PORTH_BASE	0x40080700
#define	MTK_PORTJ_BASE	0x40080800

#define PORT_DATA(n)	(MTK_PORT##n##_BASE + 0x00)		/* GPIO port data register */
#define PORT_CR(n)	(MTK_PORT##n##_BASE + 0x04)		/* GPIO port output control register */
#define PORT_FR1(n)	(MTK_PORT##n##_BASE + 0x08)		/* GPIO port function register 1 */
#define PORT_FR2(n)	(MTK_PORT##n##_BASE + 0x0C)		/* GPIO port function register 2 */
#define PORT_FR3(n)	(MTK_PORT##n##_BASE + 0x10)		/* GPIO port function register 3 */
#define PORT_FR4(n)	(MTK_PORT##n##_BASE + 0x14)		/* GPIO port function register 4 */
#define PORT_FR5(n)	(MTK_PORT##n##_BASE + 0x18)		/* GPIO port function register 5 */
#define PORT_FR6(n)	(MTK_PORT##n##_BASE + 0x1C)		/* GPIO port function register 6 */
#define PORT_FR7(n)	(MTK_PORT##n##_BASE + 0x20)		/* GPIO port function register 7 */
#define PORT_FR8(n)	(MTK_PORT##n##_BASE + 0x24)		/* GPIO port function register 8 */
#define PORT_OD(n)	(MTK_PORT##n##_BASE + 0x28)		/* GPIO port open drain control register */
#define PORT_PUP(n)	(MTK_PORT##n##_BASE + 0x2C)		/* GPIO port pullup control register */
#define PORT_PDN(n)	(MTK_PORT##n##_BASE + 0x30)		/* GPIO port pulldown control register */
#define PORT_IE(n)	(MTK_PORT##n##_BASE + 0x38)		/* GPIO port input control register */


/* ------------------------------------------------------------------------ */
/* Clock frequency
 */
#define ICLK_MHz	(80)
#define PCLKA_MHz	(48)
#define PCLKB_MHz	(24)
#define PCLKC_MHz	(48)
#define PCLKD_MHz	(48)
#define	FCLK_MHz	(24)

#define	SYSCLK		(ICLK_MHz*1000000)	// System clock (Hz)
#define TMCLK_KHz	(ICLK_MHz*1000)		// System timer clock input (kHz)
#define TMCLK		(ICLK_MHz)		// System timer clock input (MHz)

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
