/**************************************************************************************************
  Filename:       PGA450.h
  Revised:        $Date: 2016-03-01 16:46:59
  Revision:       $Revision: 44594 $

  Description:    This file contains the PGA450 driver
**************************************************************************************************/

#ifndef PGA450_H
#define PGA450_H

#ifdef __cplusplus
extern "C"
{
#endif

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

#include <ti/drivers/lcd/LCDDogm1286.h>
/*********************************************************************
 * TYPEDEFS
 */
typedef struct PGA450_W_Package {
	uint8 cmd;
	uint8 address;
	uint8 data0;
	uint8 data1;
} PGA450_W_Package;

typedef struct PGA450_R_Package {
	uint8 CheckByte;
	uint8 data;
} PGA450_R_Package;
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern PGA450_W_Package PGA450_Write;
extern PGA450_R_Package PGA450_Read;
extern SPI_Handle SPI_PGA450_Handle;
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
#define CMD_Internal_RAM_write    0x13
#define CMD_Internal_RAM_read     0x10
#define CMD_ESFR_write            0x1A
#define CMD_ESFR_read             0x19
#define CMD_OTP_write             0x07
#define CMD_OTP_read              0x04
#define CMD_EEPROM_cache_write    0x0E
#define CMD_EEPROM_cache_read     0x0D
#define CMD_External_RAM_write    0x0B
#define CMD_External_RAM_read     0x08
#define CMD_DEV_RAM_write         0x1F
#define CMD_DEV_RAM_read          0x1C
#define CMD_TEST_write            0x16
#define CMD_TEST_read             0x15
/*********************************************************************
 * FUNCTIONS
 */
void PGA450_Reset(void);
void PGA450_Release(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PGA450_H */
