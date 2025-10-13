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

// get/set RA6 aliases
#define TEST_PIN_TRIS                 TRISAbits.TRISA6
#define TEST_PIN_LAT                  LATAbits.LATA6
#define TEST_PIN_PORT                 PORTAbits.RA6
#define TEST_PIN_WPU                  WPUAbits.
#define TEST_PIN_OD                   ODCONAbits.
#define TEST_PIN_ANS                  ANSELAbits.
#define TEST_PIN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define TEST_PIN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define TEST_PIN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define TEST_PIN_GetValue()           PORTAbits.RA6
#define TEST_PIN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define TEST_PIN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define TEST_PIN_SetPullup()          do { WPUAbits. = 1; } while(0)
#define TEST_PIN_ResetPullup()        do { WPUAbits. = 0; } while(0)
#define TEST_PIN_SetPushPull()        do { ODCONAbits. = 0; } while(0)
#define TEST_PIN_SetOpenDrain()       do { ODCONAbits. = 1; } while(0)
#define TEST_PIN_SetAnalogMode()      do { ANSELAbits. = 1; } while(0)
#define TEST_PIN_SetDigitalMode()     do { ANSELAbits. = 0; } while(0)

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

// get/set RC6 aliases
#define IO_RC6_TRIS                 TRISCbits.TRISC6
#define IO_RC6_LAT                  LATCbits.LATC6
#define IO_RC6_PORT                 PORTCbits.RC6
#define IO_RC6_WPU                  WPUCbits.
#define IO_RC6_OD                   ODCONCbits.
#define IO_RC6_ANS                  ANSELCbits.ANSC6
#define IO_RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define IO_RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define IO_RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define IO_RC6_GetValue()           PORTCbits.RC6
#define IO_RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define IO_RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define IO_RC6_SetPullup()          do { WPUCbits. = 1; } while(0)
#define IO_RC6_ResetPullup()        do { WPUCbits. = 0; } while(0)
#define IO_RC6_SetPushPull()        do { ODCONCbits. = 0; } while(0)
#define IO_RC6_SetOpenDrain()       do { ODCONCbits. = 1; } while(0)
#define IO_RC6_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define IO_RC6_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 aliases
#define IO_RC7_TRIS                 TRISCbits.TRISC7
#define IO_RC7_LAT                  LATCbits.LATC7
#define IO_RC7_PORT                 PORTCbits.RC7
#define IO_RC7_WPU                  WPUCbits.
#define IO_RC7_OD                   ODCONCbits.
#define IO_RC7_ANS                  ANSELCbits.ANSC7
#define IO_RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define IO_RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define IO_RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define IO_RC7_GetValue()           PORTCbits.RC7
#define IO_RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define IO_RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define IO_RC7_SetPullup()          do { WPUCbits. = 1; } while(0)
#define IO_RC7_ResetPullup()        do { WPUCbits. = 0; } while(0)
#define IO_RC7_SetPushPull()        do { ODCONCbits. = 0; } while(0)
#define IO_RC7_SetOpenDrain()       do { ODCONCbits. = 1; } while(0)
#define IO_RC7_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define IO_RC7_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)

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