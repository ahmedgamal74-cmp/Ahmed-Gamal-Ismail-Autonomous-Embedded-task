/* 
 * File:   mcal_external_interrupt.c
 * Author: user
 *
 * Created on July 18, 2023, 10:15 PM
 */

#include "mcal_external_interrupt.h"

/* pointer to callback functions for INTx Interrupts */
static InterruptHandler INT0_InterruptHandler = NULL;               // static void (*INT0_InterruptHandler)(void) = NULL;
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;
/* pointer to callback functions for RBx Interrupts */
static InterruptHandler RB4_InterruptHandler_ChangedTOHigh = NULL;  // static void (*RB4_InterruptHandler_ChangedTOHigh)(void) = NULL;
static InterruptHandler RB4_InterruptHandler_ChangedTOLow = NULL;
static InterruptHandler RB5_InterruptHandler_ChangedTOHigh = NULL;
static InterruptHandler RB5_InterruptHandler_ChangedTOLow = NULL;
static InterruptHandler RB6_InterruptHandler_ChangedTOHigh = NULL;
static InterruptHandler RB6_InterruptHandler_ChangedTOLow = NULL;
static InterruptHandler RB7_InterruptHandler_ChangedTOHigh = NULL;
static InterruptHandler RB7_InterruptHandler_ChangedTOLow = NULL;

static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj);

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj);

static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t *int_obj);

/**
 * 
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the external interrupt */
        ret = Interrupt_INTx_Disable(int_obj);
        /* Clear external interrupt Flag */
        ret &= Interrupt_INTx_Clear_Flag(int_obj);
        /* Configure external interrupt edge */
        ret &= Interrupt_INTx_Edge_Init(int_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        /* Configure external interrupt priority */
        ret &= Interrupt_INTx_Priority_Init(int_obj);
#endif
        /* Configure external interrupt i/o pins */
        ret &= Interrupt_INTx_Pin_Init(int_obj);
        /* Configure external interrupt CallBack */
        ret &= Interrupt_INTx_SetInterruptHandler(int_obj);
        /* Enable the external interrupt */
        ret &= Interrupt_INTx_Enable(int_obj);
    }
    return ret;
}

/**
 * 
 */
void INT0_ISR(void){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_INT0_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(INT0_InterruptHandler){ INT0_InterruptHandler(); }
    else{ /* Nothing */ }
}

/**
 * 
 */
void INT1_ISR(void){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_INT1_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(INT1_InterruptHandler){ INT1_InterruptHandler(); }
    else{ /* Nothing */ }
}

/**
 * 
 */
void INT2_ISR(void){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_INT2_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(INT2_InterruptHandler){ INT2_InterruptHandler(); }
    else{ /* Nothing */ }
}

/**
 * 
 * @param RB4_Source
 */
void RB4_ISR(uint8 RB4_Source){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_RBx_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(RBx_ChangedToHigh == RB4_Source){
        if(RB4_InterruptHandler_ChangedTOHigh){ RB4_InterruptHandler_ChangedTOHigh(); }
        else{ /* Nothing */ }
    }
    else if(RBx_ChangedToLow == RB4_Source){
        if(RB4_InterruptHandler_ChangedTOLow){ RB4_InterruptHandler_ChangedTOLow(); }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
    
}

/**
 * 
 * @param RB5_Source
 */
void RB5_ISR(uint8 RB5_Source){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_RBx_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(RBx_ChangedToHigh == RB5_Source){
        if(RB5_InterruptHandler_ChangedTOHigh){ RB5_InterruptHandler_ChangedTOHigh(); }
        else{ /* Nothing */ }
    }
    else if(RBx_ChangedToLow == RB5_Source){
        if(RB5_InterruptHandler_ChangedTOLow){ RB5_InterruptHandler_ChangedTOLow(); }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
    
}

/**
 * 
 * @param RB6_Source
 */
void RB6_ISR(uint8 RB6_Source){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_RBx_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(RBx_ChangedToHigh == RB6_Source){
        if(RB6_InterruptHandler_ChangedTOHigh){ RB6_InterruptHandler_ChangedTOHigh(); }
        else{ /* Nothing */ }
    }
    else if(RBx_ChangedToLow == RB6_Source){
        if(RB6_InterruptHandler_ChangedTOLow){ RB6_InterruptHandler_ChangedTOLow(); }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
    
}

void RB7_ISR(uint8 RB7_Source){
    /* Clear the interrupt occurred bit (Flag) */
    EXT_RBx_InterruptFlagClear();
    /* Code */
    
    /* CallBack Function */
    if(RBx_ChangedToHigh == RB7_Source){
        if(RB7_InterruptHandler_ChangedTOHigh){ RB7_InterruptHandler_ChangedTOHigh(); }
        else{ /* Nothing */ }
    }
    else if(RBx_ChangedToLow == RB7_Source){
        if(RB7_InterruptHandler_ChangedTOLow){ RB7_InterruptHandler_ChangedTOLow(); }
        else{ /* Nothing */ }
    }
    else{ /* Nothing */ }
    
}

/**
 * 
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        ret = Interrupt_INTx_Disable(int_obj);
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the external interrupt RBx */
        EXT_RBx_InterruptDisable();
        /* Clear RBx Flag */
        EXT_RBx_InterruptFlagClear();
        /* Priority Configuration */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(int_obj->priority == INTERRUPT_LOW_PRIORITY){
            INTERRUPT_GlobalInterruptLowEnable();
            EXT_RBx_LowPrioritySet();
        }
        else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
            INTERRUPT_GlobalInterruptHighEnable();
            EXT_RBx_HighPrioritySet();
        }
        else{ /* Nothing */ }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        /* Pins Configuration */
        ret = gpio_pin_direction_intialize(&(int_obj->mcu_pin));
        /* CallBack Functions Configuration */
        switch(int_obj->mcu_pin.pin){
            case GPIO_PIN4:
                RB4_InterruptHandler_ChangedTOHigh = int_obj->EXT_InterruptHandler_ChangedToHigh;
                RB4_InterruptHandler_ChangedTOLow = int_obj->EXT_InterruptHandler_ChangedToLow;
                break;
            case GPIO_PIN5:
                RB5_InterruptHandler_ChangedTOHigh = int_obj->EXT_InterruptHandler_ChangedToHigh;
                RB5_InterruptHandler_ChangedTOLow = int_obj->EXT_InterruptHandler_ChangedToLow;
                break;
            case GPIO_PIN6:
                RB6_InterruptHandler_ChangedTOHigh = int_obj->EXT_InterruptHandler_ChangedToHigh;
                RB6_InterruptHandler_ChangedTOLow = int_obj->EXT_InterruptHandler_ChangedToLow;
                break;
            case GPIO_PIN7:
                RB7_InterruptHandler_ChangedTOHigh = int_obj->EXT_InterruptHandler_ChangedToHigh;
                RB7_InterruptHandler_ChangedTOLow = int_obj->EXT_InterruptHandler_ChangedToLow;
                break;
            default : ret = E_NOT_OK;
        }    
        /* Enable the external interrupt RBx */
        EXT_RBx_InterruptEnable();
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the external interrupt RBx */
        EXT_RBx_InterruptDisable();
        /* Clear RBx Flag */
        EXT_RBx_InterruptFlagClear();
    }
    return ret;
}

