/* 
 * File:   mcal_internal_interrupt.h
 * Author: user
 *
 * Created on July 18, 2023, 10:14 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/* Section : Includes */
#include "mcal_interrupt_config.h"

/* Section : Macro Declarations */

/* Section : Macro  Function Declarations */

/************************ ADC *****************/
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable ADC interrupt */
#define ADC_InterruptDisable()              (PIE1bits.ADIE = 0)
/* Enable ADC interrupt */
#define ADC_InterruptEnable()               (PIE1bits.ADIE = 1)
/* Clear ADC interrupt flag */
#define ADC_InterruptFlagClear()            (PIR1bits.ADIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set ADC priority high */
#define ADC_HighPrioritySet()          (IPR1bits.ADIP = 1)
/* set ADC priority low */
#define ADC_LowPrioritySet()           (IPR1bits.ADIP = 0)
#endif

#endif

/************************ TIMER0 *****************/
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable TIMER0 interrupt */
#define TMR0_InterruptDisable()              (INTCONbits.TMR0IE = 0)
/* Enable TIMER0 interrupt */
#define TMR0_InterruptEnable()               (INTCONbits.TMR0IE = 1)
/* Clear TIMER0 interrupt flag */
#define TMR0_InterruptFlagClear()            (INTCONbits.TMR0IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set TIMER0 priority high */
#define TMR0_HighPrioritySet()          (INTCON2bits.TMR0IP = 1)
/* set TIMER0 priority low */
#define TMR0_LowPrioritySet()           (INTCON2bits.TMR0IP = 0)
#endif

#endif

/************************ TIMER1 *****************/
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable TIMER1 interrupt */
#define TMR1_InterruptDisable()              (PIE1bits.TMR1IE = 0)
/* Enable TIMER1 interrupt */
#define TMR1_InterruptEnable()               (PIE1bits.TMR1IE = 1)
/* Clear TIMER1 interrupt flag */
#define TMR1_InterruptFlagClear()            (PIR1bits.TMR1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set TIMER1 priority high */
#define TMR1_HighPrioritySet()          (IPR1bits.TMR1IP = 1)
/* set TIMER1 priority low */
#define TMR1_LowPrioritySet()           (IPR1bits.TMR1IP = 0)
#endif

#endif

/************************ TIMER2 *****************/
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable TIMER2 interrupt */
#define TMR2_InterruptDisable()              (PIE1bits.TMR2IE = 0)
/* Enable TIMER2 interrupt */
#define TMR2_InterruptEnable()               (PIE1bits.TMR2IE = 1)
/* Clear TIMER2 interrupt flag */
#define TMR2_InterruptFlagClear()            (PIR1bits.TMR2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set TIMER2 priority high */
#define TMR2_HighPrioritySet()          (IPR1bits.TMR2IP = 1)
/* set TIMER2 priority low */
#define TMR2_LowPrioritySet()           (IPR1bits.TMR2IP = 0)
#endif

#endif

/************************ TIMER3 *****************/
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable TIMER3 interrupt */
#define TMR3_InterruptDisable()              (PIE2bits.TMR3IE = 0)
/* Enable TIMER3 interrupt */
#define TMR3_InterruptEnable()               (PIE2bits.TMR3IE = 1)
/* Clear TIMER3 interrupt flag */
#define TMR3_InterruptFlagClear()            (PIR2bits.TMR3IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set TIMER3 priority high */
#define TMR3_HighPrioritySet()          (IPR2bits.TMR3IP = 1)
/* set TIMER3 priority low */
#define TMR3_LowPrioritySet()           (IPR2bits.TMR3IP = 0)
#endif

#endif

/************************ CCP1 *****************/
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable CCP1 interrupt */
#define CCP1_InterruptDisable()              (PIE1bits.CCP1IE = 0)
/* Enable CCP1 interrupt */
#define CCP1_InterruptEnable()               (PIE1bits.CCP1IE = 1)
/* Clear TIMER3 interrupt flag */
#define CCP1_InterruptFlagClear()            (PIR1bits.CCP1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set CCP1 priority high */
#define CCP1_HighPrioritySet()          (IPR1bits.CCP1IP = 1)
/* set CCP1 priority low */
#define CCP1_LowPrioritySet()           (IPR1bits.CCP1IP = 0)
#endif

