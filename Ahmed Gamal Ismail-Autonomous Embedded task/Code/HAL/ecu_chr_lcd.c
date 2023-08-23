/* 
 * File:   ecu_chr_lcd.c
 * Author: user
 *
 * Created on July 15, 2023, 9:08 PM
 */

#include "ecu_chr_lcd.h"

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd, uint8 _command);
static Std_ReturnType lcd_4bits_send_enable_signal(const chr_lcd_4bit_t *lcd);
static Std_ReturnType lcd_4bits_set_cursor(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column);
static Std_ReturnType lcd_8bits_send_enable_signal(const chr_lcd_8bit_t *lcd);
static Std_ReturnType lcd_8bits_set_cursor(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column);

/**************************** 4bit LCD Interfaces ****************************/
/**
 * 
 * @param lcd
 * @return 
 */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(lcd->lcd_rs));
        ret = gpio_pin_intialize(&(lcd->lcd_en));
        for(l_data_pins_counter = ZERO_INIT; l_data_pins_counter < CHR_LCD_4BITS_PINS; l_data_pins_counter++){
            ret = gpio_pin_intialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
        __delay_ms(FIRST_INITIALIZATION_DELAY_MS);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(SECOND_INITIALIZATION_DELAY_MS);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_us(THIRD_INITIALIZATION_DELAY_US);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd, _LCD_CLEAR);
        ret = lcd_4bit_send_command(lcd, _LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(lcd, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_4bit_send_command(lcd, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_4bit_send_command(lcd, _LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd, CURSOR_FIRST_ROW_FIRST_COLUMN_LOCATION);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param command
 * @return 
 */
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd, uint8 command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        ret = lcd_send_4bits(lcd, command >> CHR_LCD_4BITS_PINS);  // send the 4 high bits
        ret = lcd_4bits_send_enable_signal(lcd);
        ret = lcd_send_4bits(lcd, command);                        // send the 4 low bits
        ret = lcd_4bits_send_enable_signal(lcd);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param data
 * @return 
 */
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        ret = lcd_send_4bits(lcd, data >> CHR_LCD_4BITS_PINS);  // send the 4 high bits
        ret = lcd_4bits_send_enable_signal(lcd);
        ret = lcd_send_4bits(lcd, data);                        // send the 4 low bits
        ret = lcd_4bits_send_enable_signal(lcd);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return 
 */
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bits_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_char_data(lcd, data);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param str
 * @return 
 */
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_4bit_send_char_data(lcd, *(str++));
        }
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return 
 */
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bits_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_string(lcd, str);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return 
 */
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column, const uint8 _chr[], uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 l_lcd_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_send_command(lcd, (_LCD_CGRAM_START + (8*mem_pos)));
        for(l_lcd_counter = ZERO_INIT; l_lcd_counter < CHR_LCD_8BITS_PINS; l_lcd_counter++){
            ret = lcd_4bit_send_char_data(lcd, _chr[l_lcd_counter]);
        }
        ret = lcd_4bit_send_char_data_pos(lcd, row, column, mem_pos);
    }
    return ret;
}

/**************************** 8bit LCD Interfaces ****************************/
/**
 * 
 * @param lcd
 * @return 
 */
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(lcd->lcd_rs));
        ret = gpio_pin_intialize(&(lcd->lcd_en));
        for(l_data_pins_counter = ZERO_INIT; l_data_pins_counter < CHR_LCD_8BITS_PINS; l_data_pins_counter++){
            ret = gpio_pin_intialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
        __delay_ms(FIRST_INITIALIZATION_DELAY_MS);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(SECOND_INITIALIZATION_DELAY_MS);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_us(THIRD_INITIALIZATION_DELAY_US);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd, _LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd, _LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_8bit_send_command(lcd, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd, CURSOR_FIRST_ROW_FIRST_COLUMN_LOCATION);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param command
 * @return 
 */
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd, uint8 command){
    Std_ReturnType ret = E_OK;
    uint8 l_8bits_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        for(l_8bits_counter = ZERO_INIT; l_8bits_counter < CHR_LCD_8BITS_PINS; l_8bits_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_8bits_counter]), (command >> l_8bits_counter) & (FIRST_BIT_MUSK));
        }
        ret = lcd_8bits_send_enable_signal(lcd);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param data
 * @return 
 */
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd, uint8 data){
    Std_ReturnType ret = E_OK;
    uint8 l_8bits_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        for(l_8bits_counter = ZERO_INIT; l_8bits_counter < CHR_LCD_8BITS_PINS; l_8bits_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_8bits_counter]), (data >> l_8bits_counter) & (FIRST_BIT_MUSK));
        }
        ret = lcd_8bits_send_enable_signal(lcd);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return 
 */
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bits_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_char_data(lcd, data);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param str
 * @return 
 */
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_8bit_send_char_data(lcd, *(str++));
        }
        /*
        uint8 l_data_counter = ZERO_INIT;
        for(l_data_counter = 0; *(str + l_data_counter) != '\0'; l_data_counter++){
            ret = lcd_8bit_send_char_data(lcd, *(str + l_data_counter));
        }
        */
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return 
 */
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bits_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_string(lcd, str);
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return 
 */
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column, const uint8 _chr[], uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 l_lcd_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_send_command(lcd, (_LCD_CGRAM_START + (8*mem_pos)));
        for(l_lcd_counter = ZERO_INIT; l_lcd_counter < CHR_LCD_8BITS_PINS; l_lcd_counter++){
            ret = lcd_8bit_send_char_data(lcd, _chr[l_lcd_counter]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd, row, column, mem_pos);
    }
    return ret;
}