/************************** Static Helper Functions **************************/
/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT0 :
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_GlobalInterruptHighEnable();
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif            
            EXT_INT0_InterruptEnable();
            break;
        case INTERRUPT_EXTERNAL_INT1 :
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(int_obj->priority == INTERRUPT_LOW_PRIORITY){
                INTERRUPT_GlobalInterruptLowEnable();
            }
            else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                INTERRUPT_GlobalInterruptHighEnable();
            }
            else{ /* Nothing */ }
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
            EXT_INT1_InterruptEnable();
            break;
        case INTERRUPT_EXTERNAL_INT2 :
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelEnable();
            if(int_obj->priority == INTERRUPT_LOW_PRIORITY){
                INTERRUPT_GlobalInterruptLowEnable();
            }
            else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                INTERRUPT_GlobalInterruptHighEnable();
            }
            else{ /* Nothing */ }
#else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif
            EXT_INT2_InterruptEnable();
            break;
        default : ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT0 : EXT_INT0_InterruptDisable(); break;
        case INTERRUPT_EXTERNAL_INT1 : EXT_INT1_InterruptDisable(); break;
        case INTERRUPT_EXTERNAL_INT2 : EXT_INT2_InterruptDisable(); break;
        default : ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT1 :
            if(int_obj->priority == INTERRUPT_LOW_PRIORITY){ EXT_INT1_LowPrioritySet(); }
            else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){ EXT_INT1_HighPrioritySet(); }
            else{ /* Nothing */ }
            break;
        case INTERRUPT_EXTERNAL_INT2 :
            if(int_obj->priority == INTERRUPT_LOW_PRIORITY){ EXT_INT2_LowPrioritySet(); }
            else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){ EXT_INT2_HighPrioritySet(); }
            else{ /* Nothing */ }
            break;
        default : ret = E_NOT_OK;
    }
    return ret;
}
#endif

/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT0 :
            if(int_obj->edge == INTERRUPT_FALLING_EDGE){ EXT_INT0_FallingEdgeSet(); }
            else if(int_obj->edge == INTERRUPT_RISING_EDGE){ EXT_INT0_RisingEdgeSet(); }
            else{ /* Nothing */ }
            break;
        case INTERRUPT_EXTERNAL_INT1 :
            if(int_obj->edge == INTERRUPT_FALLING_EDGE){ EXT_INT1_FallingEdgeSet(); }
            else if(int_obj->edge == INTERRUPT_RISING_EDGE){ EXT_INT1_RisingEdgeSet(); }
            else{ /* Nothing */ }
            break;
        case INTERRUPT_EXTERNAL_INT2 :
            if(int_obj->edge == INTERRUPT_FALLING_EDGE){ EXT_INT2_FallingEdgeSet(); }
            else if(int_obj->edge == INTERRUPT_RISING_EDGE){ EXT_INT2_RisingEdgeSet(); }
            else{ /* Nothing */ }
            break;
        default : ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_direction_intialize(&(int_obj->mcu_pin));
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT0 : EXT_INT0_InterruptFlagClear(); break;
        case INTERRUPT_EXTERNAL_INT1 : EXT_INT1_InterruptFlagClear(); break;
        case INTERRUPT_EXTERNAL_INT2 : EXT_INT2_InterruptFlagClear(); break;
        default : ret = E_NOT_OK;
    }
    return ret;
}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT0_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT1_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * 
 * @param InterruptHandler
 * @return 
 */
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT2_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * 
 * @param int_obj
 * @return 
 */
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    switch(int_obj->source){
        case INTERRUPT_EXTERNAL_INT0 : ret = INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler); break;
        case INTERRUPT_EXTERNAL_INT1 : ret = INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler); break;
        case INTERRUPT_EXTERNAL_INT2 : ret = INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler); break;
        default : ret = E_NOT_OK;
    }
    return ret;
}