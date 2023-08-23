/* 
 * File:   mcal_external_interrupt.h
 * Author: user
 *
 * Created on July 18, 2023, 10:15 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* Section : Includes */
#include "mcal_interrupt_config.h"

/* Section : Macro Declarations */

/* Section : Macro  Function Declarations */
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_INT0_InterruptDisable()         (INTCONbits.INT0IE = 0)         /* disable int0 interrupt */
#define EXT_INT0_InterruptEnable()          (INTCONbits.INT0IE = 1)         /* enable int0 interrupt */
#define EXT_INT0_InterruptFlagClear()       (INTCONbits.INT0IF = 0)         /* clear int0 interrupt flag */
#define EXT_INT0_RisingEdgeSet()            (INTCON2bits.INTEDG0 = 1)       /* interrupt int0 on rising edge */
#define EXT_INT0_FallingEdgeSet()           (INTCON2bits.INTEDG0 = 0)       /* interrupt int0 on falling edge */

#define EXT_INT1_InterruptDisable()         (INTCON3bits.INT1IE = 0)        /* disable int1 interrupt */
#define EXT_INT1_InterruptEnable()          (INTCON3bits.INT1IE = 1)        /* enable int1 interrupt */
#define EXT_INT1_InterruptFlagClear()       (INTCON3bits.INT1IF = 0)        /* clear int1 interrupt flag */
#define EXT_INT1_RisingEdgeSet()            (INTCON2bits.INTEDG1 = 1)       /* interrupt int1 on rising edge */
#define EXT_INT1_FallingEdgeSet()           (INTCON2bits.INTEDG1 = 0)       /* interrupt int1 on falling edge */

#define EXT_INT2_InterruptDisable()         (INTCON3bits.INT2IE = 0)        /* disable int2 interrupt */
#define EXT_INT2_InterruptEnable()          (INTCON3bits.INT2IE = 1)        /* enable int2 interrupt */
#define EXT_INT2_InterruptFlagClear()       (INTCON3bits.INT2IF = 0)        /* clear int2 interrupt flag */
#define EXT_INT2_RisingEdgeSet()            (INTCON2bits.INTEDG2 = 1)       /* nterrupt int2 on rising edge */
#define EXT_INT2_FallingEdgeSet()           (INTCON2bits.INTEDG2 = 0)       /* nterrupt int2 on falling edge */

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_INT1_HighPrioritySet()          (INTCON3bits.INT1IP = 1)        /* set int1 priority high */
#define EXT_INT1_LowPrioritySet()           (INTCON3bits.INT1IP = 0)        /* set int1 priority low */

#define EXT_INT2_HighPrioritySet()          (INTCON3bits.INT2IP = 1)        /* set int2 priority high */
#define EXT_INT2_LowPrioritySet()           (INTCON3bits.INT2IP = 0)        /* set int2 priority low */

#endif

#endif

#if EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_RBx_InterruptDisable()          (INTCONbits.RBIE = 0)           /* disable rbx interrupt */
#define EXT_RBx_InterruptEnable()           (INTCONbits.RBIE = 1)           /* enable rbx interrupt */
#define EXT_RBx_InterruptFlagClear()        (INTCONbits.RBIF = 0)           /* clear rbx interrupt flag */

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_RBx_HighPrioritySet()          (INTCON2bits.RBIP = 1)           /* set rbx priority high */
#define EXT_RBx_LowPrioritySet()           (INTCON2bits.RBIP = 0)           /* set rbx priority low */

#endif

#endif

/* Section : Data Type Declarations */
typedef void (*InterruptHandler)(void);

typedef enum{
    INTERRUPT_FALLING_EDGE = 0,
    INTERRUPT_RISING_EDGE        
}interrupt_INTx_edge;

typedef enum{
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2        
}interrupt_INTx_src;

typedef struct{
    void (* EXT_InterruptHandler)(void);
    pin_config_t mcu_pin;
    interrupt_INTx_edge edge;
    interrupt_INTx_src source;
    interrupt_priority_cfg priority;
}interrupt_INTx_t;

typedef struct{
    void (* EXT_InterruptHandler_ChangedToHigh)(void);
    void (* EXT_InterruptHandler_ChangedToLow)(void);
    pin_config_t mcu_pin;                   /* in RB register pins: 4, 5, 6, 7 */  
    interrupt_priority_cfg priority;
}interrupt_RBx_t;

/* Section : Function Declarations */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *int_obj);
Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t *int_obj);

Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t *int_obj);
Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t *int_obj);

#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

