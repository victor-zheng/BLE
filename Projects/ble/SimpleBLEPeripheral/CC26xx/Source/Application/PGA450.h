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
typedef struct Bandpass_Coefficient {
	uint16 A2;
	uint16 A3;
	uint16 B1;
}Bandpass_Coefficient;

typedef struct Lowpass_Coefficient {
	uint16 A2;
	uint16 B1;
}Lowpass_Coefficient;

typedef struct Transducer_Driver_Parameter {
	uint8    burst_mode;        //0=Push-pull mode
	                            //1=Single-ended on OUTA, micro control on OUTB
	                            //2=Single-ended on OUTB, micro control on OUTA
	                            //3=Single-ended on OUTA, single-ended on OUTB
	                            //4=Micro on OUTA, micro on OUTB
	uint8    drive_voltage;     //0-15   4.7V-8.4V
	uint8    outA_pulse_num;    //6bit
	uint8    outB_pulse_num;    //6bit
	uint32   frequency;         //49-70kHz
	uint16   outA_on_time;
	uint16   outA_off_time;
	uint16   outB_on_time;
	uint16   outB_off_time;
	uint8    dead_time;

}Transducer_Driver_Parameter;

typedef struct PGA450_Parameter {
	Transducer_Driver_Parameter  driver;
	uint8                        LNA_gain;      //0=1750V/  1=930V/V  2=517V/V  3=104V/V
	uint8                        BPF_CF;        //39-70
	uint8                        BPF_BW;        //4-7
	uint8                        downsample;    //if low pass enable 25 ¡Ü DOWNSAMPLE ¡Ü 50
	uint8                        LPF_CUT;       //0-7  0.5kHz-4kHz
	uint8                        FIFO_mode;     //0=12bit   1=upper 8bit  2=lower 8bit 3=mid 8bit
	uint8                        blink_time;    //0-255
}PGA450_Parameter;

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
// command code
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

// ESFR Memory Map
//bandpass
#define ADDR_BPF_B1_MSB           0x92
#define ADDR_BPF_B1_LSB           0x93
#define ADDR_BPF_A2_MSB           0x94
#define ADDR_BPF_A2_LSB           0x95
#define ADDR_BPF_A3_MSB           0x96
#define ADDR_BPF_A3_LSB           0x97
//lowpass
#define ADDR_LPF_B1_MSB           0xA1
#define ADDR_LPF_B1_LSB           0xA2
#define ADDR_LPF_A2_MSB           0xA3
#define ADDR_LPF_A2_LSB           0xA4
//
#define ADDR_DOWNSAMPLE           0xA5
//
#define ADDR_ONA_MSB              0xA6
#define ADDR_ONA_LSB              0xA7
#define ADDR_OFFA_MSB             0xA9
#define ADDR_OFFA_LSB             0xAA
//
#define ADDR_ONB_MSB              0xAB
#define ADDR_ONB_LSB              0xAC
#define ADDR_OFFB_MSB             0xAD
#define ADDR_OFFB_LSB             0xAE
//
#define ADDR_PULSE_CNTA           0xAF
#define ADDR_PULSE_CNTB           0xB1
#define ADDR_DEADTIME             0xB2
#define ADDR_BURST_MODE           0xB3
#define ADDR_TEMP_SENS            0xB4
//
#define ADDR_SAT_DEGLITCH         0xB5
#define ADDR_SAT_TIME             0xB6
//
#define ADDR_CONTROL_1            0xB7
#define ADDR_BLANKING_TIME        0xB9
#define ADDR_FRT_MSB              0xBA
#define ADDR_FRT_LSB              0xBB
//
#define ADDR_GPIO_CTRL            0xBC
#define ADDR_CLK_SEL              0xBD
#define ADDR_WD_EN                0xBE
#define ADDR_LIN_SCI              0xBF
#define ADDR_EE_CTRL              0xC0
#define ADDR_STATUS1              0xC1
#define ADDR_STATUS2              0xC2
#define ADDR_PWR_MODE             0xC3
#define ADDR_DP_SCI_CTRL          0xC4
#define ADDR_FIFO_CTRL            0xC5
#define ADDR_EN_CTRL              0xC8
//
#define ADDR_RX_DATA1             0xC9
#define ADDR_RX_DATA2             0xCA
#define ADDR_RX_DATA3             0xCB
#define ADDR_RX_DATA4             0xCC
#define ADDR_RX_DATA5             0xCD
#define ADDR_RX_DATA6             0xCE
#define ADDR_RX_DATA7             0xCF
#define ADDR_RX_DATA8             0xD1
#define ADDR_LIN_PID              0xD2
#define ADDR_TX_DATA1             0xD3
#define ADDR_TX_DATA2             0xD4
#define ADDR_TX_DATA3             0xD5
#define ADDR_TX_DATA4             0xD6
#define ADDR_TX_DATA5             0xD7
#define ADDR_TX_DATA6             0xD8
#define ADDR_TX_DATA7             0xD9
#define ADDR_TX_DATA8             0xDA
#define ADDR_DATA_CNT             0xDB
#define ADDR_LIN_CFG              0xDC
#define ADDR_LIN_CTRL             0xDD
#define ADDR_LIN_STATUS           0xDE
#define ADDR_FIFO_POINTER_MSB     0xDF
#define ADDR_FIFO_POINTER_LSB     0xE1
#define ADDR_VREG_SEL             0xE2
#define ADDR_SYNC_COUNT_MSB       0xE3
#define ADDR_SYNC_COUNT_LSB       0xE4
#define ADDR_TEMP_DAC_CTRL        0xE5
#define ADDR_OSC_SYNC_CTRL        0xE6
#define ADDR_ANALOG_MUX           0xE9
#define ADDR_DIGITAL_MUX          0xEA


