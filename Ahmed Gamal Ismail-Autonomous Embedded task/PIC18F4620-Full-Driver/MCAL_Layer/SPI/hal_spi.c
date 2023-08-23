/* 
 * File:   hal_spi.c
 * Author: user
 *
 * Created on August 2, 2023, 9:05 PM
 */

#include "hal_spi.h"

#if SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (* SPI_InterruptHandler)(void) = NULL;
#endif

static Std_ReturnType SPI_Master_Mode_Pins_Config(const spi_t *_spi);
static Std_ReturnType SPI_Slave_Mode_Pins_Config(const spi_t *_spi);

/**
 * 
 * @param _spi
 * @return 
 */
Std_ReturnType SPI_Init(const spi_t *_spi){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the module */
        SSPCON1bits.SSPEN = SPI_MODULE_DISABLE;
        /* Select the mode */
        SSPCON1bits.SSPM = _spi->spi_mode;
        /* Pin Configuration */
        if((SPI_MASTER_MODE_FOSC_DIV_BY_4 == _spi->spi_mode) || (SPI_MASTER_MODE_FOSC_DIV_BY_16 == _spi->spi_mode) || 
            (SPI_MASTER_MODE_FOSC_DIV_BY_64 == _spi->spi_mode) ||
            (SPI_MASTER_MODE_FOSC_TMR2_OUTPUT_DIV_BY_4 == _spi->spi_mode)){
            ret = SPI_Master_Mode_Pins_Config(_spi);
            SSPSTATbits.SMP = _spi->spi_sample_location;
        }
        else if((SPI_SLAVE_MODE_SLAVE_SELECTION_PIN_ENABLED == _spi->spi_mode) ||
                (SPI_SLAVE_MODE_SLAVE_SELECTION_PIN_DISABLED == _spi->spi_mode)){
            ret = SPI_Slave_Mode_Pins_Config(_spi);
            SSPSTATbits.SMP = SPI_SLAVE_MODE_DATA_SAMPLE_BIT_CLEAR;
        }
        else{/**/}
        /* Set Transmit Edge */
        SSPSTATbits.CKE = _spi->spi_transmit_edge;
        /* Set Clock Polarity */
        SSPCON1bits.CKP = _spi->spi_clock_polarity;
        /* Interrupt */
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
            EUSART_RX_InterruptEnable();
            SPI_InterruptHandler = _spi->SPI_InterruptHandler;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(INTERRUPT_HIGH_PRIORITY == _spi->priority){
                INTERRUPT_GlobalInterruptHighEnable();
                SPI_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == _spi->priority){
                INTERRUPT_GlobalInterruptLowEnable();
                SPI_LowPrioritySet();
            }
            else{ /* Nothing */ }
#endif

#endif
        /* Enable the module */
        SSPCON1bits.SSPEN = SPI_MODULE_ENABLE;
    }
    return ret;
}

/**
 * 
 * @param _spi
 * @return 
 */
Std_ReturnType SPI_DeInit(const spi_t *_spi){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }
    else{
        SSPCON1bits.SSPEN = SPI_MODULE_DISABLE;
#if SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        SPI_InterruptDisable();
#endif
    }
    return ret;
}

/**
 * 
 * @param _spi
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Send_Byte_With_Blocking(const spi_t *_spi, uint8 _data){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }
    else{
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF));
        SPI_InterruptFlagClear();
    }
    return ret;
}

/**
 * 
 * @param _spi
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Read_Byte_With_Blocking(const spi_t *_spi, uint8 *_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _spi) || (NULL == _data)){
        ret = E_NOT_OK;
    }
    else{
        while(!(SSPSTATbits.BF));
        *_data = SSPBUF;
    }
    return ret;
}

/**
 * 
 * @param _spi
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Send_Byte_WithOut_Blocking(const spi_t *_spi, uint8 _data){
    Std_ReturnType ret = E_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }
    else{
        
    }
    return ret;
}

/**
 * 
 * @param _spi
 * @param _data
 * @return 
 */
Std_ReturnType SPI_Read_Byte_WithOut_Blocking(const spi_t *_spi, uint8 *_data){
    Std_ReturnType ret = E_OK;
    if((NULL == _spi) || (NULL == _data)){
        ret = E_NOT_OK;
    }
    else{
        
    }
    return ret;
}

void SPI_ISR(void){
    //SPI_InterruptFlagClear(); 
    
    if(SPI_InterruptHandler){
        SPI_InterruptHandler();
    }
    else{}
}
/****************************** Helper Functions ******************************/
static Std_ReturnType SPI_Master_Mode_Pins_Config(const spi_t *_spi){
    Std_ReturnType ret = E_OK;
    
    pin_config_t SPI_Master_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_Master_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_Master_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_OUTPUT};
    
    ret = gpio_pin_direction_intialize(&SPI_Master_SDO);
    ret &= gpio_pin_direction_intialize(&SPI_Master_SDI);
    ret &= gpio_pin_direction_intialize(&SPI_Master_CLK);
    
    return ret;
}

static Std_ReturnType SPI_Slave_Mode_Pins_Config(const spi_t *_spi){
    Std_ReturnType ret = E_OK;
    
    pin_config_t SPI_Master_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_Master_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_Master_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_Master_SS = {.port = PORTA_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_INPUT};
    
    ret = gpio_pin_direction_intialize(&SPI_Master_SDO);
    ret &= gpio_pin_direction_intialize(&SPI_Master_SDI);
    ret &= gpio_pin_direction_intialize(&SPI_Master_CLK);
    
    if(SPI_SLAVE_MODE_SLAVE_SELECTION_PIN_ENABLED == _spi->spi_mode){
        ret &= gpio_pin_direction_intialize(&SPI_Master_SS);
    }
    
    return ret;
}