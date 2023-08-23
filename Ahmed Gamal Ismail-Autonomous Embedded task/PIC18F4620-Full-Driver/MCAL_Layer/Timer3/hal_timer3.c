/* 
 * File:   hal_timer3.c
 * Author: user
 *
 * Created on July 25, 2023, 6:56 PM
 */

#include "hal_timer3.h"

#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* TMR3_InterruptHandler)(void) = NULL;
#endif
    
static uint16 timer3_preload = ZERO_INIT;

static inline void Timer3_Mode_config(const timer3_t *_timer);

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER3_Init(const timer3_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER3_MODULE_DISABLE();
        Timer3_Mode_config(_timer);
        TIMER3_PRESCALER_SELECT(_timer->timer3_prescaler_value);
        TMR3H = (uint8)((_timer->timer3_preload_value) >> TMR3_HIGH_SHIFT_BITS);
        TMR3L = (uint8)(_timer->timer3_preload_value);
        timer3_preload = _timer->timer3_preload_value;
        
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        TMR3_InterruptEnable();
        TMR3_InterruptFlagClear();
        TMR3_InterruptHandler = _timer->TMR3_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TMR3_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TMR3_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif

#endif
        
        TIMER3_MODULE_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER3_DeInit(const timer3_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER3_MODULE_DISABLE();
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TMR3_InterruptDisable();
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
Std_ReturnType TIMER3_Write_Value(const timer3_t *_timer, uint16 _value){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TMR3H = (uint8)((_value) >> TMR3_HIGH_SHIFT_BITS);
        TMR3L = (uint8)(_value);
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType TIMER3_Read_Value(const timer3_t *_timer, uint16 *_value){
    Std_ReturnType ret = E_OK;
    if((NULL == _timer) || (NULL == _value)){
        ret = E_NOT_OK;
    }
    else{
        uint8 l_tmr3Low = TMR3L;
        uint8 l_tmr3High = TMR3H;
        *_value = (uint16)(l_tmr3Low + (l_tmr3High << TMR3_HIGH_SHIFT_BITS));
    }
    return ret;
}

void TMR3_ISR(void){
    TMR3_InterruptFlagClear();
    
    TMR3H = (uint8)(timer3_preload >> TMR3_HIGH_SHIFT_BITS);
    TMR3L = (uint8)(timer3_preload);
    
    if(TMR3_InterruptHandler){
        TMR3_InterruptHandler();
    }
}
/***************************** Helper Functions *******************************/
static inline void Timer3_Mode_config(const timer3_t *_timer){
    if(TIMER3_TIMER_MODE == _timer->timer3_mode){
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE == _timer->timer3_mode){
        TIMER3_COUNTER_MODE_ENABLE();
        if(TIMER3_SYNC_COUNTER_MODE == _timer->timer3_counter_mode){
            TIMER3_SYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER3_ASYNC_COUNTER_MODE == _timer->timer3_counter_mode){
            TIMER3_ASYNC_COUNTER_MODE_ENABLE();
        }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
}