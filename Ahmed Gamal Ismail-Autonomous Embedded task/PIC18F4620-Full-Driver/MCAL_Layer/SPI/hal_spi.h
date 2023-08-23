/* 
 * File:   hal_spi.h
 * Author: user
 *
 * Created on August 2, 2023, 9:05 PM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations */
#define SPI_MASTER_MODE_DATA_SAMPLED_AT_END         1
#define SPI_MASTER_MODE_DATA_SAMPLED_AT_MIDDLE      0
#define SPI_SLAVE_MODE_DATA_SAMPLE_BIT_CLEAR        0

#define SPI_TRANSMIT_OCCURS_AT_RISING_EDGES         0
#define SPI_TRANSMIT_OCCURS_AT_FALLING_EDGES        1

#define SPI_RECEIVE_COMPLETE        1
#define SPI_RECEIVE_NOT_COMPLETE    0

#define SPI_COLLISION_DETECTED      1
#define SPI_NO_COLLISION            0

#define SPI_SLAVE_MODE_OVERFOLW_DETECTED    1
#define SPI_SLAVE_MODE_NO_OVERFOLW          0

#define SPI_MODULE_ENABLE     1
#define SPI_MODULE_DISABLE    0

#define SPI_CLOCK_IDLE_HIGH     1
#define SPI_CLOCK_IDLE_LOW      0

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */
typedef enum{
    SPI_MASTER_MODE_FOSC_DIV_BY_4 = 0,
    SPI_MASTER_MODE_FOSC_DIV_BY_16,
    SPI_MASTER_MODE_FOSC_DIV_BY_64,
    SPI_MASTER_MODE_FOSC_TMR2_OUTPUT_DIV_BY_4,
    SPI_SLAVE_MODE_SLAVE_SELECTION_PIN_ENABLED,
    SPI_SLAVE_MODE_SLAVE_SELECTION_PIN_DISABLED        
}spi_mode_select_t;

typedef struct{
#if SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void (* SPI_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    spi_mode_select_t spi_mode;
    uint8 spi_sample_location : 1;
    uint8 spi_transmit_edge : 1;
    uint8 spi_clock_polarity : 1;
    uint8 spi_reserved : 5;
}spi_t;

/* Section : Function Declarations */
Std_ReturnType SPI_Init(const spi_t *_spi);
Std_ReturnType SPI_DeInit(const spi_t *_spi);

Std_ReturnType SPI_Send_Byte_With_Blocking(const spi_t *_spi, uint8 _data);
Std_ReturnType SPI_Read_Byte_With_Blocking(const spi_t *_spi, uint8 *_data);

Std_ReturnType SPI_Send_Byte_WithOut_Blocking(const spi_t *_spi, uint8 _data);
Std_ReturnType SPI_Read_Byte_WithOut_Blocking(const spi_t *_spi, uint8 *_data);

#endif	/* HAL_SPI_H */

