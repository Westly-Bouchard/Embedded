/**
 * System Driver Source File
 * 
 * @file system.c
 * 
 * @ingroup systemdriver
 * 
 * @brief This file contains the API implementation for the System driver.
 *
 * @version Driver Version 1.0.1
 *
 * @version Package Version 1.0.3
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../system.h"

/** 
* @ingroup systemdriver
* @brief Initializes the Peripheral Module Disable (PMD) module.
* @param None.
* @return None.
*/
void PMD_Initialize(void);


void SYSTEM_Initialize(void)
{
    CLOCK_Initialize();
    PIN_MANAGER_Initialize();
    TMR0_Initialize();
    TMR1_Initialize();
    PMD_Initialize();
    INTERRUPT_Initialize();
}

void PMD_Initialize(void)
{
    //TMR1MD TMR1MD enabled; TMR2MD TMR2MD enabled; TMR3MD TMR3MD enabled; TMR4MD TMR4MD enabled; TMR5MD TMR5MD enabled; TMR6MD TMR6MD enabled; UART1MD UART1MD enabled; UART2MD UART2MD enabled; 
    PMD0 = 0x0;
    //CCP1MD CCP1MD enabled; CCP2MD CCP2MD enabled; CCP3MD CCP3MD enabled; CCP4MD CCP4MD enabled; CCP5MD CCP5MD enabled; MSSP1MD MSSP1MD enabled; MSSP2MD MSSP2MD enabled; 
    PMD1 = 0x0;
    //CTMUMD CTMUMD enabled; CMP2MD CMP2MD enabled; CMP1MD CMP1MD enabled; ADCMD ADCMD enabled; 
    PMD2 = 0x0;
}

