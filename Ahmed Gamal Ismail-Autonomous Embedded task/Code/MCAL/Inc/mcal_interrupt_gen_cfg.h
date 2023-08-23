/* 
 * File:   mcal_interrupt_gen_cfg.h
 * Author: user
 *
 * Created on July 19, 2023, 12:00 AM
 */

#ifndef MCAL_INTERRUPT_GEN_CFG_H
#define	MCAL_INTERRUPT_GEN_CFG_H

/* Section : Includes */

/* Section : Macro Declarations */
#define INTERRUPT_FEATURE_ENABLE    1U

//#define INTERRUPT_PRIORITY_LEVELS_ENABLE                INTERRUPT_FEATURE_ENABLE

#define EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE          INTERRUPT_FEATURE_ENABLE
#define EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE      INTERRUPT_FEATURE_ENABLE

#define ADC_INTERRUPT_FEATURE_ENABLE                    INTERRUPT_FEATURE_ENABLE 

#define TIMER0_INTERRUPT_FEATURE_ENABLE                 INTERRUPT_FEATURE_ENABLE
#define TIMER1_INTERRUPT_FEATURE_ENABLE                 INTERRUPT_FEATURE_ENABLE
#define TIMER2_INTERRUPT_FEATURE_ENABLE                 INTERRUPT_FEATURE_ENABLE
#define TIMER3_INTERRUPT_FEATURE_ENABLE                 INTERRUPT_FEATURE_ENABLE

#define CCP1_INTERRUPT_FEATURE_ENABLE                   INTERRUPT_FEATURE_ENABLE
#define CCP2_INTERRUPT_FEATURE_ENABLE                   INTERRUPT_FEATURE_ENABLE

#define EUSART_TX_INTERRUPT_FEATURE_ENABLE              INTERRUPT_FEATURE_ENABLE
#define EUSART_RX_INTERRUPT_FEATURE_ENABLE              INTERRUPT_FEATURE_ENABLE

#define SPI_INTERRUPT_FEATURE_ENABLE                    INTERRUPT_FEATURE_ENABLE
#define I2C_INTERRUPT_FEATURE_ENABLE                    INTERRUPT_FEATURE_ENABLE

/* Section : Macro  Function Declarations */

/* Section : Data Type Declarations */

/* Section : Function Declarations */

#endif	/* MCAL_INTERRUPT_GEN_CFG_H */
