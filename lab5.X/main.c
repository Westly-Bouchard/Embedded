//--------------------------------------------------------------------
// Name:            Chris Coulston
// Date:            Fall 2020
// Purp:            inLab05
//
// Assisted:        The entire class of EENG 383
// Assisted by:     Microchips 18F26K22 Tech Docs
// Conversion to MCC Melody: Nolan Pratt (nolanpratt@mines.edu), Fall 2025
//-
//- Academic Integrity Statement: I certify that, while others may have
//- assisted me in brain storming, debugging and validating this program,
//- the program itself is my own work. I understand that submitting code
//- which is the work of other individuals is a violation of the course
//- Academic Integrity Policy and may result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board. I also understand that if I knowingly give my original work to
//- another individual that it could also result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board.
//------------------------------------------------------------------------
#include "mcc_generated_files/system/system.h"
#include <inttypes.h>
#include <stdint.h>
#pragma warning disable 520     // warning: (520) function "xyz" is never called
#pragma warning disable 1498    // fputc.c:16:: warning: (1498) pointer (unknown)

#define DUTY_INC    0x10
#define NUM_COLOR 6

    //Green-->Red-->Fuchsia-->Blue-->Turquoise-->Green
    uint16_t deltaRed[NUM_COLOR] = {0x0000,  0x0000,   0x0001, 0x0000, 0x0000, 0xFFFF};
    uint16_t deltaGre[NUM_COLOR] = {0x0001,  0x0000,   0x0000, 0xFFFF, 0x0000, 0x0000};
    uint16_t deltaBlu[NUM_COLOR] = {0x0000,  0xFFFF,   0x0000, 0x0000, 0x0001, 0x0000};
    //                            Inc G   Dec B   INC R DEC G INC B DEC R
    
    uint16_t initRed[NUM_COLOR] = {0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF};
    uint16_t initGre[NUM_COLOR] = {0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000};
    uint16_t initBlu[NUM_COLOR] = {0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF};
    //                            Yel   Red   Fuc   Blu   Tur   Gre
//----------------------------------------------
// Main "function"
//----------------------------------------------
bool colorTour = false;
void timer0ISR();

