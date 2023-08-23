/* 
 * File:   hal_usart.c
 * Author: user
 *
 * Created on July 30, 2023, 5:48 PM
 */


#include "hal_usart.h"

#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (* EUSART_TxDefaultInterruptHandler)(void) = NULL;
#endif
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (* EUSART_RxDefaultInterruptHandler)(void) = NULL;
static void (* EUSART_FramingErrorInterruptHandler)(void) = NULL;
static void (* EUSART_OverrunErrorInterruptHandler)(void) = NULL;
#endif

static void EUSART_BaudRate_Calculations(const usart_t *_eusart);
static void EUSART_ASYNC_Tx_Init(const usart_t *_eusart);
static void EUSART_ASYNC_Rx_Init(const usart_t *_eusart);

/**
 * 
 * @param _eusart
 * @return 
 */
Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart){
    Std_ReturnType ret = E_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }
    else{
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        TRISCbits.RC6 = GPIO_HIGH;
        TRISCbits.RC7 = GPIO_HIGH;
        EUSART_BaudRate_Calculations(_eusart);
        EUSART_ASYNC_Tx_Init(_eusart);
        EUSART_ASYNC_Rx_Init(_eusart);
        RCSTAbits.SPEN = EUSART_MODULE_ENABLE;
    }
    return ret;
}

/**
 * 
 * @param _eusart
 * @return 
 */
Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart){
    Std_ReturnType ret = E_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }
    else{
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
    }
    return ret;
}

/**
 * 
 * @param _eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_ASYNC_ReadByte_WithBlocking(uint8 *_data){
    Std_ReturnType ret = E_OK;
    while(!PIR1bits.RCIF);
    *_data = RCREG;
    return ret;
}

/**
 * 
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_ASYNC_ReadByte_WithOutBlocking(uint8 *_data){
    Std_ReturnType ret = E_OK;
    if(1 == PIR1bits.RCIF){
        *_data = RCREG;
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @param _eusart
 * @param _data
 * @return 
 */
Std_ReturnType EUSART_ASYNC_WriteByte_WithBlocking(uint8 _data){
    Std_ReturnType ret = E_OK;
    while(!TXSTAbits.TRMT);
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
#endif
    TXREG = _data;
    return ret;
}

/**
 * 
 * @param _data
 * @param str_length
 * @return 
 */
Std_ReturnType EUSART_ASYNC_WriteString_WithBlocking(uint8 *_data, uint16 str_length){
    Std_ReturnType ret = E_OK;
    uint16 Char_Counter = ZERO_INIT;
    for(Char_Counter = ZERO_INIT; Char_Counter < str_length; Char_Counter++){
        ret = EUSART_ASYNC_WriteByte_WithBlocking(_data[Char_Counter]);
    }
    return ret;
}

/**
 * To solve OverRun Error
 * @return 
 */
Std_ReturnType EUSART_ASYNC_Rx_Restart(void){
    Std_ReturnType ret = E_OK;
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
    return ret;
}

void EUSART_TX_ISR(void){
    EUSART_TX_InterruptDisable();
    if(EUSART_TxDefaultInterruptHandler){
        EUSART_TxDefaultInterruptHandler();
    }
    else{}
}

void EUSART_RX_ISR(void){
    if(EUSART_RxDefaultInterruptHandler){
        EUSART_RxDefaultInterruptHandler();
    }
    else{}
    if(EUSART_FramingErrorInterruptHandler){
        EUSART_FramingErrorInterruptHandler();
    }
    else{}
    if(EUSART_OverrunErrorInterruptHandler){
        EUSART_OverrunErrorInterruptHandler();
    }
    else{}
}
/****************************** Helper Functions ******************************/
static void EUSART_BaudRate_Calculations(const usart_t *_eusart){
    float Baud_Rate_Temp = ZERO_INIT;
    switch(_eusart->baudrate_gen_config){
        case BAUDRATE_ASYNC_8BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 64) - 1;
            break;
        case BAUDRATE_ASYNC_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 16) - 1;
            break;
        case BAUDRATE_ASYNC_16BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 16) - 1;
            break;
        case BAUDRATE_ASYNC_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
            break;
        case BAUDRATE_SYNC_8BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
            break;
        case BAUDRATE_SYNC_16BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_Temp = ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
            break;
        default : ;
    }
    SPBRG = (uint8)((uint32)Baud_Rate_Temp);
    SPBRGH = (uint8)((uint32)Baud_Rate_Temp >> SPBRG_HIGH_SHIFT_BITS);
}

static void EUSART_ASYNC_Tx_Init(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_enable){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUS_TX_ENABLE;
        if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE;
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
        
            EUSART_TX_InterruptEnable();
            EUSART_TxDefaultInterruptHandler = _eusart->EUSART_TxDefaultInterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_tx_cfg.usart_tx_intrrupt_priority){
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_TX_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_tx_cfg.usart_tx_intrrupt_priority){
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_TX_LowPrioritySet();
            }
            else{ /* Nothing */ }
#endif

#endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
        }
        else{/**/}
    
        if(EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE;
        }
        else{/**/}
    }
    else if(EUSART_ASYNCHRONOUS_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_enable){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUS_TX_DISABLE;
    } 
    else{/**/}
}

static void EUSART_ASYNC_Rx_Init(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_enable){
        RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_ENABLE;
        if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE;
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
        
            EUSART_RX_InterruptEnable();
            EUSART_RxDefaultInterruptHandler = _eusart->EUSART_RxDefaultInterruptHandler;
            EUSART_FramingErrorInterruptHandler = _eusart->EUSART_FramingErrorHandler;
            EUSART_OverrunErrorInterruptHandler = _eusart->EUSART_OverrunErrorHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_rx_cfg.usart_rx_intrrupt_priority){
                INTERRUPT_GlobalInterruptHighEnable();
                EUSART_RX_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_rx_cfg.usart_rx_intrrupt_priority){
                INTERRUPT_GlobalInterruptLowEnable();
                EUSART_RX_LowPrioritySet();
            }
            else{ /* Nothing */ }
#endif

#endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
        }
        else{/**/}
    
        if(EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE;
        }
        else{/**/}
    }
    else if(EUSART_ASYNCHRONOUS_RX_DISABLE == _eusart->usart_tx_cfg.usart_tx_enable){
        RCSTAbits.CREN = EUSART_ASYNCHRONOUS_RX_DISABLE;
    } 
    else{/**/} 
}
