/* 
 * File:   ecu_led.h
 * Author: user
 *
 * Created on June 24, 2023, 4:19 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/* Section : Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_led_cfg.h"

/* Section : Macro Declarations */

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */
typedef enum{
    LED_OFF = 0,
    LED_ON
}led_status_t;

typedef struct{
    uint8 port_name : 4; /* needs 3 and 1 reserved for future usage */
    uint8 pin : 3;
    uint8 led_status : 1;
}led_t;

/* Section : Function Declarations */
Std_ReturnType led_initialize(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);


#endif	/* ECU_LED_H */

