/**
 * PWM3 Generated Driver File.
 * 
 * @file eccp3.c
 * 
 * @ingroup pwm3
 * 
 * @brief This file contains the API implementations for the PWM3 driver.
 *
 * @version PWM3 Driver Version 1.0.0
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

#include <xc.h>
#include "../eccp3.h"

#define PWM3_INITIALIZE_DUTY_VALUE 0

void ECCP3_Initialize(void)
{
    // Set the PWM3 to the options selected in the User Interface
    
    // CCPM P3A,P3C: active high; P3B,P3D: active high; DCB 0; PM single; 
    CCP3CON = 0xC;
    
    // PSSBD low; PSSAC low; CCPAS disabled; CCPASE operating; 
    ECCP3AS = 0x0;
    
    // PDC 0; PRSEN manual_restart; 
    PWM3CON = 0x0;
    
    // STRA P3A_to_CCP3M; STRB P3B_to_port; STRSYNC start_at_begin; STRC P3C_to_port; STRD P3D_to_port; 
    PSTR3CON = 0x1;
    
    // CCPRH 0; 
    CCPR3H = 0x0;
    
    // CCPRL 0; 
    CCPR3L = 0x0;
    
    // Selecting Timer 2
    CCPTMRS0bits.C3TSEL = 0x0; 
}

void ECCP3_LoadDutyValue(uint16_t dutyValue)
{
	dutyValue &= 0x03FF;

    // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR3L = ((dutyValue & 0x03FC) >> 2);

    // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP3CON = (uint8_t)((CCP3CON & 0xCF) | ((dutyValue & 0x0003) << 4));
}

/**
 End of File
*/
