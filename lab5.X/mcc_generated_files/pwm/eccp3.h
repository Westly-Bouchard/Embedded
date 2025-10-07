/**
 * PWM3 Generated Driver API Header File.
 * 
 * @file eccp3.h
 * 
 * @defgroup pwm3 PWM3
 * 
 * @brief This file contains the API prototypes for the PWM3 module.
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

#ifndef PWM3_H
#define PWM3_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @ingroup pwm3
 * @brief Defines the Custom Name for the \ref ECCP3_Initialize API
 */
#define EPWM3_Initialize ECCP3_Initialize

/**
 * @ingroup pwm3
 * @brief Defines the Custom Name for the \ref ECCP3_LoadDutyValue API
 */
#define EPWM3_LoadDutyValue ECCP3_LoadDutyValue

 /**
 * @ingroup pwm3
 * @brief Initializes the ECCP3 module. This is called only once before calling other ECCP3 APIs.
 * @param None.
 * @return None.
 */
void ECCP3_Initialize(void);

/**
 * @ingroup pwm3
 * @brief Loads the 16-bit duty cycle value.
 * @pre ECCP3_Initialize() is already called.
 * @param dutyValue - 16-bit duty cycle value
 * @return None.
 */
void ECCP3_LoadDutyValue(uint16_t dutyValue);

#endif //PWM3_H
/**
 End of File
*/
