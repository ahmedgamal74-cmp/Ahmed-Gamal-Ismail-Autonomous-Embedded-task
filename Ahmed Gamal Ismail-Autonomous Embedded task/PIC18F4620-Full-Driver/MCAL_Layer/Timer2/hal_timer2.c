/* 
 * File:   hal_timer2.c
 * Author: user
 *
 * Created on July 25, 2023, 4:29 PM
 */

#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* TMR2_InterruptHandler)(void) = NULL;
#endif
    
static uint8 timer2_preload = ZERO_INIT;

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER2_Init(const timer2_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER2_MODULE_DISABLE();
        TIMER2_PRESCALER_SELECT(_timer->timer2_prescaler_value);
        TIMER2_POSTSCALER_SELECT(_timer->timer2_postscaler_value);
        TMR2 = (uint8)(_timer->timer2_preload_value);
        timer2_preload = (uint8)(_timer->timer2_preload_value);
        
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        TMR2_InterruptEnable();
        TMR2_InterruptFlagClear();
        TMR2_InterruptHandler = _timer->TMR2_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TMR2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TMR2_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif
        
#endif
        
        TIMER2_MODULE_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER2_DeInit(const timer2_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER2_MODULE_DISABLE();
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TMR2_InterruptDisable();
#endif
        
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType TIMER2_Write_Value(const timer2_t *_timer, uint8 _value){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TMR2 = (uint8)(_value);
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType TIMER2_Read_Value(const timer2_t *_timer, uint8 *_value){
    Std_ReturnType ret = E_OK;
    if((NULL == _timer) || (NULL == _value)){
        ret = E_NOT_OK;
    }
    else{
        *_value = (uint8)(TMR2);
    }
    return ret;
}

void TMR2_ISR(void){
    TMR2_InterruptFlagClear();
    
    TMR2 = (uint8)(timer2_preload);
    
    if(TMR2_InterruptHandler){
        TMR2_InterruptHandler();
    }
}