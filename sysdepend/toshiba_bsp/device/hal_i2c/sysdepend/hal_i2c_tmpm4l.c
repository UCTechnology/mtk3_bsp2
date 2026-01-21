/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <config_bsp/toshiba_bsp/config_bsp.h>

#ifdef MTKBSP_TOSHIBA_BSP
#if DEVCNF_USE_HAL_IIC

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/toshiba_bsp/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "../hal_i2c_cnf.h"


/*----------------------------------*/
/** 
 * @enum   Processing
 * @brief  Processing mode.
*/
/*----------------------------------*/
typedef enum
{
    PROC_IDLE,              /*!< Idle.                              */
    PROC_MASTER_START,      /*!< Master start.                      */
    PROC_MASTER_STOP,       /*!< Master stop.                       */
    PROC_TRANS_DATA,        /*!< Trans data.                        */
    PROC_COMMAND_MSG,       /*!< Output first message.              */
    PROC_NACK_DETECT,       /*!< NACK detect                        */
    PROC_MASTER_ERROR,      /*!< Master Error.                      */
    PROC_ERROR              /*!< Error.                             */
} Processing;

static volatile Processing proc;                                    /*!< Processing mode.           */

/*
 *	hal_i2c_tmpm4l.c
 *	I2C device driver (TMPM4L)
*/
/*--------------------------------------------------*/

EXPORT ER dev_i2c_master_trans(bsp_ei2c_t *hi2c, W slave_address, UB *data_buf, B data_size)
{
  static uint32_t length;                                             /*!< Length of Master Tx Data.  */
  ER error = E_OK;
  int32_t work;
  
  length = 0;
  
  /* I2C BB check and startcondition issue */
  EI2C_startcondition(&hi2c->ei2c);
  
  /* Startcondition issue check */
  do
  {
      work = EI2C_status_stcf(&hi2c->ei2c); 
  }
  while(work == 0);

  /* Startcondition issue clear flag */
  EI2C_clear_int_stcf(&hi2c->ei2c);
  
  /* Slave address transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(slave_address<<1));
  
  /* ACK check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
  /* Not Detect = 0  */
      work = EI2C_status_tbe(&hi2c->ei2c);
  }
  while (work == 0);
  
  do
  {
      /* Data transmit */
      EI2C_write_data(&hi2c->ei2c, (uint32_t)*data_buf);
      data_buf++;
      length++;
      
      if( EI2C_status_tend(&hi2c->ei2c) )
      {
          EI2C_clear_int_tend(&hi2c->ei2c);
      }
      
      /* ACK receive */
      do
      {
          if( EI2C_get_ack(&hi2c->ei2c) )
          {
              break;
          }
          work = EI2C_status_tbe(&hi2c->ei2c);
      }
      while (work == 0);
  }
  while (length <= data_size);
  
  EI2C_stop_condition(&hi2c->ei2c);
  EI2C_clear_int_tend(&hi2c->ei2c);
  EI2C_clear_int_nack(&hi2c->ei2c);
  
  return error;
}


EXPORT ER dev_i2c_master_receive(bsp_ei2c_t *hi2c, W slave_address, UB *data_buf, B data_size)
{
  static uint32_t length;                                             /*!< Length of Master Tx Data.  */
  ER error = E_OK;
  int32_t work;
  
  length = 0;
  
  /* I2C BB check and startcondition issue */
  EI2C_startcondition(&hi2c->ei2c);
  
  /* Startcondition issue check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
      work = EI2C_status_stcf(&hi2c->ei2c); 
  }
  while(work == 0);

  /* Startcondition issue clear flag */
  EI2C_clear_int_stcf(&hi2c->ei2c);
  
  /* Slave address transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(slave_address<<1 | 0x01));
  
  do
  {   
      /* Receive wait */
      do
      {
          work = EI2C_status_rbf(&hi2c->ei2c);
      }
      while (work == 0);
      
      /* Data receive */
      *data_buf = (uint32_t)EI2C_read_data(&hi2c->ei2c);
      data_buf++;
      length++;
  }
  while (length < data_size);
  
  EI2C_stop_condition(&hi2c->ei2c);
  EI2C_clear_int_nack(&hi2c->ei2c);
  
  return error;
}


