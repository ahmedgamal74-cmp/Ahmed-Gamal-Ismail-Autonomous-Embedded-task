/* 
 * File:   hal_i2c.c
 * Author: user
 *
 * Created on August 10, 2023, 6:12 PM
 */

#include "hal_i2c.h"

#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void (* I2C_InterruptHandler)(void) = NULL;
static void (* I2C_Report_Write_Collision)(void) = NULL;
static void (* I2C_Report_Receive_Overflow)(void) = NULL;
#endif

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Init(const i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the Module */
        MSSP_MODULE_DISABLE();
        /* Mode Select */
        if(i2c_obj->i2c_cfg.i2c_mode_MAorSL == I2C_MASTER_MODE){
            /* Mode Configure */
            SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
            /* Clock Configure */
            SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c_obj->i2c_cfg.i2c_clock) - 1);
        }
        else if(i2c_obj->i2c_cfg.i2c_mode_MAorSL == I2C_SLAVE_MODE){
            /* General Call Configure */
            if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_cfg.i2c_general_call_en){
                I2C_GENERAL_CALL_ENABLE_CFG();
            }
            else if(I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_cfg.i2c_general_call_en){
                I2C_GENERAL_CALL_DISABLE_CFG();
            }
            else{}
            /* Clear Collision bit */
            SSPCON1bits.WCOL = 0;
            /* Clear overflow bit */
            SSPCON1bits.SSPOV = 0;
            /* Release clock */
            SSPCON1bits.CKP = 1;
            /* Assign Slave Address */
            SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
            /* Mode Configure */
            SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
        }
        else{}
        /* GPIO Configure */
        TRISCbits.TRISC3 = 1;
        TRISCbits.TRISC4 = 1;
        /* Slew rate */
        if(I2C_SLEW_RATE_ENABLE == i2c_obj->i2c_cfg.i2c_slew_rate_en){
            I2C_SLEW_RATE_ENABLE_CFG();
        }
        else if(I2C_SLEW_RATE_DISABLE == i2c_obj->i2c_cfg.i2c_slew_rate_en){
            I2C_SLEW_RATE_DISABLE_CFG();
        }
        else{}
        /* SMBus Control */
        if(I2C_SMBus_ENABLE == i2c_obj->i2c_cfg.i2c_SMBus_control_en){
            I2C_SMBus_ENABLE_CFG();
        }
        else if(I2C_SMBus_DISABLE == i2c_obj->i2c_cfg.i2c_SMBus_control_en){
            I2C_SMBus_DISABLE_CFG();
        }
        else{}
        /* Interrupts Configuration */
#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE!=INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        I2C_InterruptEnable();
        I2C_InterruptFlagClear();
        I2C_BUS_COLLISION_InterruptEnable();
        I2C_BUS_COLLISION_InterruptFlagClear();
        I2C_InterruptHandler = i2c_obj->I2C_InterruptHandler;
        I2C_Report_Write_Collision = i2c_obj->I2C_Report_Write_Collision;
        I2C_Report_Receive_Overflow = i2c_obj->I2C_Report_Receive_Overflow;
        
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.i2c_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            I2C_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.i2c_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            I2C_LowPrioritySet();
        }
        else{ /* Nothing */ }
        
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.i2c_bus_collision_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            I2C_BUS_COLLISION_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.i2c_bus_collision_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            I2C_BUS_COLLISION_LowPrioritySet();
        }
        else{ /* Nothing */ }
#endif        
        
#endif
        /* Enable the Module */
        MSSP_MODULE_ENABLE();
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_DeInit(const i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the Module */
        MSSP_MODULE_DISABLE();
        /* Disable interrupts */
#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        I2C_InterruptDisable();
        I2C_BUS_COLLISION_InterruptDisable();
#endif
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Master_Send_Start(const i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        SSPCON2bits.SEN = 1;        // Initiate start condition
        while(SSPCON2bits.SEN);     // Wait to send start condition
        PIR1bits.SSPIF = 0;         // Clear the flag
        if(I2C_START_BIT_NOT_DETECTED == SSPSTATbits.S){
            ret = E_NOT_OK;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Master_Send_Repeated_Start(const i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        SSPCON2bits.RSEN = 1;       // Initiate start condition
        while(SSPCON2bits.RSEN);    // Wait to send start condition
        PIR1bits.SSPIF = 0;         // Clear the flag
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @return 
 */
Std_ReturnType I2C_Master_Send_Stop(const i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        SSPCON2bits.PEN = 1;        // Initiate stop condition
        while(SSPCON2bits.PEN);     // Wait to send stop condition
        PIR1bits.SSPIF = 0;         // Clear the flag
        if(I2C_STOP_BIT_NOT_DETECTED == SSPSTATbits.P){
            ret = E_NOT_OK;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @param i2c_data
 * @return 
 */
Std_ReturnType I2C_Master_Write_Data_WithBlocking(const i2c_t *i2c_obj, uint8 i2c_data, uint8 *_ack){
    Std_ReturnType ret = E_OK;
    if((NULL == i2c_obj) || (NULL == _ack)){
        ret = E_NOT_OK;
    }
    else{
        SSPBUF = (uint8)i2c_data;
        while(!PIR1bits.SSPIF);             // Wait for sending
        PIR1bits.SSPIF = 0;                 // Clear the flag
        if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *_ack = I2C_ACK_RECEIVED_FROM_SLAVE;
        }
        else if(I2C_ACK_NOT_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT){
            *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE;
        }
        else{}
    }
    return ret;
}

/**
 * 
 * @param i2c_obj
 * @param ack
 * @param i2c_data
 * @return 
 */
Std_ReturnType I2C_Master_Read_Data_WithBlocking(const i2c_t *i2c_obj, uint8 ack, uint8 *i2c_data){
    Std_ReturnType ret = E_OK;
    if((NULL == i2c_obj) || (NULL == i2c_data)){
        ret = E_NOT_OK;
    }
    else{
        I2C_MASTER_RECEIVE_ENABLE_CFG();        // Enable receiving master
        while(!SSPSTATbits.BF);                 // Wait for receiving
        *i2c_data = SSPBUF;
        if(I2C_MASTER_SEND_ACK == ack){
            SSPCON2bits.ACKDT = 0;
            SSPCON2bits.ACKEN = 1;
        }
        else if(I2C_MASTER_SEND_NOT_ACK == ack){
            SSPCON2bits.ACKDT = 1;
            SSPCON2bits.ACKEN = 1;
        }
        else{}
    }
    return ret;
}

void I2C_ISR(void){
#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    I2C_InterruptFlagClear();
    
    if(I2C_InterruptHandler){
        I2C_InterruptHandler();
    }
    else{}
#endif
}

void I2C_BC_ISR(void){
#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    I2C_BUS_COLLISION_InterruptFlagClear();
    
    if(I2C_Report_Write_Collision){
        I2C_Report_Write_Collision();
    }
    else{}
#endif
}
