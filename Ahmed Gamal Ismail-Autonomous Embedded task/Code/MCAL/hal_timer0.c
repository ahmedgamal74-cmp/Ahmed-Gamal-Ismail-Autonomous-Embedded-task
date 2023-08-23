/* 
 * File:   hal_timer0.c
 * Author: user
 *
 * Created on July 24, 2023, 12:16 AM
 */

#include "hal_timer0.h"

#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(*TMR0_InterruptHandler)(void) = NULL;
#endif

static uint16 timer0_preload = ZERO_INIT;

static inline void Timer0_Prescaler_config(const timer0_t *_timer);
static inline void Timer0_Mode_config(const timer0_t *_timer);
static inline void Timer0_Register_Size_config(const timer0_t *_timer);

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER0_Init(const timer0_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER0_MODULE_DISABLE();
        Timer0_Prescaler_config(_timer);
        Timer0_Mode_config(_timer);
        Timer0_Register_Size_config(_timer);
        TMR0H = (uint8)((_timer->timer0_preload_value) >> TMR0_HIGH_SHIFT_BITS);
        TMR0L = (uint8)(_timer->timer0_preload_value);
        timer0_preload = _timer->timer0_preload_value;
        
        #if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

        #if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        
        TMR0_InterruptEnable();
        TMR0_InterruptFlagClear();
        TMR0_InterruptHandler = _timer->TMR0_InterruptHandler;
        
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TMR0_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TMR0_LowPrioritySet();
        }
        else{ /* Nothing */ }
        #endif

        #endif

        TIMER0_MODULE_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @return 
 */
Std_ReturnType TIMER0_DeInit(const timer0_t *_timer){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TIMER0_MODULE_DISABLE();
        #if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TMR0_InterruptDisable();
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
Std_ReturnType TIMER0_Write_Value(const timer0_t *_timer, uint16 _value){
    Std_ReturnType ret = E_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }
    else{
        TMR0H = (uint8)(_value >> TMR0_HIGH_SHIFT_BITS);
        TMR0L = (uint8)(_value);
    }
    return ret;
}

/**
 * 
 * @param _timer
 * @param _value
 * @return 
 */
Std_ReturnType TIMER0_Read_Value(const timer0_t *_timer, uint16 *_value){
    Std_ReturnType ret = E_OK;
    if((NULL == _timer) || (NULL == _value)){
        ret = E_NOT_OK;
    }
    else{
        uint8 l_tmr0Low = TMR0L;
        uint8 l_tmr0High = TMR0H;
        *_value = (uint16)(l_tmr0Low + (l_tmr0High << TMR0_HIGH_SHIFT_BITS));
    }
    return ret;
}

void TMR0_ISR(void){
    TMR0_InterruptFlagClear();
    
    TMR0H = (uint8)((timer0_preload) >> TMR0_HIGH_SHIFT_BITS);
    TMR0L = (uint8)(timer0_preload);
    
    if(TMR0_InterruptHandler){
        TMR0_InterruptHandler();
    }
}
/****************************** Helper Functions ******************************/
/**
 * 
 * @param _timer
 */
static inline void Timer0_Prescaler_config(const timer0_t *_timer){
    if(TIMER0_PRESCAlER_ENABLE_CFG == _timer->prescaler_enable){
        TIMER0_PRESCAlER_ENABLE();
        T0CONbits.T0PS = _timer->prescaler_value;
    }
    else if(TIMER0_PRESCAlER_DISABLE_CFG == _timer->prescaler_enable){
        TIMER0_PRESCAlER_DISABLE();
    }
    else{ /* Nothing */ }
}

/**
 * 
 * @param _timer
 */
static inline void Timer0_Mode_config(const timer0_t *_timer){
    if(TIMER0_TIMER_MODE == _timer->timer0_mode){
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE == _timer->timer0_mode){
        TIMER0_COUNTER_MODE_ENABLE();
        if(TIMER0_RISING_EDGE_CFG == _timer->timer0_counter_edge){
            TIMER0_RISING_EDGE_ENABLE();
        }
        else if(TIMER0_FALLING_EDGE_CFG == _timer->timer0_counter_edge){
            TIMER0_FALLING_EDGE_ENABLE();
        }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
}

static inline void Timer0_Register_Size_config(const timer0_t *_timer){
    if(TIMER0_8BIT_REGISTER_MODE == _timer->timer0_register_size){
        TIMER0_8BIT_REGISTER_MODE_ENABLE();
    }
    else if(TIMER0_16BIT_REGISTER_MODE == _timer->timer0_register_size){
        TIMER0_16BIT_REGISTER_MODE_ENABLE();
    }
    else{ /* Nothing */ }
}