void main(void) {



    
    //So we can do nice 15 points of step (Gets us 0 and 255 whole numbers)
    int16_t redDuty = 135;
    int16_t greenDuty = 135;
    int16_t blueDuty = 135;
    uint16_t maxColorSpeed = 63536U;
    uint16_t twoFiftySixRaw = 62976U;
    uint16_t twoHundRaw = 2000U;
    uint16_t currentPeriod = 62976U;
    
    uint8_t i;
    char cmd;
   
    SYSTEM_Initialize();
    //ECCP1 C2  THIS WILL BE THE RED PART
    //ECCP2 C1  This WILL BE THE GREEN PART
    //ECCP3 B5  THIS WILL BE THE BLUE PART
    //Remeber our PIC is shitty so we must flip red and blue
    TMR0_OverflowCallbackRegister(timer0ISR);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    // Not necessary, but this delay allows the baud rate generator to
    // stablize before printing the splash screen on reset. If you are going to
    // do this, then make sure to put delay BEFORE enabling TMR interrupt.

    printf("inLab 05\r\n");
    printf("Color Cube\r\n");
    printf("Dev'21 board wiring\r\n");
    printf("RC2 <-> Red LED");
    printf("\r\n> "); // print a nice command prompt

    for (;;) {

        if (EUSART1_IsRxReady()) { // wait for incoming data on USART
            cmd = EUSART1_Read();
            switch (cmd) { // and do what it tells you to do
                case '?':
                    printf("------------------------------\r\n");
                    printf("    Red:   0x%x\r\n",redDuty);
                    printf("    Green: 0x%x\r\n",greenDuty);
                    printf("    Blue : 0x%x\r\n",blueDuty);
                    printf("------------------------------\r\n");
                    printf("?: Help menu\r\n");
                    printf("Z: Reset processor\r\n");
                    printf("z: Clear the terminal\r\n");
                    printf("R/r: increase/decrease Red intensity\r\n");
                    printf("G/g: increase/decrease Green intensity\r\n");
                    printf("B/b: increase/decrease Blue intensity\r\n");
                    printf("C/c: start/stop color cycle\r\n");
                    printf("a: All LEDs off\r\n");
                    printf("+/-: increase/decrease the color tour speed.\r\n");
                    printf("------------------------------\r\n");
                    break;
                    
                //--------------------------------------------
                // Reset the processor after clearing the terminal
                //--------------------------------------------
                case 'Z':
                    for (i = 0; i < 40; i++) printf("\n");
                    RESET();
                break;

                //--------------------------------------------
                // Clear the terminal
                //--------------------------------------------
                case 'z':
                    for (i = 0; i < 40; i++) printf("\n");
                    break;
                
                case 'R':   //Make RED LED BRIGHTER
                    if(redDuty-15<0){
                        printf("Already at Max Red\r\n");
                    } else {
                        redDuty-=15;
                        printf("Red it now brighter\r\n");
                        EPWM1_LoadDutyValue(redDuty);
                    }
                    break;
                    
                    
                case 'r':   //Make RED LED DIMMER  
                    if(redDuty+15>255){
                        printf("Can't go any darker!\r\n");
                    } else {
                        redDuty+=15;
                        printf("Red it now darker\r\n");
                        EPWM1_LoadDutyValue(redDuty);
                    }
                    break;
                    
                    
                case 'G':   //Make GREEN LED BRIGHTER
                    if(greenDuty-15<0){
                        printf("Already at Max Green\r\n");
                    } else {
                        greenDuty-=15;
                        printf("green it now brighter\r\n");
                        EPWM2_LoadDutyValue(greenDuty);
                    }
                    break;
                    
                    
                case 'g':   //Make Green LED DIMMER  
                    if(greenDuty+15>255){
                        printf("Can't go any darker!\r\n");
                    } else {
                        greenDuty+=15;
                        printf("green it now darker\r\n");
                        EPWM2_LoadDutyValue(greenDuty);
                    }
                    break;
                    
                    
                case 'B':   //Make GREEN LED BRIGHTER
                    if(blueDuty-15<0){
                        printf("Already at Max BLUE\r\n");
                    } else {
                        blueDuty-=15;
                        EPWM3_LoadDutyValue(blueDuty);
                    }
                    break;
                    
                    
                case 'b':   //Make BLUE LED DIMMER  
                    if(blueDuty+15>255){
                        printf("Can't go any darker!\r\n");
                    } else {
                        blueDuty+=15;
                        printf("blue it now darker\r\n");
                        EPWM3_LoadDutyValue(blueDuty);
                    }
                    break;
                    
                    
                case 'C':
                    printf("Starting color tour\r\n");
                    colorTour = true;
                    break;
                    
                case 'c':
                    printf("Ending color tour\r\n");
                    colorTour = false;
                    break;
                    
                case '+':
                    printf("Adding 2ms \r\n");
                    if(currentPeriod - twoHundRaw < 20000) {
                        printf("Can't go any slower! \r\n");
                    } else {
                        currentPeriod-=twoHundRaw;
                        TMR0_PeriodCountSet(currentPeriod);
                    }
                    break;
                    
                case '-':
                    printf("Subtracting 2ms \r\n");
                    if(currentPeriod + twoHundRaw > maxColorSpeed) {
                        printf("Can't go any faster! \r\n");
                    } else {
                        currentPeriod+=twoHundRaw;
                        TMR0_PeriodCountSet(currentPeriod);
                    }
                    break;
                    
                case 'a':
                    printf("Setting all channels to 0\r\n");
                    redDuty=0x0FF;
                    greenDuty=0xFF;
                    blueDuty=0xFF;
                    EPWM1_LoadDutyValue(redDuty);
                    EPWM2_LoadDutyValue(greenDuty);
                    EPWM3_LoadDutyValue(blueDuty);
                    break;
                //-------
                //--------------------------------------------
                // If something unknown is hit, tell user
                //--------------------------------------------
                default:
                    printf("Unknown key %c\r\n", cmd);
                    break;
            } // end switch
        } // end if
    } // end while
} // end main


void timer0ISR(){
    //Go between 0-255 to corespodn to duty cycle for each color
    static uint16_t currentRedValue = 0x0000;
    static uint16_t currentGreenValue = 0x0000;
    static uint16_t currentBlueValue = 0x00FF;
    
    static uint8_t currentCubeIndex = 0;
    static uint8_t currentIntraIndex = 0;
    if(colorTour == true) {
        
        EPWM1_LoadDutyValue(currentRedValue);
        EPWM2_LoadDutyValue(currentGreenValue);
        EPWM3_LoadDutyValue(currentBlueValue);
        
        currentRedValue += deltaRed[currentCubeIndex];
        currentGreenValue += deltaGre[currentCubeIndex];
        currentBlueValue += deltaBlu[currentCubeIndex];
        
        currentIntraIndex++;
        
        if(currentIntraIndex >= 255) {
            currentIntraIndex=0;
            if(currentCubeIndex >= 5){
                currentCubeIndex = 0;
            } else {
                currentCubeIndex++;
            }
        }
        
        
    }
    return;
}
//    EPWM1_LoadDutyValue(0xFFFF);
//    EPWM2_LoadDutyValue(0xFFFF);
//    EPWM3_LoadDutyValue(0x0000);