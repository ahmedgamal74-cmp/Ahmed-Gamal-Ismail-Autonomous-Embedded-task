/* 
 * File:   hal_ccp.h
 * Author: user
 *
 * Created on July 26, 2023, 5:26 PM
 */

#ifndef HAL_CCP_H
#define	HAL_CCP_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "../Timer2/hal_timer2.h"
#include "hal_ccp_cfg.h"

/* Section : Macro Declarations */
#define CCP_MODULE_DISABLE                     ((uint8)0x00)
#define CCP_CAPTURE_MODE_1_FALLING_EDGE        ((uint8)0x04)
#define CCP_CAPTURE_MODE_1_RISING_EDGE         ((uint8)0x05)
#define CCP_CAPTURE_MODE_4_RISING_EDGE         ((uint8)0x06)
#define CCP_CAPTURE_MODE_16_RISING_EDGE        ((uint8)0x07)
#define CCP_COMPARE_MODE_SET_PIN_HIGH          ((uint8)0x08)
#define CCP_COMPARE_MODE_SET_PIN_LOW           ((uint8)0x09)
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH       ((uint8)0x02)
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT      ((uint8)0x0A)
#define CCP_COMPARE_MODE_GEN_EVENT             ((uint8)0x0B)
#define CCP_PWM_MODE                           ((uint8)0x0C)

#define CCP1_CAPTURE_NOT_READY                  (0x00)
#define CCP1_CAPTURE_READY                      (0x01)

#define CCP1_COMPARE_NOT_READY                  (0x00)
#define CCP1_COMPARE_READY                      (0x01)

#define PWM_DUTY_MASK           0x0003
#define PWM_DUTY_SHIFT_BITS     2

#define CCP_POSTSCALER_DEV_BY_1        1
#define CCP_POSTSCALER_DEV_BY_2        2
#define CCP_POSTSCALER_DEV_BY_3        3
#define CCP_POSTSCALER_DEV_BY_4        4
#define CCP_POSTSCALER_DEV_BY_5        5
#define CCP_POSTSCALER_DEV_BY_6        6
#define CCP_POSTSCALER_DEV_BY_7        7
#define CCP_POSTSCALER_DEV_BY_8        8
#define CCP_POSTSCALER_DEV_BY_9        9
#define CCP_POSTSCALER_DEV_BY_10       10
#define CCP_POSTSCALER_DEV_BY_11       11
#define CCP_POSTSCALER_DEV_BY_12       12
#define CCP_POSTSCALER_DEV_BY_13       13
#define CCP_POSTSCALER_DEV_BY_14       14
#define CCP_POSTSCALER_DEV_BY_15       15
#define CCP_POSTSCALER_DEV_BY_16       16

#define CCP_PRESCALER_DEV_BY_1         1
#define CCP_PRESCALER_DEV_BY_4         4
#define CCP_PRESCALER_DEV_BY_16        16

/* Section : Macro  Function Declarations */
#define CCP1_SET_MODE(_CONFIG_)     (CCP1CONbits.CCP1M = _CONFIG_)
#define CCP2_SET_MODE(_CONFIG_)     (CCP2CONbits.CCP2M = _CONFIG_)

/* Section : Data Type Declarations */
typedef enum{
    CCP_CAPTURE_MODE_SELECT = 0,
    CCP_COMPARE_MODE_SELECT,
    CCP_PWM_MODE_SELECT
}ccp1_mode_t;

typedef union{
    struct{
        uint8 ccpr_low;
        uint8 ccpr_high;
    };
    struct{
        uint16 ccpr_16bit;
    };
}CCP_PERIOD_REG_T;

typedef enum{
    CCP1_INST,
    CCP2_INST 
}ccp_inst_t;

typedef enum{
    CCP1_CCP2_TIMER3 = 0,
    CCP1_TIMER1_CCP2_TIMER3,
    CCP1_CCP2_TIMER1
}ccp_capture_compare_select_timer_t;

typedef struct{
    ccp_inst_t ccp_inst;
    ccp1_mode_t ccp_mode;
    uint8 ccp_submode_cfg;
    pin_config_t ccp_pin;
    ccp_capture_compare_select_timer_t ccp_capture_compare_select_timer;
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP1_InterruptHandler)(void);
    interrupt_priority_cfg ccp1_priority;
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP2_InterruptHandler)(void);
    interrupt_priority_cfg ccp2_priority;
#endif   
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
    uint32 PWM_Frequency;
    uint8 timer2_postscaler_value : 4;
    uint8 timer2_prescaler_value : 2;
#endif
    
}ccp_t;

/* Section : Function Declarations */
Std_ReturnType CCP_Init(const ccp_t *_cpp_obj);
Std_ReturnType CCP_DeInit(const ccp_t *_cpp_obj);

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_CAPTURE_MODE_SELECTED
Std_ReturnType CCP1_IsCaptureDataReady(uint8 *_capture_status);
Std_ReturnType CCP1_Capture_Mode_Read_Value(uint16 *_capture_value);
#endif

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_COMPARE_MODE_SELECTED
Std_ReturnType CCP_IsCompareCompleted(uint8 *_compare_status);
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16 _compare_value);
#endif

#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty);
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj);
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj);
#endif

#endif	/* HAL_CCP_H */

