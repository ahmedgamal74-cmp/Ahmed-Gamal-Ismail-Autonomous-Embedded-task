/* 
 * File:   application.c
 * Author: user
 *
 * Created on June 24, 2023, 3:39 PM
 */

#include "application.h"

#define FRAME_BYTES     8

/* USART Interrupt Handlers */
void Tx_int_func(void);
void Rx_int_func(void);
void Overrun_error_int_solution_func(void);
void Framing_error_int_solution_func(void);

/* TIMER0 Interrupt Handler */
void TMR0_int_handler(void);

/* This functions used to show the clock */
void Clock_write_digits(void);

/* An array contains the frame */
uint8 my_data[FRAME_BYTES];

/* The 6 digits of the Clock, volatile as they will be incremented in the interrupt handler */
volatile uint8 hours_tens = 0;
volatile uint8 hours_ones = 0;
volatile uint8 mintues_tens = 0;
volatile uint8 mintues_ones = 0;
volatile uint8 senconds_tens = 0;
volatile uint8 senconds_ones = 0;

/* The 6 digits of the Clock converted to string to print them */
uint8 hours_tens_conv = '0';
uint8 hours_ones_conv = '0';
uint8 mintues_tens_conv = '0';
uint8 mintues_ones_conv = '0';
uint8 senconds_tens_conv = '0';
uint8 senconds_ones_conv = '0';

/* The initialization of The USART Module Object */
usart_t usart_obj = {
    .baudrate = 9600,
    .baudrate_gen_config = BAUDRATE_ASYNC_8BIT_LOW_SPEED,
    .usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_ENABLE,
    .usart_tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE,
    .usart_tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE,
    .usart_rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUS_RX_ENABLE,
    .usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE,
    .usart_rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE,
    .EUSART_FramingErrorHandler = Framing_error_int_solution_func,
    .EUSART_OverrunErrorHandler = Overrun_error_int_solution_func,
    .EUSART_RxDefaultInterruptHandler = Rx_int_func,
    .EUSART_TxDefaultInterruptHandler = Tx_int_func
};

/* The initialization of The LCD Module Object */
chr_lcd_8bit_t lcd_obj = {
    .lcd_rs.port = PORTC_INDEX, .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT, .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTC_INDEX, .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT, .lcd_en.logic = GPIO_LOW,
    
    .lcd_data[0].port = PORTD_INDEX, .lcd_data[0].pin = GPIO_PIN0,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[1].port = PORTD_INDEX, .lcd_data[1].pin = GPIO_PIN1,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[2].port = PORTD_INDEX, .lcd_data[2].pin = GPIO_PIN2,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[3].port = PORTD_INDEX, .lcd_data[3].pin = GPIO_PIN3,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[3].logic = GPIO_LOW,
    .lcd_data[4].port = PORTD_INDEX, .lcd_data[4].pin = GPIO_PIN4,
    .lcd_data[4].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[4].logic = GPIO_LOW,
    .lcd_data[5].port = PORTD_INDEX, .lcd_data[5].pin = GPIO_PIN5,
    .lcd_data[5].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[5].logic = GPIO_LOW,
    .lcd_data[6].port = PORTD_INDEX, .lcd_data[6].pin = GPIO_PIN6,
    .lcd_data[6].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[6].logic = GPIO_LOW,
    .lcd_data[7].port = PORTD_INDEX, .lcd_data[7].pin = GPIO_PIN7,
    .lcd_data[7].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[7].logic = GPIO_LOW
};

/* The initialization of The TIMER0 Module Object */
timer0_t tmr0_obj = {
    .TMR0_InterruptHandler = TMR0_int_handler,
    .prescaler_enable = TIMER0_PRESCAlER_ENABLE_CFG,
    .prescaler_value = TIMER0_PRESCALER_DEV_BY_32,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .timer0_counter_edge = TIMER0_RISING_EDGE_CFG,
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_preload_value = 3036,
    .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
};

/* The implementation of the Transmitter Interrupt Handler */
void Tx_int_func(void){
    /* Nothing ( Not Used ) */
}

/* Receiver counter to the 8 bytes inputs */
volatile uint8 Rx_counter = 0;

