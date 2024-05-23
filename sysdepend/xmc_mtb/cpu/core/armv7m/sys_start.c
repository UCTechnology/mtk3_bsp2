/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/05.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#if defined(MTKBSP_MODUSTOOLBOX) && defined(MTKBSP_CPU_CORE_ARMV7M)

/*
 *	sys_start.c (ModusToolbox & ARMv7-M)
 *	Kernel start routine 
 */


#include <stdint.h>
#include <tk/tkernel.h>
#include <kernel.h>
#include <sysdepend/xmc_mtb/halif.h>
#include "sysdepend.h"
#include <cy_syslib.h>


/* Exception handler table (RAM) */
EXPORT UW knl_exctbl[sizeof(UW)*(N_SYSVEC + CORE_EXT_INTVEC + CORE_SOFT_INTVEC)]
	__attribute__((section(".mtk_exctbl"))) __attribute__ ((aligned(256)));

EXPORT UW knl_sysclk;		// System clock frequency
EXPORT UW *knl_exctbl_o;	// Exception handler table (Origin)

EXPORT void		*knl_lowmem_top;	// Head of area (Low address)
EXPORT void		*knl_lowmem_limit;	// End of area (High address)
IMPORT const void	*end;

#if USE_STATIC_SYS_MEM
EXPORT UW knl_system_mem[SYSTEM_MEM_SIZE/sizeof(UW)] __attribute__((section(".mtk_sysmem")));
#endif

#if USE_DEBUG_SYSMEMINFO
EXPORT void		*knl_sysmem_top	= 0;
EXPORT void		*knl_sysmem_end	= 0;
#endif

EXPORT void knl_start_mtkernel(void)
{
	UW	*src, *top;
	UW	reg;
	INT	i;

	disint();		// Disable Interrupt (with BASEPRI)
	__enable_irq();		// Enable Interrupt (with CPSR.I)
	
	/* 
	 * The following code are copied from 
	 * mtb_shared/cmsis/release-v5.8.0/Core/Include/cmsis_gcc.h
	 */
	typedef struct {
		uint32_t const* src;
		uint32_t* dest;
		uint32_t  wlen;
	} __copy_table_t;

	typedef struct {
		uint32_t* dest;
		uint32_t  wlen;
	} __zero_table_t;

	extern const __copy_table_t __copy_table_start__;
	extern const __copy_table_t __copy_table_end__;
	extern const __zero_table_t __zero_table_start__;
	extern const __zero_table_t __zero_table_end__;

	for (__copy_table_t const* pTable = &__copy_table_start__; pTable < &__copy_table_end__; ++pTable) {
		for(uint32_t i=0u; i<pTable->wlen; ++i) {
			pTable->dest[i] = pTable->src[i];
		}
	}

	for (__zero_table_t const* pTable = &__zero_table_start__; pTable < &__zero_table_end__; ++pTable) {
		for(uint32_t i=0u; i<pTable->wlen; ++i) {
			pTable->dest[i] = 0u;
		}
	}
	
	knl_startup_hw();

	/* Copy exception handler (ROM -> RAM) */
	src = knl_exctbl_o = (UW*)in_w(SCB_VTOR);
	top = (UW*)knl_exctbl;
	for(i=0; i < (N_SYSVEC + CORE_EXT_INTVEC + CORE_SOFT_INTVEC); i++) {
		*top++ = *src++;
	}
	out_w(SCB_VTOR, (UW)knl_exctbl);

	/* Configure exception priorities */
	reg = *(_UW*)SCB_AIRCR;
	reg = (reg & (~AIRCR_PRIGROUP3)) | AIRCR_PRIGROUP0;	// PRIGRP:SUBPRI = 4 : 4
	*(_UW*)SCB_AIRCR = (reg & 0x0000FFFF) | AIRCR_VECTKEY;

	out_w(SCB_SHPR2, SCB_SHPR2_VAL);			// SVC pri = 0
	out_w(SCB_SHPR3, SCB_SHPR3_VAL);			// SysTick = 1 , PendSV = 7

	knl_sysclk	= halif_get_sysclk();			// Get System clock frequency

#if USE_IMALLOC
#if USE_STATIC_SYS_MEM
	knl_lowmem_top = knl_system_mem;
	knl_lowmem_limit = &knl_system_mem[SYSTEM_MEM_SIZE/sizeof(UW)];
#else
	/* Set System memory area */
	if(INTERNAL_RAM_START > SYSTEMAREA_TOP) {
		knl_lowmem_top = (UW*)INTERNAL_RAM_START;
	} else {
		knl_lowmem_top = (UW*)SYSTEMAREA_TOP;
	}
	if((UW)knl_lowmem_top < (UW)&end) {
		knl_lowmem_top = (UW*)&end;
	}

	if((SYSTEMAREA_END != 0) && (INTERNAL_RAM_END > CNF_SYSTEMAREA_END)) {
		knl_lowmem_limit = (UW*)(SYSTEMAREA_END - EXC_STACK_SIZE);
	} else {
		knl_lowmem_limit = (UW*)(INTERNAL_RAM_END - EXC_STACK_SIZE);
	}
#endif

#if USE_DEBUG_SYSMEMINFO
	knl_sysmem_top	= knl_lowmem_top;
	knl_sysmem_end	= knl_lowmem_limit;
#endif	// USE_DEBUG_MEMINFO
#endif	// USE_IMALLOC

	/* Startup Kernel */
	knl_main();		// *** No return ****/
	while(1);		// guard - infinite loops
}

#endif	/* defined(MTKBSP_MODUSTOOLBOX) && defined(MTKBSP_CPU_CORE_ARMV7M) */