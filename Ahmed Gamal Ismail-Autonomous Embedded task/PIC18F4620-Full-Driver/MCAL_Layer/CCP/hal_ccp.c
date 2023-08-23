/* 
 * File:   hal_ccp.c
 * Author: user
 *
 * Created on July 26, 2023, 5:26 PM
 */

#include "hal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP1_InterruptHandler)(void) = NULL;
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* CCP2_InterruptHandler)(void) = NULL;
#endif  
    
static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static void CCP_Capture_Compare_Mode_Timer_Select(const ccp_t *_ccp_obj);

/**
 * 
 * @param _cpp_obj
 * @return 
 */
Std_ReturnType CCP_Init(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
            if(CCP_CAPTURE_MODE_SELECT == _ccp_obj->ccp_mode){
                switch(_ccp_obj->ccp_submode_cfg){
                    case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
                    case CCP_CAPTURE_MODE_1_RISING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE); break;
                    case CCP_CAPTURE_MODE_4_RISING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE); break;
                    case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
                    default : ret = E_NOT_OK;
                }
                CCP_Capture_Compare_Mode_Timer_Select(_ccp_obj);
            }
            else if(CCP_COMPARE_MODE_SELECT == _ccp_obj->ccp_mode){
                switch(_ccp_obj->ccp_submode_cfg){
                    case CCP_COMPARE_MODE_SET_PIN_HIGH : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); break;
                    case CCP_COMPARE_MODE_SET_PIN_LOW : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); break;
                    case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); break;
                    case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
                    case CCP_COMPARE_MODE_GEN_EVENT : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
                    default : ret = E_NOT_OK;
                }
                CCP_Capture_Compare_Mode_Timer_Select(_ccp_obj);
            }
            else if(CCP_PWM_MODE_SELECT == _ccp_obj->ccp_mode){
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
                if(CCP1_INST == _ccp_obj->ccp_inst){
                    if(CCP_PWM_MODE == _ccp_obj->ccp_submode_cfg){
                        CCP1_SET_MODE(CCP_PWM_MODE);
                    }
                    else{ /* Nothing */ }
                }
                else if(CCP2_INST == _ccp_obj->ccp_inst){
                    if(CCP_PWM_MODE == _ccp_obj->ccp_submode_cfg){
                        CCP2_SET_MODE(CCP_PWM_MODE);
                    }
                    else{ /* Nothing */ }
                }
                else{ /* Nothing */ }
            
                PR2 = (uint8)(((_XTAL_FREQ)/(_ccp_obj->PWM_Frequency * 4.0 * _ccp_obj->timer2_prescaler_value *
                                                _ccp_obj->timer2_postscaler_value)) - 1);      // equation
#endif
            }
            else{ /* Nothing */ }
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
            if(CCP_CAPTURE_MODE_SELECT == _ccp_obj->ccp_mode){
                switch(_ccp_obj->ccp_submode_cfg){
                    case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
                    case CCP_CAPTURE_MODE_1_RISING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE); break;
                    case CCP_CAPTURE_MODE_4_RISING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE); break;
                    case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
                    default : ret = E_NOT_OK;
                }
                CCP_Capture_Compare_Mode_Timer_Select(_ccp_obj);
            }
            else if(CCP_COMPARE_MODE_SELECT == _ccp_obj->ccp_mode){
                switch(_ccp_obj->ccp_submode_cfg){
                    case CCP_COMPARE_MODE_SET_PIN_HIGH : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); break;
                    case CCP_COMPARE_MODE_SET_PIN_LOW : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); break;
                    case CCP_COMPARE_MODE_TOGGLE_ON_MATCH : CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); break;
                    case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
                    case CCP_COMPARE_MODE_GEN_EVENT : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
                    default : ret = E_NOT_OK;
                }
                CCP_Capture_Compare_Mode_Timer_Select(_ccp_obj);
            }
            else if(CCP_PWM_MODE_SELECT == _ccp_obj->ccp_mode){
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
                if(CCP1_INST == _ccp_obj->ccp_inst){
                    if(CCP_PWM_MODE == _ccp_obj->ccp_submode_cfg){
                        CCP1_SET_MODE(CCP_PWM_MODE);
                    }
                    else{ /* Nothing */ }
                }
                else if(CCP2_INST == _ccp_obj->ccp_inst){
                    if(CCP_PWM_MODE == _ccp_obj->ccp_submode_cfg){
                        CCP2_SET_MODE(CCP_PWM_MODE);
                    }
                    else{ /* Nothing */ }
                }
                else{ /* Nothing */ }
            
                PR2 = (uint8)(((_XTAL_FREQ)/(_ccp_obj->PWM_Frequency * 4.0 * _ccp_obj->timer2_prescaler_value *
                                                _ccp_obj->timer2_postscaler_value)) - 1);      // equation
#endif
            }
            else{ /* Nothing */ }
        }
        else{ /* Nothing */ }
        
        ret = gpio_pin_intialize(&(_ccp_obj->ccp_pin));
        
        CCP_Interrupt_Config(_ccp_obj);

        
    }
    return ret;
}

