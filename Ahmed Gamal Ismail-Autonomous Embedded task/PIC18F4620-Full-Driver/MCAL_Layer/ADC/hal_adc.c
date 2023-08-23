/* 
 * File:   hal_adc.c
 * Author: user
 *
 * Created on July 21, 2023, 9:00 PM
 */

#include "hal_adc.h"

#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE    
static void (*ADC_InterruptHandler)(void) = NULL;     
#endif

static inline void adc_input_channel_port_configure(adc_channel_select_t channel);
static inline void select_result_fromat(const adc_conf_t *_adc);
static inline void configure_voltage_reference(const adc_conf_t *_adc);

/**
 * 
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_Init(const adc_conf_t *_adc){
    Std_ReturnType ret = E_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Configure the acquisition time */
        ADCON2bits.ACQT = _adc->acquisition_time;
        /* Configure the conversion clock */
        ADCON2bits.ADCS = _adc->conversion_clock;
        /* Configure the default channel */
        ADCON0bits.CHS = _adc->adc_channel;
        adc_input_channel_port_configure(_adc->adc_channel);
        /* Configure the interrupt */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE          
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif      
        ADC_InterruptEnable();
        ADC_InterruptFlagClear();
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        if(INTERRUPT_HIGH_PRIORITY == _adc->priority){
            ADC_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _adc->priority){
            ADC_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif
        
        ADC_InterruptHandler = _adc->ADC_IterruptHandler;
#endif
        /* Configure the result format */
        select_result_fromat(_adc);
        /* Configure the voltage reference */
        configure_voltage_reference(_adc);
        /* Enable the ADC */
        ADC_CONVERTER_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_DeInit(const adc_conf_t *_adc){
    Std_ReturnType ret = E_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Disable the interrupt */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE   
        ADC_InterruptDisable();
#endif 
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_Select_Channel(const adc_conf_t *_adc, adc_channel_select_t channel){
    Std_ReturnType ret = E_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ADCON0bits.CHS = channel;
        adc_input_channel_port_configure(channel);
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_Start_Conversion(const adc_conf_t *_adc){
    Std_ReturnType ret = E_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        /* Start the conversion */
        ADC_START_CONVERSION();
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_IsConversionDone(const adc_conf_t *_adc, uint8 *conversion_status){
    Std_ReturnType ret = E_OK;
    if((NULL == _adc)|| (NULL == conversion_status)){
        ret = E_NOT_OK;
    }
    else{
        *conversion_status = (uint8)(!(ADC_CONVERSION_STATUS()));
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @param conversion_result
 * @return 
 */
Std_ReturnType ADC_Get_Conversion_Result(const adc_conf_t *_adc, adc_result_t *conversion_result){
    Std_ReturnType ret = E_OK;
    if((NULL == _adc)|| (NULL == conversion_result)){
        ret = E_NOT_OK;
    }
    else{
        if(ADC_RESULT_RIGHT  == _adc->result_format){
            *conversion_result = (adc_result_t)((ADRESH << ADR_RIGHT_RESULT_SHIFT_BITS) + (ADRESL));
        }
        else if(ADC_RESULT_LEFT  == _adc->result_format){
            *conversion_result = (adc_result_t)(((ADRESH << ADR_RIGHT_RESULT_SHIFT_BITS) + (ADRESL)) >>  ADR_LEFT_RESULT_SHIFT_BITS);
        }
        else{
            *conversion_result = (adc_result_t)((ADRESH << ADR_RIGHT_RESULT_SHIFT_BITS) + (ADRESL));
        }
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @param channel
 * @param conversion_result
 * @return 
 */
Std_ReturnType ADC_GetConversion_WithBlocking(const adc_conf_t *_adc, adc_channel_select_t channel,
                                  adc_result_t *conversion_result){
    Std_ReturnType ret = E_OK;
    if((NULL == _adc)|| (NULL == conversion_result)){
        ret = E_NOT_OK;
    }
    else{
        /* Select the Channel */
        ret = ADC_Select_Channel(_adc, channel);
        /* Start the conversion */
        ret &= ADC_Start_Conversion(_adc);
        /* Wait for the conversion to be completed */
        while(ADC_CONVERSION_STATUS());
        /* Get the Result */
        ret &= ADC_Get_Conversion_Result(_adc, conversion_result);
    }
    return ret;
}

/**
 * 
 * @param _adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_StartConversion_WithInterrupt(const adc_conf_t *_adc, adc_channel_select_t channel){
    Std_ReturnType ret = E_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        /* Select the Channel */
        ret = ADC_Select_Channel(_adc, channel);
        /* Start the conversion */
        ret &= ADC_Start_Conversion(_adc);
    }
    return ret;
}
/******************************* Helper Functions *****************************/
/**
 * 
 * @param _adc
 */
static inline void adc_input_channel_port_configure(adc_channel_select_t channel){
    switch(channel){
        case ADC_CHANNEL_AN0  : SET_BIT(TRISA, _TRISA_RA0_POSN); break;     /* Disable the digital output driver */
        case ADC_CHANNEL_AN1  : SET_BIT(TRISA, _TRISA_RA1_POSN); break;
        case ADC_CHANNEL_AN2  : SET_BIT(TRISA, _TRISA_RA2_POSN); break;
        case ADC_CHANNEL_AN3  : SET_BIT(TRISA, _TRISA_RA3_POSN); break;
        case ADC_CHANNEL_AN4  : SET_BIT(TRISA, _TRISA_RA5_POSN); break;
        case ADC_CHANNEL_AN5  : SET_BIT(TRISE, _TRISE_RE0_POSN); break;
        case ADC_CHANNEL_AN6  : SET_BIT(TRISE, _TRISE_RE1_POSN); break;
        case ADC_CHANNEL_AN7  : SET_BIT(TRISE, _TRISE_RE2_POSN); break;
        case ADC_CHANNEL_AN8  : SET_BIT(TRISB, _TRISB_RB2_POSN); break;
        case ADC_CHANNEL_AN9  : SET_BIT(TRISB, _TRISB_RB3_POSN); break;
        case ADC_CHANNEL_AN10 : SET_BIT(TRISB, _TRISB_RB1_POSN); break;
        case ADC_CHANNEL_AN11 : SET_BIT(TRISB, _TRISB_RB4_POSN); break;
        case ADC_CHANNEL_AN12 : SET_BIT(TRISB, _TRISB_RB0_POSN); break;
        default : /* Nothing */;
    }
}

/**
 * 
 * @param _adc
 */
static inline void select_result_fromat(const adc_conf_t *_adc){
    if(ADC_RESULT_RIGHT  == _adc->result_format){
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if(ADC_RESULT_LEFT  == _adc->result_format){
        ADC_RESULT_LEFT_FORMAT();
    }
    else{
        ADC_RESULT_RIGHT_FORMAT();      // default
    }
}

/**
 * 
 * @param _adc
 */
static inline void configure_voltage_reference(const adc_conf_t *_adc){
    if(ADC_VOLTAGE_REFERENCE_ENABLE == _adc->voltage_reference){
        ADC_ENABLE_VOLTAGE_REFERENCE();
    }
    else if(ADC_VOLTAGE_REFERENCE_DISABLE == _adc->voltage_reference){
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
    else{
        ADC_DISABLE_VOLTAGE_REFERENCE();        // default
    }
}

void ADC_ISR(void){
    ADC_InterruptFlagClear();
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
}