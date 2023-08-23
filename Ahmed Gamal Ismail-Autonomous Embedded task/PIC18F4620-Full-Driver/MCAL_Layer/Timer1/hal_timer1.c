/* 
 * File:   hal_timer1.c
 * Author: user
 *
 * Created on July 24, 2023, 11:12 PM
 */

#include "hal_timer1.h"

#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*TMR1_InterruptHandler)(void) = NULL;
#endif

static uint16 timer1_preload = ZERO_INIT;

static inline void Timer1_Mode_config(const timer1_t *_timer);

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER1_Init(const timer1_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){   
        ret = E_NOT_OK;
    }
    else{
        TIMER1_MODULE_DISABLE();
        TIMER1_PRESCALER_SELECT(_timer->timer1_prescaler_value);
        Timer1_Mode_config(_timer);
        TMR1H = (uint8)((_timer->timer1_preload_value) >> TMR1_HIGH_SHIFT_BITS);
        TMR1L = (uint8)(_timer->timer1_preload_value);
        timer1_preload = _timer->timer1_preload_value;
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        TMR1_InterruptEnable();
        TMR1_InterruptFlagClear();
        TMR1_InterruptHandler = _timer->TMR1_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TMR1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TMR1_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif

#endif
        TIMER1_MODULE_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER1_DeInit(const timer1_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){   
        ret = E_NOT_OK;
    }
    else{
        TIMER1_MODULE_DISABLE();
        #if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TMR1_InterruptDisable();
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
Std_ReturnType TIMER1_Write_Value(const timer1_t *_timer, uint16 _value){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){   
        ret = E_NOT_OK;
    }
    else{
        TMR1H = (uint8)((_value) >> TMR1_HIGH_SHIFT_BITS);
        TMR1L = (uint8)(_value);
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType TIMER1_Read_Value(const timer1_t *_timer, uint16 *_value){
    Std_ReturnType ret = E_OK;
    if((NULL == _timer) || (NULL == _value)){   
        ret = E_NOT_OK;
    }
    else{
        uint8 l_tmr1Low = TMR1L;
        uint8 l_tmr1High = TMR1H;
        *_value = (uint16)(l_tmr1Low + (l_tmr1High << TMR1_HIGH_SHIFT_BITS));
    }
    return ret;
}

void TMR1_ISR(void){
    TMR1_InterruptFlagClear();
    
    TMR1H = (uint8)((timer1_preload) >> TMR1_HIGH_SHIFT_BITS);
    TMR1L = (uint8)(timer1_preload);
    
    if(TMR1_InterruptHandler){
        TMR1_InterruptHandler();
    }
}
/****************************** Helper Functions ******************************/
/**
 * 
 * @param _timer
 */
static inline void Timer1_Mode_config(const timer1_t *_timer){
    if(TIMER1_TIMER_MODE == _timer->timer1_mode){
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == _timer->timer1_mode){
        TIMER1_COUNTER_MODE_ENABLE();
        if(TIMER1_SYNC_COUNTER_MODE == _timer->timer1_counter_mode){
            TIMER1_SYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER1_ASYNC_COUNTER_MODE == _timer->timer1_counter_mode){
            TIMER1_ASYNC_COUNTER_MODE_ENABLE();
        }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
}