EXPORT ER dev_i2c_master_trans_reg(bsp_ei2c_t *hi2c, W slave_address, W register_buf, UB *data_buf, B data_size)
{
  static uint32_t length;                                             /*!< Length of Master Tx Data.  */
  ER error = E_OK;
  int32_t work;
  
  length = 0;
  
  /* I2C BB check and startcondition issue */
  EI2C_startcondition(&hi2c->ei2c);
  
  /* Startcondition issue check */
  do
  {
      work = EI2C_status_stcf(&hi2c->ei2c); 
  }
  while(work == 0);

  /* Startcondition issue clear flag */
  EI2C_clear_int_stcf(&hi2c->ei2c);
  
  /* Slave address transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(slave_address<<1));
  
  /* ACK check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
  /* Not Detect = 0  */
      work = EI2C_status_tbe(&hi2c->ei2c);
  }
  while (work == 0);

  /* Register transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(register_buf));
  
  /* ACK check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
  /* Not Detect = 0  */
      work = EI2C_status_tbe(&hi2c->ei2c);
  }
  while (work == 0);
  
  do
  {
      /* Data transmit */
      EI2C_write_data(&hi2c->ei2c, (uint32_t)*data_buf);
      data_buf++;
      length++;
      
      if( EI2C_status_tend(&hi2c->ei2c) )
      {
          EI2C_clear_int_tend(&hi2c->ei2c);
      }
      
      /* ACK receive */
      do
      {
          if( EI2C_get_ack(&hi2c->ei2c) )
          {
              break;
          }
          work = EI2C_status_tbe(&hi2c->ei2c);
      }
      while (work == 0);
  }
  while (length <= data_size);
  
  EI2C_stop_condition(&hi2c->ei2c);
  EI2C_clear_int_tend(&hi2c->ei2c);
  EI2C_clear_int_nack(&hi2c->ei2c);
  
  return error;
}

EXPORT ER dev_i2c_master_receive_reg(bsp_ei2c_t *hi2c, W slave_address, W register_buf, UB *data_buf, B data_size)
{
  static uint32_t length;                                             /*!< Length of Master Tx Data.  */
  ER error = E_OK;
  int32_t work;
  
  length = 0;
  
  /* I2C BB check and startcondition issue */
  EI2C_startcondition(&hi2c->ei2c);
  
  /* Startcondition issue check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
      work = EI2C_status_stcf(&hi2c->ei2c); 
  }
  while(work == 0);

  /* Startcondition issue clear flag */
  EI2C_clear_int_stcf(&hi2c->ei2c);
  
  /* Slave address transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(slave_address<<1 | 0x01));
  
  /* ACK check */
  do
  {
      if( EI2C_get_ack(&hi2c->ei2c) )
      {
          break;
      }
  /* Not Detect = 0  */
      work = EI2C_status_tbe(&hi2c->ei2c);
  }
  while (work == 0);

  /* Register transmit */
  EI2C_write_data(&hi2c->ei2c, (uint32_t)(register_buf));
  
  do
  {   
      /* Receive wait */
      do
      {
          work = EI2C_status_rbf(&hi2c->ei2c);
      }
      while (work == 0);
      
      /* Data receive */
      *data_buf = (uint32_t)EI2C_read_data(&hi2c->ei2c);
      data_buf++;
      length++;
  }
  while (length < data_size);
  
  EI2C_stop_condition(&hi2c->ei2c);
  EI2C_clear_int_nack(&hi2c->ei2c);
  
  return error;
}

#endif		/* DEVCNF_USE_HAL_IIC */
#endif		/* MTKBSP_TOSHIBA_BSP */