/* 
 * File:   ecu_dc_motor.c
 * Author: user
 *
 * Created on July 12, 2023, 12:37 AM
 */

#include "ecu_dc_motor.h"

/**
 * 
 * @param _dc_motor
 * @return 
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_intialize(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]));
        gpio_pin_intialize(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN2]));
    }
    return ret;
}

/**
 * 
 * @param _dc_motor
 * @return 
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_HIGH);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN2]), GPIO_LOW);
    }
    return ret;
}

/**
 * 
 * @param _dc_motor
 * @return 
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN2]), GPIO_HIGH);
    }
    return ret;
}

/**
 * 
 * @param _dc_motor
 * @return 
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *_dc_motor){
    Std_ReturnType ret = E_OK;
    if(NULL == _dc_motor){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[DC_MOTOR_PIN2]), GPIO_LOW);
    }
    return ret;
}
