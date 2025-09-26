/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.1
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

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA2 aliases
#define TOP_BUTTON_TRIS                 TRISAbits.TRISA2
#define TOP_BUTTON_LAT                  LATAbits.LATA2
#define TOP_BUTTON_PORT                 PORTAbits.RA2
#define TOP_BUTTON_WPU                  WPUAbits.
#define TOP_BUTTON_OD                   ODCONAbits.
#define TOP_BUTTON_ANS                  ANSELAbits.ANSA2
#define TOP_BUTTON_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define TOP_BUTTON_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define TOP_BUTTON_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define TOP_BUTTON_GetValue()           PORTAbits.RA2
#define TOP_BUTTON_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define TOP_BUTTON_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define TOP_BUTTON_SetPullup()          do { WPUAbits. = 1; } while(0)
#define TOP_BUTTON_ResetPullup()        do { WPUAbits. = 0; } while(0)
#define TOP_BUTTON_SetPushPull()        do { ODCONAbits. = 0; } while(0)
#define TOP_BUTTON_SetOpenDrain()       do { ODCONAbits. = 1; } while(0)
#define TOP_BUTTON_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define TOP_BUTTON_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RA3 aliases
#define BOTTOM_BUTTON_TRIS                 TRISAbits.TRISA3
#define BOTTOM_BUTTON_LAT                  LATAbits.LATA3
#define BOTTOM_BUTTON_PORT                 PORTAbits.RA3
#define BOTTOM_BUTTON_WPU                  WPUAbits.
#define BOTTOM_BUTTON_OD                   ODCONAbits.
#define BOTTOM_BUTTON_ANS                  ANSELAbits.ANSA3
#define BOTTOM_BUTTON_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define BOTTOM_BUTTON_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define BOTTOM_BUTTON_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define BOTTOM_BUTTON_GetValue()           PORTAbits.RA3
#define BOTTOM_BUTTON_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define BOTTOM_BUTTON_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define BOTTOM_BUTTON_SetPullup()          do { WPUAbits. = 1; } while(0)
#define BOTTOM_BUTTON_ResetPullup()        do { WPUAbits. = 0; } while(0)
#define BOTTOM_BUTTON_SetPushPull()        do { ODCONAbits. = 0; } while(0)
#define BOTTOM_BUTTON_SetOpenDrain()       do { ODCONAbits. = 1; } while(0)
#define BOTTOM_BUTTON_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define BOTTOM_BUTTON_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set RB5 aliases
#define SPEAKER_PIN_TRIS                 TRISBbits.TRISB5
#define SPEAKER_PIN_LAT                  LATBbits.LATB5
#define SPEAKER_PIN_PORT                 PORTBbits.RB5
#define SPEAKER_PIN_WPU                  WPUBbits.WPUB5
#define SPEAKER_PIN_OD                   ODCONBbits.
#define SPEAKER_PIN_ANS                  ANSELBbits.ANSB5
#define SPEAKER_PIN_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SPEAKER_PIN_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SPEAKER_PIN_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SPEAKER_PIN_GetValue()           PORTBbits.RB5
#define SPEAKER_PIN_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SPEAKER_PIN_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SPEAKER_PIN_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SPEAKER_PIN_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define SPEAKER_PIN_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define SPEAKER_PIN_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define SPEAKER_PIN_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define SPEAKER_PIN_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/