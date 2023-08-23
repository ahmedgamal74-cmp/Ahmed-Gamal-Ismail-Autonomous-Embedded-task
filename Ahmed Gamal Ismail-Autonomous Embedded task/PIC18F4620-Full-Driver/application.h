/* 
 * File:   application.h
 * Author: user
 *
 * Created on June 24, 2023, 4:25 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : Includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
#include "MCAL_Layer/CCP/hal_ccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/SPI/hal_spi.h"
#include "MCAL_Layer/I2C/hal_i2c.h"

/* Section : Macro Declarations */

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */
//extern chr_lcd_4bit_t lcd_1;


/* Section : Function Declarations */
void application_initialize(void);

//#define _XTAL_FREQ 8000000UL /* Crystal : for delay function error */

#endif	/* APPLICATION_H */

