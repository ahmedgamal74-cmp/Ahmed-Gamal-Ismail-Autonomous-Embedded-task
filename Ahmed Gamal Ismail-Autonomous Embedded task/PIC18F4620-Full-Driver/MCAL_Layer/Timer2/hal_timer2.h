/* 
 * File:   hal_timer2.h
 * Author: user
 *
 * Created on July 25, 2023, 4:29 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */
#define TIMER2_PRESCALER_DEV_BY_1       0
#define TIMER2_PRESCALER_DEV_BY_4       1
#define TIMER2_PRESCALER_DEV_BY_16      2

/* Section : Macro  Function Declarations */
#define TIMER2_MODULE_ENABLE()                  (T2CONbits.TMR2ON = 1)
#define TIMER2_MODULE_DISABLE()                 (T2CONbits.TMR2ON = 0)

#define TIMER2_PRESCALER_SELECT(_PRESCALER_)    (T2CONbits.T2CKPS = _PRESCALER_)
#define TIMER2_POSTSCALER_SELECT(_POSTSCALER_)  (T2CONbits.TOUTPS = _POSTSCALER_)

/* Section : Data Type Declarations */
typedef enum{
    TIMER2_POSTSCALER_DEV_BY_1 = 0,
    TIMER2_POSTSCALER_DEV_BY_2,
    TIMER2_POSTSCALER_DEV_BY_3,
    TIMER2_POSTSCALER_DEV_BY_4,
    TIMER2_POSTSCALER_DEV_BY_5,
    TIMER2_POSTSCALER_DEV_BY_6,
    TIMER2_POSTSCALER_DEV_BY_7,
    TIMER2_POSTSCALER_DEV_BY_8,
    TIMER2_POSTSCALER_DEV_BY_9,
    TIMER2_POSTSCALER_DEV_BY_10,
    TIMER2_POSTSCALER_DEV_BY_11,
    TIMER2_POSTSCALER_DEV_BY_12,
    TIMER2_POSTSCALER_DEV_BY_13,
    TIMER2_POSTSCALER_DEV_BY_14,
    TIMER2_POSTSCALER_DEV_BY_15,
    TIMER2_POSTSCALER_DEV_BY_16        
}timer2_postscaler_select_t;

typedef struct{
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* TMR2_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint8 timer2_preload_value;
    timer2_postscaler_select_t timer2_postscaler_value;
    uint8 timer2_prescaler_value : 2;
    uint8 timer2_reserved : 6;
}timer2_t;

/* Section : Function Declarations */
Std_ReturnType TIMER2_Init(const timer2_t *_timer);
Std_ReturnType TIMER2_DeInit(const timer2_t *_timer);
Std_ReturnType TIMER2_Write_Value(const timer2_t *_timer, uint8 _value);
Std_ReturnType TIMER2_Read_Value(const timer2_t *_timer, uint8 *_value);

#endif	/* HAL_TIMER2_H */

