/* 
 * File:   hal_eeprom.?
 * Author: user
 *
 * Created on July 20, 2023, 6:31 PM
 */

#include "hal_eeprom.h"

/**
 * 
 * @param bAdd
 * @param bData
 * @return 
 */
Std_ReturnType Data_EEPROM_WriteByte(uint16 bAdd, uint8 bData){
    Std_ReturnType ret = E_OK;
    /* Read Interrupt Status : Enabled or Disabled and Save it */
    uint8 Global_Interrupt_Status = (INTCONbits.GIE);
    /* Update Address Registers */
    EEADRH = (uint8)((bAdd >> EEADR_HIGH_SHIFT_BITS) & EEADR_HIGH_MASK);
    EEADR = (uint8)(bAdd & EEADR_LOW_MASK);
    /* Update Data Registers */
    EEDATA = bData;
    /* Access Data EEPROM Memory */
    EECON1bits.EEPGD = EEPROM_MEMORY_ACCESS;
    EECON1bits.CFGS = EEPROM_OR_FLASH_MEMORY_ACCESS;
    /* Allow Write Cycle to Flash/EEPROM */
    EECON1bits.WREN = WRITE_CYCLE_EEPROM_OR_FLASH_ALLOW;
    /* Disable all Interrupts */
#ifdef INTERRUPT_GlobalInterruptDisable
    INTERRUPT_GlobalInterruptDisable();
#endif
    /* Write this sequence : 0x55 then 0xAA */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    /* Initiate a data EEPROM erase/write cycle */
    EECON1bits.WR = DATA_ERASE_WRITE_CYCLE_INITATE;
    /* wait for the operation */
    while(EECON1bits.WR);
    /* Inhibits write cycle to Flash program/data EEPROM  */
    EECON1bits.WREN = WRITE_CYCLE_EEPROM_OR_FLASH_INHIBIT;
    /* Restore Interrupt Status */
    INTCONbits.GIE = Global_Interrupt_Status;
    return ret;
}

/**
 * 
 * @param bAdd
 * @param bData
 * @return 
 */
Std_ReturnType Data_EEPROM_ReadByte(uint16 bAdd, uint8 *bData){
    Std_ReturnType ret = E_OK;
    if(NULL == bData){
        ret = E_NOT_OK;
    }
    else{
        /* Update Address Registers */
        EEADRH = (uint8)((bAdd >> EEADR_HIGH_SHIFT_BITS) & EEADR_HIGH_MASK);
        EEADR = (uint8)(bAdd & EEADR_LOW_MASK);
        /* Access Data EEPROM Memory */
        EECON1bits.EEPGD = EEPROM_MEMORY_ACCESS;
        EECON1bits.CFGS = EEPROM_OR_FLASH_MEMORY_ACCESS;
        /* Initiate a data EEPROM erase/write cycle */
        EECON1bits.RD = DATA_READ_CYCLE_INITATE;
        /* Read Operation Latency For High Frequency */
        NOP();
        NOP();
        /* Return Data */
        *bData = EEDATA;
    }
    return ret;
}