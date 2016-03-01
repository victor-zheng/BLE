/*******************************************************************************
  Filename:       PGA450.c
  Revised:        $Date: 2016-03-01 16:46:59
  Revision:       $Revision: 44594 $

  Description:    This file contains the PGA450 driver
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "hci_tl.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "simpleGATTprofile.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_lcd.h"
#include "board_key.h"
#include "Board.h"

#include "PGA450.h"

#include <ti/drivers/lcd/LCDDogm1286.h>

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * PUBLIC VARIABLES
 */
PGA450_W_Package PGA450_Write;
PGA450_R_Package PGA450_Read;
SPI_Handle SPI_PGA450_Handle;
/*********************************************************************
 * LOCAL FUNCTIONS
 */



/*********************************************************************
 * PROFILE CALLBACKS
 */


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLEPeripheral_createTask
 *
 * @brief   Task creation function for the Simple BLE Peripheral.
 *
 * @param   None.
 *
 * @return  None.
 */
void PGA450_Transfer(SPI_Handle Handle, PGA450_W_Package *txbuf, PGA450_R_Package *rxbuf)
{
  // SPI_Transaction spiTransaction;
  uint8 tbuf[4];
  uint8 rbuf[4];
  tbuf[0] = txbuf->cmd;
  tbuf[1] = txbuf->address;
  tbuf[2] = txbuf->data0;
  tbuf[3] = txbuf->data1;

  SPI_Transaction spiTransaction;
  spiTransaction.arg = NULL;
  spiTransaction.txBuf = tbuf;
  spiTransaction.rxBuf = rbuf;
  if(txbuf->cmd==CMD_OTP_write ||
     txbuf->cmd==CMD_OTP_read ||
	 txbuf->cmd==CMD_External_RAM_write ||
	 txbuf->cmd==CMD_DEV_RAM_write ||
	 txbuf->cmd==CMD_DEV_RAM_read
	 )
  {
	  spiTransaction.count = 4;
  }
  else
  {
	  spiTransaction.count = 3;
  }

  SPI_transfer(Handle, &spiTransaction);

  rxbuf->CheckByte = rbuf[0];
  rxbuf->data = rbuf[1];
}

void PGA450_Reset(void)
{
	PGA450_Write.cmd = CMD_TEST_write;
	PGA450_Write.address = 0x2F;
	PGA450_Write.data0 = 1;
	PGA450_Transfer(SPI_PGA450_Handle, &PGA450_Write, &PGA450_Read);
}

void PGA450_Release(void)
{

	PGA450_Write.cmd = CMD_TEST_write;
	PGA450_Write.address = 0x2F;
	PGA450_Write.data0 = 0;
	PGA450_Transfer(SPI_PGA450_Handle, &PGA450_Write, &PGA450_Read);
}

/*********************************************************************
*********************************************************************/