/**
 * 
 * @param _cpp_obj
 * @return 
 */
Std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
#endif
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
#endif
        }
        else{ /* Nothing */ }
    }
    return ret;
}

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_CAPTURE_MODE_SELECTED
/**
 * 
 * @param _duty
 * @return 
 */
Std_ReturnType CCP1_IsCaptureDataReady(uint8 *_capture_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _capture_status){
        ret = E_NOT_OK;
    }
    else{
        
        
        
        if(CCP1_CAPTURE_READY == PIR1bits.CCP1IF){
            *_capture_status = CCP1_CAPTURE_READY;
            CCP1_InterruptFlagClear();
        }
        else if(CCP1_CAPTURE_NOT_READY == PIR1bits.CCP1IF){
            *_capture_status = CCP1_CAPTURE_NOT_READY;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @param _duty
 * @return 
 */
Std_ReturnType CCP1_Capture_Mode_Read_Value(uint16 *_capture_value){
    Std_ReturnType ret = E_OK;
    if(NULL == _capture_value){
        ret = E_NOT_OK;
    }
    else{
        CCP_PERIOD_REG_T capture_temp_val = {.ccpr_low = ZERO_INIT, .ccpr_high = ZERO_INIT};
        capture_temp_val.ccpr_low = CCPR1L;
        capture_temp_val.ccpr_high = CCPR1H;
        *_capture_value = capture_temp_val.ccpr_16bit;
    }
    return ret;
}
#endif

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_COMPARE_MODE_SELECTED
/**
 * 
 * @param _duty
 * @return 
 */
Std_ReturnType CCP_IsCompareCompleted(uint8 *_compare_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _compare_status){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_COMPARE_READY == PIR1bits.CCP1IF){
            *_compare_status = CCP1_COMPARE_READY;
            CCP1_InterruptFlagClear();
        }
        else if(CCP1_COMPARE_NOT_READY == PIR1bits.CCP1IF){
            *_compare_status = CCP1_COMPARE_NOT_READY;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @param _duty
 * @return 
 */
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16 _compare_value){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        CCP_PERIOD_REG_T capture_temp_val = {.ccpr_low = ZERO_INIT, .ccpr_high = ZERO_INIT};
        capture_temp_val.ccpr_16bit = _compare_value;
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCPR1L = capture_temp_val.ccpr_low;
            CCPR1H = capture_temp_val.ccpr_high;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCPR2L = capture_temp_val.ccpr_low;
            CCPR2H = capture_temp_val.ccpr_high;
        }
        else{}
    }
    return ret;
}
#endif

#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED
/**
 * 
 * @param _duty
 * @return 
 */
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        uint16 l_duty_temp = ZERO_INIT;
        l_duty_temp = (uint16)((PR2 + 1) * (_duty / 100.0) * 4.0);      // equation
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.DC1B = (uint8)(l_duty_temp & PWM_DUTY_MASK);
            CCPR1L = (uint8)(l_duty_temp >> PWM_DUTY_SHIFT_BITS);
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.DC2B = (uint8)(l_duty_temp & PWM_DUTY_MASK);
            CCPR2L = (uint8)(l_duty_temp >> PWM_DUTY_SHIFT_BITS);
        }
        else{}
    }
    
    return ret;
}

/**
 * 
 * @return 
 */
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @return 
 */
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        else{}
    }
    return ret;
}
#endif

void CCP1_ISR(void){
    CCP1_InterruptFlagClear();
    
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
    else{}
}

void CCP2_ISR(void){
    CCP2_InterruptFlagClear();
    
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
    else{}
}
/******************************* HELPER FUNCTIONS *****************************/
/**
 * 
 * @param _ccp_obj
 */
static void CCP_Interrupt_Config(const ccp_t *_ccp_obj){
    #if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        CCP1_InterruptEnable();
        CCP1_InterruptFlagClear();
        CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->ccp1_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            CCP1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->ccp1_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            CCP1_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif

#endif
        
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        CCP2_InterruptEnable();
        CCP2_InterruptFlagClear();
        CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->ccp2_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            CCP2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->ccp2_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            CCP2_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif
        
#endif
}

/**
 * 
 * @param _ccp_obj
 */
static void CCP_Capture_Compare_Mode_Timer_Select(const ccp_t *_ccp_obj){
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_compare_select_timer){
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_compare_select_timer){
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_compare_select_timer){
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{}
}