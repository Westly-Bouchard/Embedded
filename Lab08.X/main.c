//--------------------------------------------------------------------
// Name:            Chris Coulston
// Date:            Fall 2020
// Purp:            inLab08
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
#pragma warning disable 520     // warning: (520) function "xyz" is never called  3
#pragma warning disable 1498    // fputc.c:16:: warning: (1498) pointer (unknown)

#define     NUM_SAMPLES     512
uint8_t bufferFull = false;
typedef enum  {MIC_IDLE, MIC_WAIT_FOR_TRIGGER, MIC_ACQUIRE} myTMR0states_t;

myTMR0states_t myTRM0states = MIC_IDLE;

uint8_t lowLevelTrigger = 98;
uint8_t upperLevelTrigger = 158;
uint8_t fillBufferAgain = false;

void INIT_PIC(void);
void myTMR0ISR(void);

uint8_t fatBuffer[512];

//----------------------------------------------
// Main "function"
//----------------------------------------------

void main(void) {

    uint16_t i;
    char cmd;
   

    SYSTEM_Initialize();
    INTCONbits.RBIE = 0;

    // BEFORE enabling interrupts, otherwise that while loop becomes an
    // infinite loop.  Doing this to give EUSART1's baud rate generator time
    // to stabilize - this will make the splash screen looks better
    TMR0_CounterSet(0x0000);
    INTCONbits.TMR0IF = 0;
    while (INTCONbits.TMR0IF == 0);

    TMR0_OverflowCallbackRegister(myTMR0ISR);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    printf("inLab 08\r\n");
    printf("Microphone experiments\r\n");
    printf("Dev'21\r\n");
    printf("> "); // print a nice command prompt

    for (;;) {

        if (EUSART1_IsRxReady()) { // wait for incoming data on USART
            cmd = EUSART1_Read();
            switch (cmd) { // and do what it tells you to do

                case '?':
                    help_menu:
                    printf("------------------------------\r\n");
                    printf("?: Help menu\r\n");
                    printf("o: k\r\n");
                    printf("Z: Reset processor\r\n");
                    printf("z: Clear the terminal\r\n");
                    printf("T/t Increase/decrease threshold %d - %d\r\n",lowLevelTrigger, upperLevelTrigger);
                    printf("f: gather 512 samples from the microphone and calculate the frequency\r\n");
                    printf("Buffer is ready for decoding: %d\r\n", !fillBufferAgain);
                    printf("------------------------------\r\n");
                    break;

                    //--------------------------------------------
                    // Reply with "k", used for PC to PIC test
                    //--------------------------------------------    
                case 'o':
                    printf(" k\r\n>");
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
                    for (i = 0; i < 40; i++) printf("\n>");
                    break;

                    //--------------------------------------------
                    // Continue to collect samples until the user
                    // presses a key on the keyboard
                    //--------------------------------------------                                          
                case 'f':
                    uint16_t zerosIndex[120];
                    uint16_t numberOfCrossings = 0;
                            
                    
                    printf("The last %d ADC samples from the microphone are:\r\n", NUM_SAMPLES);
                    for (uint16_t j = 1; j < NUM_SAMPLES; j++) { // print-out samples
                        if(j % 16 == 0) {
                            printf("\r\nS[%d] ",j);
                        }
                        printf("%d ", fatBuffer[j]);
                        if( ( (fatBuffer[j-1] < 128) && (fatBuffer[j] >= 128)  ) )  {
                            zerosIndex[numberOfCrossings] = j;
                            numberOfCrossings++;
                        }
                    }
                    
                    uint16_t runSum = 0;
                    uint16_t average = 0;
                    for(uint16_t j = 1; j < numberOfCrossings; j++) {
                        uint16_t difference = zerosIndex[j] - zerosIndex[j-1];
                        runSum += difference;
                        printf("%d - %d = %d\r\n",zerosIndex[j], zerosIndex[j-1], difference);
                    }
                   
                    float avg = ( ( runSum*25.0f ) / (numberOfCrossings-1) );
                    

                    float frequency = 1000000.0f / avg;  
                    printf("Frequency: %.3f Hz\r\n", frequency);
                    
                    fillBufferAgain = true;
                    break;
                    

                    
                case 't':
                    lowLevelTrigger+=5;
                    upperLevelTrigger-=5;
                    goto help_menu;
                    break;
                case 'T' :
                    lowLevelTrigger-=5;
                    upperLevelTrigger+=5;
                    goto help_menu;
                    break;
                    
                    
            } // end switch
        } // end if
    } // end while 

} // end main

//-----------------------------------------------------------------------------
// Start an analog to digital conversion every 100uS.  Toggle RC1 so that users
// can check how fast conversions are being performed.
//-----------------------------------------------------------------------------

void myTMR0ISR(void) {
    TEST_PIN_SetHigh(); // Set high when every we start a new conversion
    ADCON0bits.GO_NOT_DONE = 1; // start a new conversion
    uint8_t micReadInVal = ADRESH;
    static uint16_t a = 0;
//    TMR0_CounterSet(0x10000 - (1600 - TMR0_CounterGet()));
    
    switch(myTRM0states) {
        case MIC_IDLE:
            
            if(fillBufferAgain == true){
                myTRM0states = MIC_WAIT_FOR_TRIGGER;
            }
            break;
            
        case MIC_WAIT_FOR_TRIGGER:
            if(micReadInVal > upperLevelTrigger || micReadInVal < lowLevelTrigger) { 
                myTRM0states = MIC_ACQUIRE;
            }
            break;
            
        case MIC_ACQUIRE:
            if(a >= NUM_SAMPLES) {
                myTRM0states = MIC_IDLE;
                a=0;
            }
            fatBuffer[a] = micReadInVal;
            fillBufferAgain = false;
            a++;
            break;

    }

    TEST_PIN_SetLow(); // Monitor pulse width to determine how long we are in ISR
    

} // end myTMR0ISR