#endif

/************************ CCP2 *****************/
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable CCP2 interrupt */
#define CCP2_InterruptDisable()              (PIE2bits.CCP2IE = 0)
/* Enable CCP2 interrupt */
#define CCP2_InterruptEnable()               (PIE2bits.CCP2IE = 1)
/* Clear CCP2 interrupt flag */
#define CCP2_InterruptFlagClear()            (PIR2bits.CCP2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set CCP2 priority high */
#define CCP2_HighPrioritySet()          (IPR2bits.CCP2IP = 1)
/* set CCP2 priority low */
#define CCP2_LowPrioritySet()           (IPR2bits.CCP2IP = 0)
#endif

#endif

/************************ EUSART_TX *****************/
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable EUSART_TX interrupt */
#define EUSART_TX_InterruptDisable()              (PIE1bits.TXIE = 0)
/* Enable EUSART_TX interrupt */
#define EUSART_TX_InterruptEnable()               (PIE1bits.TXIE = 1)
/* Clear EUSART_TX interrupt flag */
//#define EUSART_TX_InterruptFlagClear()            (PIR1bits.TXIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set EUSART_TX priority high */
#define EUSART_TX_HighPrioritySet()          (IPR1bits.TXIP = 1)
/* set EUSART_TX priority low */
#define EUSART_TX_LowPrioritySet()           (IPR1bits.TXIP = 0)
#endif

#endif

/************************ EUSART_RX *****************/
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable EUSART_RX interrupt */
#define EUSART_RX_InterruptDisable()              (PIE1bits.RCIE = 0)
/* Enable EUSART_RX interrupt */
#define EUSART_RX_InterruptEnable()               (PIE1bits.RCIE = 1)
/* Clear EUSART_RX interrupt flag */
//#define EUSART_RX_InterruptFlagClear()            (PIR1bits.RCIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set EUSART_RX priority high */
#define EUSART_RX_HighPrioritySet()          (IPR1bits.RCIP = 1)
/* set EUSART_RX priority low */
#define EUSART_RX_LowPrioritySet()           (IPR1bits.RCIP = 0)
#endif

#endif

/************************ SPI *****************/
#if SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable SPI interrupt */
#define SPI_InterruptDisable()              (PIE1bits.SSPIE = 0)
/* Enable SPI interrupt */
#define SPI_InterruptEnable()               (PIE1bits.SSPIE = 1)
/* Clear SPI interrupt flag */
#define SPI_InterruptFlagClear()            (PIR1bits.SSPIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set SPI priority high */
#define SPI_HighPrioritySet()          (IPR1bits.SSPIP = 1)
/* set SPI priority low */
#define SPI_LowPrioritySet()           (IPR1bits.SSPIP = 0)
#endif

#endif

/************************ I2C *****************/
#if I2C_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

/* Disable I2C interrupt */
#define I2C_InterruptDisable()                  (PIE1bits.SSPIE = 0)
#define I2C_BUS_COLLISION_InterruptDisable()    (PIE2bits.BCLIE = 0)
/* Enable I2C interrupt */
#define I2C_InterruptEnable()                   (PIE1bits.SSPIE = 1)
#define I2C_BUS_COLLISION_InterruptEnable()     (PIE2bits.BCLIE = 1)
/* Clear I2C interrupt flag */
#define I2C_InterruptFlagClear()                (PIR1bits.SSPIF = 0)
#define I2C_BUS_COLLISION_InterruptFlagClear()  (PIR2bits.BCLIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/* set I2C priority high */
#define I2C_HighPrioritySet()                       (IPR1bits.SSPIP = 1)
#define I2C_BUS_COLLISION_HighPrioritySet()         (IPR2bits.BCLIP = 1)
/* set I2C priority low */
#define I2C_LowPrioritySet()                        (IPR1bits.SSPIP = 0)
#define I2C_BUS_COLLISION_LowPrioritySet()          (IPR2bits.BCLIP = 0)
#endif

#endif

/************************ END *****************/

/* Section : Data Type Declarations */

/* Section : Function Declarations */

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

