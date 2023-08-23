/* 
 * File:   mcal_interrupt_manager.c
 * Author: user
 *
 * Created on July 18, 2023, 10:17 PM
 */

#include "mcal_interrupt_manager.h"

static volatile uint8 RB4_Flag = FLAG_HIGH;
static volatile uint8 RB5_Flag = FLAG_HIGH;
static volatile uint8 RB6_Flag = FLAG_HIGH;
static volatile uint8 RB7_Flag = FLAG_HIGH;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh(void){
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCURRED == INTCONbits.INT0IF)){
        INT0_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCURRED == INTCON3bits.INT2IF)){
        INT2_ISR();
    }
    
}

void __interrupt(low_priority) InterruptManagerLow(void){
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCURRED == INTCON3bits.INT1IF)){
        INT1_ISR();
    }
    else{ /* Nothing */ }
} 


#else

void __interrupt() InterruptManager(void){
    /********************************** INT0 **********************************/
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCURRED == INTCONbits.INT0IF)){
        INT0_ISR();
    }
    else{ /* Nothing */ }
    /********************************** INT1 **********************************/
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCURRED == INTCON3bits.INT1IF)){
        INT1_ISR();
    }
    else{ /* Nothing */ }
    /********************************** INT1 **********************************/
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCURRED == INTCON3bits.INT2IF)){
        INT2_ISR();
    }
    else{ /* Nothing */ }
    /********************************** RB4 **********************************/
    // when changed to high
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB4 == GPIO_HIGH) && (FLAG_HIGH == RB4_Flag)){
        RB4_Flag = FLAG_LOW;
        RB4_ISR(RBx_ChangedToHigh);
    }
    else{ /* Nothing */ }
    // when changed to low
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB4 == GPIO_LOW) && (FLAG_LOW == RB4_Flag)){
        RB4_Flag = FLAG_HIGH;
        RB4_ISR(RBx_ChangedToLow);
    }
    else{ /* Nothing */ }
    /********************************** RB5 **********************************/
    // when changed to high
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB5 == GPIO_HIGH) && (FLAG_HIGH == RB5_Flag)){
        RB5_Flag = FLAG_LOW;
        RB5_ISR(RBx_ChangedToHigh);
    }
    else{ /* Nothing */ }
    // when changed to low
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB5 == GPIO_LOW) && (FLAG_LOW == RB5_Flag)){
        RB5_Flag = FLAG_HIGH;
        RB5_ISR(RBx_ChangedToLow);
    }
    else{ /* Nothing */ }
    /********************************** RB6 **********************************/
    // when changed to high
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB6 == GPIO_HIGH) && (FLAG_HIGH == RB6_Flag)){
        RB6_Flag = FLAG_LOW;
        RB6_ISR(RBx_ChangedToHigh);
    }
    else{ /* Nothing */ }
    // when changed to low
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB6 == GPIO_LOW) && (FLAG_LOW == RB6_Flag)){
        RB6_Flag = FLAG_HIGH;
        RB6_ISR(RBx_ChangedToLow);
    }
    else{ /* Nothing */ }
    /********************************** RB7 **********************************/
    // when changed to high
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB7 == GPIO_HIGH) && (FLAG_HIGH == RB7_Flag)){
        RB7_Flag = FLAG_LOW;
        RB7_ISR(RBx_ChangedToHigh);
    }
    else{ /* Nothing */ }
    // when changed to low
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCURRED == INTCONbits.RBIF) && 
            (PORTBbits.RB7 == GPIO_LOW) && (FLAG_LOW == RB7_Flag)){
        RB7_Flag = FLAG_HIGH;
        RB7_ISR(RBx_ChangedToLow);
    }
    else{ /* Nothing */ }
    /********************************** ADC **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) && (INTERRUPT_OCCURRED == PIR1bits.ADIF)){
        ADC_ISR();
    }
    else{ /* Nothing */ }
    /********************************** TMR0 **********************************/
    if((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCURRED == INTCONbits.TMR0IF)){
        TMR0_ISR();
    }
    else{ /* Nothing */ }
    /********************************** TMR1 **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCURRED == PIR1bits.TMR1IF)){
        TMR1_ISR();
    }
    else{ /* Nothing */ }
    /********************************** TMR2 **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.TMR2IE) && (INTERRUPT_OCCURRED == PIR1bits.TMR2IF)){
        TMR2_ISR();
    }
    else{ /* Nothing */ }
    /********************************** TMR3 **********************************/
    if((INTERRUPT_ENABLE == PIE2bits.TMR3IE) && (INTERRUPT_OCCURRED == PIR2bits.TMR3IF)){
        TMR3_ISR();
    }
    else{ /* Nothing */ }
    /********************************** CCP1 **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.CCP1IE) && (INTERRUPT_OCCURRED == PIR1bits.CCP1IF)){
        CCP1_ISR();
    }
    else{ /* Nothing */ }
    /********************************** CCP2 **********************************/
    if((INTERRUPT_ENABLE == PIE2bits.CCP2IE) && (INTERRUPT_OCCURRED == PIR2bits.CCP2IF)){
        CCP2_ISR();
    }
    else{ /* Nothing */ }
    /********************************** EUSART_TX **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.TXIE) && (INTERRUPT_OCCURRED == PIR1bits.TXIF)){
        EUSART_TX_ISR();
    }
    else{ /* Nothing */ }
    /********************************** EUSART_RX **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCURRED == PIR1bits.RCIF)){
        EUSART_RX_ISR();
    }
    else{ /* Nothing */ }
    /********************************** SPI **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.SSPIE) && (INTERRUPT_OCCURRED == PIR1bits.SSPIF)){
        SPI_ISR();
    }
    else{ /* Nothing */ }
    /********************************** I2C **********************************/
    if((INTERRUPT_ENABLE == PIE1bits.SSPIE) && (INTERRUPT_OCCURRED == PIR1bits.SSPIF)){
        I2C_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE2bits.BCLIE) && (INTERRUPT_OCCURRED == PIR2bits.BCLIF)){
        I2C_BC_ISR();
    }
    else{ /* Nothing */ }
    /********************************** END **********************************/
}

#endif