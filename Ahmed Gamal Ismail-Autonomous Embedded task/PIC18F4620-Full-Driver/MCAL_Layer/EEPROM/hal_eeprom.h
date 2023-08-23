/* 
 * File:   hal_eeprom.h
 * Author: user
 *
 * Created on July 20, 2023, 6:31 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "../Interrupt/mcal_external_interrupt.h"

/* Section : Macro Declarations */
#define EEADR_HIGH_SHIFT_BITS                   8
#define EEADR_HIGH_MASK                         0x03
#define EEADR_LOW_MASK                          0xFF

#define EEPROM_MEMORY_ACCESS                    0
#define EEPROM_OR_FLASH_MEMORY_ACCESS           0
#define WRITE_CYCLE_EEPROM_OR_FLASH_INHIBIT     0
#define WRITE_CYCLE_EEPROM_OR_FLASH_ALLOW       1
#define DATA_ERASE_WRITE_CYCLE_INITATE          1
#define DATA_ERASE_WRITE_CYCLE_COMPLETED        0
#define DATA_READ_CYCLE_INITATE                 1
#define DATA_READ_CYCLE_COMPLETED               0

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */

/* Section : Function Declarations */
Std_ReturnType Data_EEPROM_WriteByte(uint16 bAdd, uint8 bData);
Std_ReturnType Data_EEPROM_ReadByte(uint16 bAdd, uint8 *bData);

#endif	/* HAL_EEPROM_H */

