/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_TOSHIBA_BSP

/*
 *	devinit.c (TOSHIBA BSP)
 *	Device-Dependent Initialization
 */
#include <tk/tkernel.h>
#include <tk/device.h>
#include <kernel.h>

/* ------------------------------------------------------------------------ */

/*
 * Initialization before micro T-Kernel starts
 */

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
EXPORT ER knl_start_device( void )
{
	ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC
        bsp_ei2c_t      bsp_ei2c;
        TXZ_Result      result;
        
        bsp_ei2c.ei2c.init.clock.sck = 100000;
        bsp_ei2c.ei2c.init.cr0.nfsel = EI2CxCR0_NFSEL_DIG;
        bsp_ei2c.ei2c.init.cr0.dnf = EI2CxCR0_DNF_3PRSCK;
        
        result = bsp_ei2c_init(&bsp_ei2c);
        
        NVIC_EnableIRQ(INTI2C1_IRQn);
        bsp_ei2c.ei2c.p_instance->AIE = EI2CxIE_INTSPE;

	err = dev_init_hal_i2c(DEV_HAL_I2C1, &bsp_ei2c);
	if(err < E_OK) return err;
#endif

#if DEVCNF_USE_HAL_ADC
	bsp_adc_t	hadc1;
        
        bsp_adc_initialize(&hadc1);
        
        NVIC_EnableIRQ(INTADASGL_IRQn);
        
	err = dev_init_hal_adc(DEV_HAL_ADC1, &hadc1);
	if(err < E_OK) return err;
#endif	/* DEVCNF_USE_HAL_ADC */

	return err;
}

#if USE_SHUTDOWN
/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* USE_SHUTDOWN */
#endif /* MTKBSP_TOSHIBA_BSP */