/**************************** Other Functions ****************************/
/**
 * 
 * @param value
 * @param str
 */
Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', UINT8_ARRAY_ELEMENTS);
        sprintf((char *)str, "%i", value);
    }
    return ret;
}

/**
 * 
 * @param value
 * @param str
 */
Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', UINT16_ARRAY_ELEMENTS);
        sprintf((char *)str, "%i", value);
        // My Edit:
        for(int counter = 0; counter < 5; counter++){
            if('\0' == str[counter]){
                str[counter] = ' ';
            }
        }
    }
    return ret;
}

/**
 * 
 * @param value
 * @param str
 */
Std_ReturnType convert_uint32_to_string(uint32 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', UINT32_ARRAY_ELEMENTS);
        sprintf((char *)str, "%i", (int)value);
    }
    return ret;
}

/**************************** Static Local Functions ****************************/
/**
 * 
 * @param lcd
 * @param _data_command
 * @return 
 */
static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd, uint8 _command){
    Std_ReturnType ret = E_OK;
    uint8 l_4bits_counter = ZERO_INIT;
    for(l_4bits_counter = ZERO_INIT; l_4bits_counter < CHR_LCD_4BITS_PINS; l_4bits_counter++){
        ret = gpio_pin_write_logic(&(lcd->lcd_data[l_4bits_counter]), (_command >> l_4bits_counter) & (FIRST_BIT_MUSK));
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @return 
 */
static Std_ReturnType lcd_4bits_send_enable_signal(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(ENABLE_SIGNAL_DELAY_US);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @return 
 */
static Std_ReturnType lcd_4bits_set_cursor(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column){
    Std_ReturnType ret = E_OK;
    column--;
    switch(row){
        case ROW1 : ret = lcd_4bit_send_command(lcd, (CURSOR_FIRST_ROW_FIRST_COLUMN_LOCATION  + column)); break;
        case ROW2 : ret = lcd_4bit_send_command(lcd, (CURSOR_SECOND_ROW_FIRST_COLUMN_LOCATION + column)); break;
        case ROW3 : ret = lcd_4bit_send_command(lcd, (CURSOR_THIRD_ROW_FIRST_COLUMN_LOCATION  + column)); break;
        case ROW4 : ret = lcd_4bit_send_command(lcd, (CURSOR_FOURTH_ROW_FIRST_COLUMN_LOCATION + column)); break;
        default : ret = E_NOT_OK; break;
    }
    return ret;
}

/**
 * 
 * @param lcd
 * @return 
 */
static Std_ReturnType lcd_8bits_send_enable_signal(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(ENABLE_SIGNAL_DELAY_US);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

/**
 * 
 * @param lcd
 * @param row
 * @param column
 * @return 
 */
static Std_ReturnType lcd_8bits_set_cursor(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column){
    Std_ReturnType ret = E_OK;
    column--;
    switch(row){
        case ROW1 : ret = lcd_8bit_send_command(lcd, (CURSOR_FIRST_ROW_FIRST_COLUMN_LOCATION  + column)); break;
        case ROW2 : ret = lcd_8bit_send_command(lcd, (CURSOR_SECOND_ROW_FIRST_COLUMN_LOCATION + column)); break;
        case ROW3 : ret = lcd_8bit_send_command(lcd, (CURSOR_THIRD_ROW_FIRST_COLUMN_LOCATION  + column)); break;
        case ROW4 : ret = lcd_8bit_send_command(lcd, (CURSOR_FOURTH_ROW_FIRST_COLUMN_LOCATION + column)); break;
        default : ret = E_NOT_OK; break;
    }
    return ret;
}