/* The implementation of the Receiver Interrupt Handler */
void Rx_int_func(void){
    Std_ReturnType ret = E_NOT_OK;
    
    ret = EUSART_ASYNC_ReadByte_WithOutBlocking(&my_data[Rx_counter]);
    
    Rx_counter++;
    if((8 == Rx_counter)){
        Rx_counter = 0;
        
        /* To ensure that the angle wont exceed 45 */
        if(('4' < my_data[4]) || (('5' < my_data[5]) && ('4' == my_data[4]))){
            return;
        }
        else{}
        
        /* To ensure that the last byte in frame is E */
        if('E' != my_data[7]){
            return;
        }
        else{}
        
        /* To ensure that the speed wont exceed 100% */
        if((('1' == my_data[0]) && ('0' != my_data[1])) || (('1' == my_data[0]) && ('0' != my_data[2])) || ('1' < my_data[0])){
            return;
        }
        else{}
        
        ret = lcd_8bit_send_string_pos(&lcd_obj, 1, 7, "          ");
        ret = lcd_8bit_send_string_pos(&lcd_obj, 2, 11, "          ");
        
        
        ret = lcd_8bit_send_char_data_pos(&lcd_obj, 1, 7, ' ');
        if(('0' == my_data[0]) && ('0' != my_data[1])){
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[1]);
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[2]);
        }
        else if(('0' == my_data[0]) && ('0' == my_data[1])){
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[2]);
        }
        else{
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[0]);
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[1]);
            ret = lcd_8bit_send_char_data(&lcd_obj, my_data[2]);
        }
        ret = lcd_8bit_send_char_data(&lcd_obj, '%');
        
        
        ret = lcd_8bit_send_char_data_pos(&lcd_obj, 2, 11, ' ');
        if('L' == my_data[6]){
            ret = lcd_8bit_send_char_data(&lcd_obj, '-');
        }
        else if('R' == my_data[6]){
            ret = lcd_8bit_send_char_data(&lcd_obj, '+');
        }
        else{}
        ret = lcd_8bit_send_char_data(&lcd_obj, my_data[4]);
        ret = lcd_8bit_send_char_data(&lcd_obj, my_data[5]);
        
    }
    else{}
}

/* The implementation of the Overrun Interrupt Handler */
void Overrun_error_int_solution_func(void){
    EUSART_ASYNC_Rx_Restart();
}

/* The implementation of the Framing error Interrupt Handler */
void Framing_error_int_solution_func(void){
    uint8 framing_data = 0;
    EUSART_ASYNC_ReadByte_WithOutBlocking(&framing_data);
}

/* A flag to change the clock time from AM to PM */
volatile uint8 AM_PM_flag = 0;

/* The implementation of the Timer Interrupt Handler */
void TMR0_int_handler(void){
    Std_ReturnType ret = E_NOT_OK;
    
    senconds_ones++;
    
    if(10 == senconds_ones){
        senconds_ones = 0;
        senconds_tens++;
    }
    else{}
    
    if(6 == senconds_tens){
        senconds_tens = 0;
        mintues_ones++;
    }
    else{}
    
    if(10 == mintues_ones){
        mintues_ones = 0;
        mintues_tens++;
    }
    else{}
    
    if(6 == mintues_tens){
        mintues_tens = 0;
        hours_ones++;
    }
    else{}
    
    if(10 == hours_ones){
        hours_ones = 0;
        hours_tens++;
    }
    else{}
    
    if((1 == hours_tens) && (2 == hours_ones)){
        hours_tens = 0;
        hours_ones = 0;
        AM_PM_flag++;
        if(1 == AM_PM_flag){
            ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 16, 'P');
        }
        else if(2 == AM_PM_flag){
            AM_PM_flag = 0;
            ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 16, 'A');
        }
        else{}
    }
    else{}
    
    Clock_write_digits();
}

/* The implementation of the Clock Function */
void Clock_write_digits(void){
    Std_ReturnType ret = E_NOT_OK;
    
    ret = lcd_8bit_send_string_pos(&lcd_obj, 3, 1, "Time: ");
        
    ret = convert_uint8_to_string(hours_tens, &hours_tens_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 7, hours_tens_conv);
        
    ret = convert_uint8_to_string(hours_ones, &hours_ones_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 8, hours_ones_conv);
        
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 9, ':');
        
    ret = convert_uint8_to_string(mintues_tens, &mintues_tens_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 10, mintues_tens_conv);
        
    ret = convert_uint8_to_string(mintues_ones, &mintues_ones_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 11, mintues_ones_conv);
        
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 12, ':');
        
    ret = convert_uint8_to_string(senconds_tens, &senconds_tens_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 13, senconds_tens_conv);
        
    ret = convert_uint8_to_string(senconds_ones, &senconds_ones_conv);
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 14, senconds_ones_conv);
        
    
}

int main() {
    Std_ReturnType ret = E_NOT_OK;
    application_initialize();
    
    ret = EUSART_ASYNC_Init(&usart_obj);
    ret = lcd_8bit_initialize(&lcd_obj);
    ret = TIMER0_Init(&tmr0_obj);
    
    ret = lcd_8bit_send_string_pos(&lcd_obj, 1, 1, "Speed: ");
    ret = lcd_8bit_send_string_pos(&lcd_obj, 2, 1, "Direction: ");
    Clock_write_digits();
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 16, 'A');
    ret = lcd_8bit_send_char_data_pos(&lcd_obj, 3, 17, 'M');
    ret = lcd_8bit_send_string_pos(&lcd_obj, 4, 1, "====================");
    
    while(1){
        //__delay_ms(000);
        
        
        
    }

    return (EXIT_SUCCESS);
}

void application_initialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_initialize();
}