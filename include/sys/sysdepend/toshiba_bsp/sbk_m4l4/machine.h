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
 *	machine.h
 *
 *	Machine type definition (SBK-M4L4)
 */

#ifndef _MTKBSP_SYS_SYSDEPEND_MACHINE_H_
#define _MTKBSP_SYS_SYSDEPEND_MACHINE_H_

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- SBK-M4L4 (CPU: TMPM4L4FYAUG) definition ----- */
#define MTKBSP_TOSHIBA_BSP		1	/* Target system   : TOSHIBA BSP */
#define MTKBSP_SBK_M4L4	                1	/* Target Board    : SBK-M4L4 */

#define MTKBSP_CPU_TXZ  		1	/* Target CPU type : TXZ family */
#define MTKBSP_CPU_TXZM4  		1	/* Target CPU type : TXZ M4 series */
#define MTKBSP_CPU_TMPM4L		1	/* Target CPU type : TMPM4L group */

#define MTKBSP_CPU_CORE_ARMV7M		1	/* Target CPU-Core type : ARMv7-M */
#define MTKBSP_CPU_CORE_ACM7		1	/* Target CPU-Core      : ARM Cortex-M7 */

#define KNL_SYSDEP_PATH	sysdepend/toshiba_bsp			/* Kernel sysdepend path */

#define TARGET_DIR	toshiba_bsp/sbk_m4l4    	/* Sysdepend-Directory name */
#define TARGET_GRP_DIR	toshiba_bsp 			/* Sysdepend-Group-Directory name */
#define	TARGET_CPU_DIR	tmpm4l				/* Sysdepend-CPU-Directory name */

/*
 **** CPU Core depended profile (ARMv7M)
 */
#include <sys/sysdepend/toshiba_bsp/cpu/core/armv7m/machine.h>

#endif /* _MTKBSP_SYS_SYSDEPEND_MACHINE_H_ */