// parameter for burst mode
#define PARA_MODE_PUSH_PULL       0
#define PARA_MODE_SINGLE_A        1
#define PARA_MODE_SINGLE_B        2
#define PARA_MODE_SINGLE_AB       3
#define PARA_MODE_MICRO_AB        4

//parameter for output voltage
#define PARA_VOLTAGE_4P7V         0
#define PARA_VOLTAGE_4P8V         1
#define PARA_VOLTAGE_4P9V         2
#define PARA_VOLTAGE_5P0V         3
#define PARA_VOLTAGE_5P1V         4
#define PARA_VOLTAGE_5P2V         5
#define PARA_VOLTAGE_5P3V         6
#define PARA_VOLTAGE_5P4V         7
#define PARA_VOLTAGE_7P7V         8
#define PARA_VOLTAGE_7P8V         9
#define PARA_VOLTAGE_7P9V         10
#define PARA_VOLTAGE_8P0V         11
#define PARA_VOLTAGE_8P1V         12
#define PARA_VOLTAGE_8P2V         13
#define PARA_VOLTAGE_8P3V         14
#define PARA_VOLTAGE_8P4V         15

//parameter for output frequency
#define PARA_FREQ_39kHZ           39
#define PARA_FREQ_40kHZ           40
#define PARA_FREQ_41kHZ           41
#define PARA_FREQ_42kHZ           42
#define PARA_FREQ_43kHZ           43
#define PARA_FREQ_44kHZ           44
#define PARA_FREQ_45kHZ           45
#define PARA_FREQ_46kHZ           46
#define PARA_FREQ_47kHZ           47
#define PARA_FREQ_48kHZ           48
#define PARA_FREQ_49kHZ           49
#define PARA_FREQ_50kHZ           50
#define PARA_FREQ_51kHZ           51
#define PARA_FREQ_52kHZ           52
#define PARA_FREQ_53kHZ           53
#define PARA_FREQ_54kHZ           54
#define PARA_FREQ_55kHZ           55
#define PARA_FREQ_56kHZ           56
#define PARA_FREQ_57kHZ           57
#define PARA_FREQ_58kHZ           58
#define PARA_FREQ_59kHZ           59
#define PARA_FREQ_60kHZ           60
#define PARA_FREQ_61kHZ           61
#define PARA_FREQ_62kHZ           62
#define PARA_FREQ_63kHZ           63
#define PARA_FREQ_64kHZ           64
#define PARA_FREQ_65kHZ           65
#define PARA_FREQ_66kHZ           66
#define PARA_FREQ_67kHZ           67
#define PARA_FREQ_68kHZ           68
#define PARA_FREQ_69kHZ           69
#define PARA_FREQ_70kHZ           70
//parameter for LNA gain
#define PARA_LNA_GAIN_40dB        0
#define PARA_LNA_GAIN_54dB        1
#define PARA_LNA_GAIN_59dB        2
#define PARA_LNA_GAIN_64dB        3

//parameter for bandpass filter
#define PARA_BPF_BW_4kHz          4
#define PARA_BPF_BW_5kHz          5
#define PARA_BPF_BW_6kHz          6
#define PARA_BPF_BW_7kHz          7

//parameter for lowpass filter
#define PARA_LPF_0P5kHz           0
#define PARA_LPF_1P0kHz           1
#define PARA_LPF_1P5kHz           2
#define PARA_LPF_2P0kHz           3
#define PARA_LPF_2P5kHz           4
#define PARA_LPF_3P0kHz           5
#define PARA_LPF_3P5kHz           6
#define PARA_LPF_4P0kHz           7

//parameter for FIFO mode
#define PARA_FIFO_12BIT           0
#define PARA_FIFO_8BIT_UP         1
#define PARA_FIFO_8BIT_LOW        2
#define PARA_FIFO_8BIT_MID        3
/*********************************************************************
 * INTERNAL FUNCTIONS
 */
uint8 Read_ESFR(uint8 addr, uint8* data);
uint8 Write_ESFR(uint8 addr, uint8 data);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void PGA450_Reset(void);
void PGA450_Release(void);
void Initial_PGA450(void);
void Turn_no_Sample(void);
void Turn_off_Sample(void);
void Read_PGA450_FIFO(uint8* pbuf);
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern PGA450_W_Package PGA450_Write;
extern PGA450_R_Package PGA450_Read;
extern SPI_Handle SPI_PGA450_Handle;
extern volatile uint8 FIFO_Buffer[768];

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PGA450_H */
