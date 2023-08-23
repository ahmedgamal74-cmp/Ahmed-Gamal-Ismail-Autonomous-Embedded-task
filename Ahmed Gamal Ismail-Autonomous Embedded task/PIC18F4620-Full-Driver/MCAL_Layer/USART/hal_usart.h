/* 
 * File:   hal_usart.h
 * Author: user
 *
 * Created on July 30, 2023, 5:48 PM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "hal_usart_cfg.h"

/* Section : Macro Declarations  synchronous */ 
#define EUSART_MODULE_ENABLE                        1
#define EUSART_MODULE_DISABLE                       0
#define EUSART_SYNCHRONOUS_MODE                     1
#define EUSART_ASYNCHRONOUS_MODE                    0
#define EUSART_ASYNCHRONOUS_HIGH_SPEED              1
#define EUSART_ASYNCHRONOUS_LOW_SPEED               0
#define EUSART_16BIT_BAUDRATE_GEN                   1
#define EUSART_8BIT_BAUDRATE_GEN                    0

#define SPBRG_HIGH_SHIFT_BITS   8

// EUSART ASYNCHRONOUS TX Macros
#define EUSART_ASYNCHRONOUS_TX_ENABLE               1
#define EUSART_ASYNCHRONOUS_TX_DISABLE              0
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE     1
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE    0
#define EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE          1
#define EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE         0

// EUSART ASYNCHRONOUS RX Macros
#define EUSART_ASYNCHRONOUS_RX_ENABLE               1
#define EUSART_ASYNCHRONOUS_RX_DISABLE              0
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE     1
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE    0
#define EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE          1
#define EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE         0

// Errors Macros
#define EUSART_FRAMING_ERROR_DETECTED               1
#define EUSART_FRAMING_ERROR_CLEARED                0
#define EUSART_OVERRUN_ERROR_DETECTED               1
#define EUSART_OVERRUN_ERROR_CLEARED                0

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */
typedef enum{
    BAUDRATE_ASYNC_8BIT_LOW_SPEED = 0,
    BAUDRATE_ASYNC_8BIT_HIGH_SPEED,
    BAUDRATE_ASYNC_16BIT_LOW_SPEED,
    BAUDRATE_ASYNC_16BIT_HIGH_SPEED,
    BAUDRATE_SYNC_8BIT,
    BAUDRATE_SYNC_16BIT        
}baudrate_gen_t;

typedef struct{
    uint8 usart_tx_reserved : 5;
    uint8 usart_tx_enable : 1;
    uint8 usart_tx_interrupt_enable : 1;
    uint8 usart_tx_9bit_enable : 1;
    interrupt_priority_cfg usart_tx_intrrupt_priority;
}usart_tx_cfg_t;

typedef struct{
    uint8 usart_rx_reserved : 5;
    uint8 usart_rx_enable : 1;
    uint8 usart_rx_interrupt_enable : 1;
    uint8 usart_rx_9bit_enable : 1;
    interrupt_priority_cfg usart_rx_intrrupt_priority;
}usart_rx_cfg_t;

typedef union{
    struct{
        uint8 usart_error_reserved : 6;
        uint8 usart_ferr : 1;
        uint8 usart_oerr : 1;
    };
    uint8 status;
}usart_error_status_t;

typedef struct{
    uint32 baudrate;
    baudrate_gen_t baudrate_gen_config;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status_t usart_error_status;
    void(* EUSART_TxDefaultInterruptHandler)(void);
    void(* EUSART_RxDefaultInterruptHandler)(void);
    void(* EUSART_FramingErrorHandler)(void);
    void(* EUSART_OverrunErrorHandler)(void);
}usart_t;

/* Section : Function Declarations */
Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart);
Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart);

Std_ReturnType EUSART_ASYNC_ReadByte_WithBlocking(uint8 *_data);
Std_ReturnType EUSART_ASYNC_ReadByte_WithOutBlocking(uint8 *_data);

Std_ReturnType EUSART_ASYNC_WriteByte_WithBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteString_WithBlocking(uint8 *_data, uint16 str_length);

Std_ReturnType EUSART_ASYNC_WriteByte_WithOutBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteString_WithOutBlocking(uint8 *_data, uint16 str_length);

Std_ReturnType EUSART_ASYNC_Rx_Restart(void);

#endif	/* HAL_USART_H */
