/* 
 * File:   mcal_interrupt_config.h
 * Author: user
 *
 * Created on July 18, 2023, 10:13 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H

/* Section : Includes */
//#include "pic18f4620.h"       /* #include "../include/proc/pic18f4620.h" */ /* warning : included directly */
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "mcal_interrupt_gen_cfg.h"
#include "../GPIO/hal_gpio.h"

/* Section : Macro Declarations */
#define INTERRUPT_ENABLE            1
#define INTERRUPT_DISABLE           0
#define INTERRUPT_OCCURRED          1
#define INTERRUPT_NOT_OCCURRED      0
#define INTERRUPT_PRIORITY_ENABLE   1
#define INTERRUPT_PRIORITY_DISABLE  0

#define RBx_ChangedToHigh           0
#define RBx_ChangedToLow            1

#define FLAG_HIGH                   1
#define FLAG_LOW                    0

/* Section : Macro  Function Declarations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define INTERRUPT_PriorityLevelEnable()         (RCONbits.IPEN = 1)             /* enable priority  */
#define INTERRUPT_PriorityLevelDisable()        (RCONbits.IPEN = 0)             /* disable priority  */

#define INTERRUPT_GlobalInterruptHighEnable()   (INTCONbits.GIEH = 1)           /* enable high priority global interrupts */
#define INTERRUPT_GlobalInterruptHighDisable()  (INTCONbits.GIEH = 0)           /* disable high priority global interrupts */

#define INTERRUPT_GlobalInterruptLowEnable()    (INTCONbits.GIEL = 1)           /* enable low priority global interrupts */
#define INTERRUPT_GlobalInterruptLowDisable()   (INTCONbits.GIEL = 0)           /* disable low priority global interrupts */

#else

#define INTERRUPT_GlobalInterruptEnable()       (INTCONbits.GIE = 1)            /* enable global interrupts */
#define INTERRUPT_GlobalInterruptDisable()      (INTCONbits.GIE = 0)            /* disable global interrupts */

#define INTERRUPT_PeripheralInterruptEnable()   (INTCONbits.PEIE = 1)           /* enable peripheral interrupts  */
#define INTERRUPT_PeripheralInterruptDisable()  (INTCONbits.PEIE = 0)           /* disable peripheral interrupts  */

#endif

/* Section : Data Type Declarations */
typedef enum{
    INTERRUPT_LOW_PRIORITY = 0,
    INTERRUPT_HIGH_PRIORITY        
}interrupt_priority_cfg;

/* Section : Function Declarations */

#endif	/* MCAL_INTERRUPT_CONFIG_H */

