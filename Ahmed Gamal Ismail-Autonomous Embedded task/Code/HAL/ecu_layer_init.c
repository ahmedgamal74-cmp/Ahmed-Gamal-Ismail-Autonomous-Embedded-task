/* 
 * File:   ecu_layer_init.c
 * Author: user
 *
 * Created on July 13, 2023, 11:35 PM
 */

#include "ecu_layer_init.h"

/*
pin_config_t hours_seg1_en = {.port = PORTD_INDEX, .pin = GPIO_PIN0, .direction = GPIO_DIRECTION_OUTPUT, .logic = GPIO_HIGH}; 

led_t led1 = {.port_name = PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};

button_t btn_high = {
    .button_pin.port = PORTC_INDEX,
    .button_pin.pin = GPIO_PIN7,
    .button_pin.direction = GPIO_DIRECTION_INPUT,
    .button_pin.logic = GPIO_LOW,
    .button_state = BUTTON_RELEASED,
    .button_connection = BUTTON_ACTIVE_HIGH
};

relay_t relay_1 = {
    .relay_port = PORTC_INDEX,
    .relay_pin = GPIO_PIN0,
    .relay_status = RELAY_OFF_STATUS
};
 
dc_motor_t dc_motor_1 = {
    .dc_motor_pin[0].port = PORTC_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN0,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].port = PORTC_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN1,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS
};
 
uint8 seg_common_anode_values[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8 seg_common_cathode_values[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
 
segment_t seg1 = {
    .segment_pins[0].port = PORTC_INDEX,
    .segment_pins[0].pin = GPIO_PIN0,
    .segment_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .segment_pins[0].logic = GPIO_LOW,
    .segment_pins[1].port = PORTC_INDEX,
    .segment_pins[1].pin = GPIO_PIN1,
    .segment_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .segment_pins[1].logic = GPIO_LOW,
    .segment_pins[2].port = PORTC_INDEX,
    .segment_pins[2].pin = GPIO_PIN2,
    .segment_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .segment_pins[2].logic = GPIO_LOW,
    .segment_pins[3].port = PORTC_INDEX,
    .segment_pins[3].pin = GPIO_PIN3,
    .segment_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .segment_pins[3].logic = GPIO_LOW,
    .segment_type = SEGMENT_COMMON_CATHODE
}; 
 
keypad_t keypad1 = {
    .keypad_row_pins[0].port = PORTC_INDEX, .keypad_row_pins[0].pin = GPIO_PIN0,
    .keypad_row_pins[0].direction = GPIO_DIRECTION_OUTPUT, .keypad_row_pins[0].logic = GPIO_LOW,
    .keypad_row_pins[1].port = PORTC_INDEX, .keypad_row_pins[1].pin = GPIO_PIN1,
    .keypad_row_pins[1].direction = GPIO_DIRECTION_OUTPUT, .keypad_row_pins[1].logic = GPIO_LOW,
    .keypad_row_pins[2].port = PORTC_INDEX, .keypad_row_pins[2].pin = GPIO_PIN2,
    .keypad_row_pins[2].direction = GPIO_DIRECTION_OUTPUT, .keypad_row_pins[2].logic = GPIO_LOW,
    .keypad_row_pins[3].port = PORTC_INDEX, .keypad_row_pins[3].pin = GPIO_PIN3,
    .keypad_row_pins[3].direction = GPIO_DIRECTION_OUTPUT, .keypad_row_pins[3].logic = GPIO_LOW,
    
    .keypad_column_pins[0].port = PORTC_INDEX, .keypad_column_pins[0].pin = GPIO_PIN4,
    .keypad_column_pins[0].direction = GPIO_DIRECTION_INPUT, .keypad_column_pins[0].logic = GPIO_LOW,
    .keypad_column_pins[1].port = PORTC_INDEX, .keypad_column_pins[1].pin = GPIO_PIN5,
    .keypad_column_pins[1].direction = GPIO_DIRECTION_INPUT, .keypad_column_pins[1].logic = GPIO_LOW,
    .keypad_column_pins[2].port = PORTC_INDEX, .keypad_column_pins[2].pin = GPIO_PIN6,
    .keypad_column_pins[2].direction = GPIO_DIRECTION_INPUT, .keypad_column_pins[2].logic = GPIO_LOW,
    .keypad_column_pins[3].port = PORTC_INDEX, .keypad_column_pins[3].pin = GPIO_PIN7,
    .keypad_column_pins[3].direction = GPIO_DIRECTION_INPUT, .keypad_column_pins[3].logic = GPIO_LOW
};
 
chr_lcd_4bit_t lcd_1 = {
    .lcd_rs.port = PORTC_INDEX, .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT, .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTC_INDEX, .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT, .lcd_en.logic = GPIO_LOW,
    
    .lcd_data[0].port = PORTC_INDEX, .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[1].port = PORTC_INDEX, .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[2].port = PORTC_INDEX, .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[3].port = PORTC_INDEX, .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT, .lcd_data[3].logic = GPIO_LOW
};

chr_lcd_8bit_t lcd_2 = {
    .lcd_rs.port = PORTC_INDEX, .lcd_rs.pin = GPIO_PIN6,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT, .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTC_INDEX, .lcd_en.pin = GPIO_PIN7,
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
 * 
 * 
 * ************************** INTx Interrupt ************************** 
void int0_app_isr(void){
    led_turn_toggle(&led1);
    __delay_ms(250);
}

void int1_app_isr(void){
    led_turn_toggle(&led2);
    __delay_ms(250);
}

void int2_app_isr(void){
    led_turn_toggle(&led3);
    __delay_ms(250);
}

interrupt_INTx_t int0_obj = {
    .EXT_InterruptHandler = int0_app_isr,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT0,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN0,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT
};

interrupt_INTx_t int1_obj = {
    .EXT_InterruptHandler = int1_app_isr,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_LOW_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT1,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN1,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT
};

interrupt_INTx_t int2_obj = {
    .EXT_InterruptHandler = int2_app_isr,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .source = INTERRUPT_EXTERNAL_INT2,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN2,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT
};
 * 
 * 
 * 
void rb4_int_app_isr_high(void){
    led_turn_on(&led1);
}
void rb4_int_app_isr_low(void){
    led_turn_on(&led2);
}

 interrupt_RBx_t rb4_int_obj = {
    .EXT_InterruptHandler_ChangedToHigh = rb4_int_app_isr_high,
    .EXT_InterruptHandler_ChangedToLow = rb4_int_app_isr_low,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN4,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT
};
 * 
 * 
 * 
adc_conf_t adc1 = {
    .ADC_IterruptHandler = adc_int_fun_1,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLE
};
 * 
 * 
 * 

timer0_t tmr0_timer = {
    .TMR0_InterruptHandler = tmr0_int_isr,
    .prescaler_enable = TIMER0_PRESCAlER_ENABLE_CFG,
    .prescaler_value = TIMER0_PRESCALER_DEV_BY_8,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .timer0_counter_edge = TIMER0_RISING_EDGE_CFG,
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_preload_value = 3036,
    .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
};

timer0_t tmr0_counter = {
    .TMR0_InterruptHandler = tmr0_int_isr,
    .prescaler_enable = TIMER0_PRESCAlER_DISABLE_CFG,
    .timer0_counter_edge = TIMER0_RISING_EDGE_CFG,
    .timer0_mode = TIMER0_COUNTER_MODE,
    .timer0_preload_value = 0,
    .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
};
 * 
 * 
 * 
void timer1_timer_init(void){
    timer1_t timer_obj = {
        .TMR1_InterruptHandler = timer1_callback,
        .priority = INTERRUPT_LOW_PRIORITY,
        .timer1_mode = TIMER1_TIMER_MODE,
        .timer1_reg_rw_mode = TIMER1_RW_REG_16BIT_MODE,
        .timer1_prescaler_value = TIMER1_PRESCALER_DEV_BY_4,
        .timer1_preload_value = 15536
    };
    TIMER1_Init(&timer_obj);
}

void timer1_counter_init(void){
    timer1_t counter_obj = {
        .TMR1_InterruptHandler = timer1_callback,
        .priority = INTERRUPT_LOW_PRIORITY,
        .timer1_mode = TIMER1_COUNTER_MODE,
        .timer1_counter_mode = TIMER1_SYNC_COUNTER_MODE,
        .timer1_reg_rw_mode = TIMER1_RW_REG_16BIT_MODE,
        .timer1_prescaler_value = TIMER1_PRESCALER_DEV_BY_1,
        .timer1_preload_value = 0
    };
    TIMER1_Init(&counter_obj);
}
 * 
 * 
 * 
timer3_t timer3_obj = {
    .TMR3_InterruptHandler = timer3_callback,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .timer3_prescaler_value = TIMER3_PRESCALER_DEV_BY_8,
    .timer3_preload_value = 15536,
    .timer3_mode = TIMER3_TIMER_MODE
};
 * 
 * 
ccp_t ccp_1 = {
    .ccp_inst = CCP1_INST,
    .CCP1_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECT,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.pin = GPIO_PIN2,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .ccp_pin.logic = GPIO_LOW,
    .timer2_postscaler_value = CCP_POSTSCALER_DEV_BY_1,
    .timer2_prescaler_value = CCP_PRESCALER_DEV_BY_1
};

ccp_t ccp_2 = {
    .ccp_inst = CCP2_INST,
    .CCP1_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECT,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.pin = GPIO_PIN1,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .ccp_pin.logic = GPIO_LOW,
    .timer2_postscaler_value = CCP_POSTSCALER_DEV_BY_1,
    .timer2_prescaler_value = CCP_PRESCALER_DEV_BY_1
};
 * 
 * 
 * 
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
 * 
 * 
spi_t spi ={
    .spi_mode = SPI_MASTER_MODE_FOSC_DIV_BY_4,
    .spi_clock_polarity = SPI_CLOCK_IDLE_HIGH,
    .spi_sample_location = SPI_MASTER_MODE_DATA_SAMPLED_AT_MIDDLE,
    .spi_transmit_edge = SPI_TRANSMIT_OCCURS_AT_RISING_EDGES
};

 * 
 * 
i2c_t i2c_master = {
    .i2c_cfg.i2c_mode_MAorSL = I2C_SLAVE_MODE,
    .i2c_cfg.i2c_mode_cfg = I2C_SLAVE_MODE_7BIT_ADDRESS,
    .i2c_cfg.i2c_SMBus_control_en = I2C_SMBus_DISABLE,
    .i2c_cfg.i2c_slew_rate_en = I2C_SLEW_RATE_DISABLE,
    .I2C_InterruptHandler = NULL,
    .I2C_Report_Receive_Overflow = NULL,
    .I2C_Report_Write_Collision = NULL
};
*/



void ecu_layer_initialize(void){
    Std_ReturnType ret = E_NOT_OK;
    
}