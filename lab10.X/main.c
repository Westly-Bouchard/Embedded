#include "mcc_generated_files/pwm/eccp1.h"
#include "mcc_generated_files/system/system.h"

#include "mcc_generated_files/uart/eusart1.h"
#include "sdCard.h"
#include <pic18f25k22.h>
#pragma warning disable 520     // warning: (520) function "xyz" is never called  3
#pragma warning disable 1498    // fputc.c:16:: warning: (1498) pointer (unknown)


void initSDCard(){
    SPI2_Close();
    SPI2_Open(CLASSIC_FWPORT); // Reset the SPI channel for SD card communication
    SDCARD_Initialize(true);
}

void myTMR0ISR(void);


#define BLOCK_SIZE          512
#define MAX_NUM_BLOCKS      128

// Large arrays need to be defined as global even though you may only need to 
// use them in main.  This quirk will be important in the next two assignments.
uint16_t redIdx, blueIdx;
uint8_t blueBuffer[BLOCK_SIZE];
uint8_t redBuffer[BLOCK_SIZE];
    

typedef enum { IDLE, R_FILL_B_WRITE, B_FILL_R_WRITE, R_FILL_B_PLAY, B_FILL_R_PLAY } state;

state systemState = IDLE;

#define TEN_US 160

// Default rate is 100us
uint16_t sampleRate = 1600;

#define SINE_WAVE_ARRAY_LENGTH 26
const uint8_t   sin[SINE_WAVE_ARRAY_LENGTH] = {128, 159, 187, 213, 233, 248, 255, 255, 248, 233, 213, 187, 159, 128, 97, 69, 43, 23, 8, 1, 1, 8, 23, 43, 69, 97};

//----------------------------------------------
// Main "function"
//----------------------------------------------

void main(void) {
    uint8_t status;
    uint16_t i, blockCount;
    uint32_t sdCardAddress = 0x00000000;
    char cmd, letter;

    letter = '0';

    SYSTEM_Initialize();
    EPWM1_LoadDutyValue(0);
    CS_SetHigh();

    // Provide Baud rate generator time to stabilize before splash screen
    TMR0_CounterSet(0x0000);
    INTCONbits.TMR0IF = 0;
    while (INTCONbits.TMR0IF == 0);

    TMR0_OverflowCallbackRegister(myTMR0ISR);

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    printf("inLab 09\r\n");
    printf("SD card testing\r\n");
    printf("Dev'21\r\n");

    SPI2_Close();
    SPI2_Open(CLASSIC_FWPORT);

    initSDCard();
    for (;;) {

        if (EUSART1_IsRxReady()) { // wait for incoming data on USART
            cmd = EUSART1_Read();
            switch (cmd) { // and do what it tells you to do

                    //--------------------------------------------
                    // Reply with help menu
                    //--------------------------------------------
                case '?':
                    printf("\r\n-------------------------------------------------\r\n");
                    printf("SD card address:  ");
                    printf("%04x", sdCardAddress >> 16);
                    printf(":");
                    printf("%04x", sdCardAddress & 0X0000FFFF);
                    printf("\r\n");
                    printf("-------------------------------------------------\r\n");
                    printf("?: help menu\r\n");
                    printf("o: k\r\n");
                    printf("Z: Reset processor\r\n");
                    printf("z: Clear the terminal\r\n");
                    printf("-------------------------------------------------\r\n");
                    printf("i: Initialize SD card\r\n");
                    printf("-------------------------------------------------\r\n");
                    printf("a/A decrease/increase read address\r\n");
                    printf("r: read a block of %d bytes from SD card\r\n", BLOCK_SIZE);
                    printf("1: write perfect 26 value sine wave to 128 blocks\r\n");
                    printf("-------------------------------------------------\r\n");
                    printf("+/-: Increase/Decrease the sample rate by 10 us\r\n");
                    printf("W: Write microphone => SD card at 1600 us\r\n");
                    printf("s: spool memory to a csv file\r\n");
                    printf("-------------------------------------------------\r\n");
                    break;

                    //--------------------------------------------
                    // Reply with "k", used for PC to PIC test
                    //--------------------------------------------
                case 'o':
                    printf("o:	ok\r\n");
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

                    //-------------------------------------------- 
                    // Init SD card to get it read to perform read/write
                    // Will hang in infinite loop on error.
                    //--------------------------------------------    
                case 'i':
                    SPI2_Close();
                    SPI2_Open(CLASSIC_FWPORT); // Reset the SPI channel for SD card communication
                    SDCARD_Initialize(true);
                    break;

                    //--------------------------------------------
                    // Increase or decrease block address
                    //--------------------------------------------                 
                case 'A':
                case 'a':
                    if (cmd == 'a') {
                        sdCardAddress -= BLOCK_SIZE;
                        if (sdCardAddress >= 0x04000000) {
                            printf("Underflowed to high address\r\n");
                            sdCardAddress = 0x04000000 - BLOCK_SIZE;
                        } else {
                            printf("Decreased address\r\n");
                        }
                    } else {
                        sdCardAddress += BLOCK_SIZE;
                        if (sdCardAddress >= 0x04000000) {
                            printf("Overflowed to low address\r\n");
                            sdCardAddress = 0x00000000;
                        } else {
                            printf("Increased address\r\n");
                        }
                    }

                    // 32-bit integers need printed as a pair of 16-bit integers
                    printf("SD card address:  ");
                    printf("%04x", sdCardAddress >> 16);
                    printf(":");
                    printf("%04x", sdCardAddress & 0X0000FFFF);
                    printf("\r\n");
                    break;

                    //--------------------------------------------
                    // w: write a block of BLOCK_SIZE bytes to SD card
                    //--------------------------------------------
                    
                case '0': {
                    printf("SD Card address has been reset");
                    sdCardAddress = 0;
                    break;
                }

                    //--------------------------------------------
                    // r: read a block of BLOCK_SIZE bytes from SD card                
                    //--------------------------------------------
                case 'r': {
                    SDCARD_ReadBlock(sdCardAddress, blueBuffer);
                    printf("Read block: \r\n");
                    printf("    Address:    ");
                    printf("%04x", sdCardAddress >> 16);
                    printf(":");
                    printf("%04x", sdCardAddress & 0X0000FFFF);
                    printf("\r\n");
                    hexDumpBuffer(blueBuffer);
                    break;
                }
                    
                case '1': {
                    uint16_t sineIdx = 0;
                    uint16_t blockCount = 0;
                    for (blockCount = 0; blockCount < 5; blockCount++) {
                        // For loop to fill the red buffer with 512 sine wave samples
                        for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
                            blueBuffer[i] = sin[sineIdx];
                            sineIdx = (sineIdx + 1) % 26;
                        }
                        
                        SDCARD_WriteBlock(sdCardAddress, blueBuffer);
                        while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE);
                        
                        // Slow down the write sequence to ensure that blocks are written
                        TMR1_CounterSet(0x0000);
                        PIR1bits.TMR1IF = 0;
                        while (PIR1bits.TMR1IF == 0);
                        
                        // Update sdCardAddress
                        sdCardAddress += BLOCK_SIZE;
                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }
                   
                    }
                    
                    
                    // Tell user  how many blocks were written
                    printf("Stored %u blocks of sine wave\r\n", blockCount);
                    break;
                }
                    
                case '+':
                case '-':
                    if (cmd == '+') {
                        sampleRate += TEN_US;
                    } else if (sampleRate - TEN_US >= 2 * TEN_US) {
                        sampleRate -= TEN_US;
                    } else {
                        printf("Can't go lower than 20us sampling rate\r\n");
                    }
                    break;
                    
                case 'D' :
                    for(uint16_t i = 0; i < BLOCK_SIZE; i++) {
                        blueBuffer[i] = 0;
                    }
                    while(true){
                        SDCARD_WriteBlock(sdCardAddress, blueBuffer);
                        while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE);
                        TMR1_CounterSet(0x0000);
                        PIR1bits.TMR1IF = 0;
                        while (PIR1bits.TMR1IF == 0);
                        // Increment address
                        sdCardAddress += BLOCK_SIZE;
                        
                    }
                    break;
                    
                case 'W': {
                    printf("Press any key to start recording audio and press any key to stop recording.\r\n");
                    
                    // Wait for key press
                    while (!EUSART1_IsRxReady());
                    char uuuu = EUSART1_Read();
                    printf("Now saving data.");
                    // Signal to start filling buffer
                    redIdx = 0;
                    blueIdx = 0;
                    systemState = R_FILL_B_WRITE;
                    
                    for (;;) {
                        // Wait for red buffer to fill
                        while (systemState == R_FILL_B_WRITE);
                        
                        // Write red buffer to SD card
                        

                       SDCARD_WriteBlock(sdCardAddress, redBuffer);
                       while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE);
                        TMR1_CounterSet(0x0000);
                        PIR1bits.TMR1IF = 0;
                        while (PIR1bits.TMR1IF == 0);
                        
                        // Increment address
                        sdCardAddress += BLOCK_SIZE;
                        
                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }
                        
                        // Wait for blue buffer to fill
                        while (systemState == B_FILL_R_WRITE);
                        
                        // Write blue buffer to SD card
                      SDCARD_WriteBlock(sdCardAddress, blueBuffer);
                      while ((status = SDCARD_PollWriteComplete()) == WRITE_NOT_COMPLETE);
                        TMR1_CounterSet(0x0000);
                        PIR1bits.TMR1IF = 0;
                        while (PIR1bits.TMR1IF == 0);
                        
                        sdCardAddress += BLOCK_SIZE;
                        
                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }
                        
                        // Break out of loop if user enters character
                        if (EUSART1_IsRxReady()) {
                            char _ = EUSART1_Read();
                            printf("Recording complete");
                            systemState = IDLE;
                            
                            break;
                        }
                    }
                    break;
                }
                    
                case 's': {
                    printf("You may terminate spooling at any time with a keypress.\r\n");
                    printf("To spool terminal contents into a file follow these instructions:\r\n");
                    printf("Right mouse click on the upper left of the PuTTY window\r\n");
                    printf("Select:\tChange settings...\r\n");
                    printf("Select:\tLogging\r\n");
                    printf("Select:\tSession logging: All session output\r\n");
                    printf("Log file name:   Browse and provide a .csv extension to your file name\r\n");
                    printf("Select:\tApply");
                    printf("Press any key to start\r\n");
                    
                    while (!EUSART1_IsRxReady());
                    char _ = EUSART1_Read();
                    
                    for (uint16_t i = 0; i < MAX_NUM_BLOCKS; i++) {
                        SDCARD_ReadBlock(sdCardAddress, blueBuffer);
                        sdCardAddress += BLOCK_SIZE;
                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }
                        
                        for (uint16_t j = 0; j < BLOCK_SIZE; j++) {
                            printf("%u\r\n", blueBuffer[j]);
                            
                            if (EUSART1_IsRxReady()) {
                            (void) EUSART1_Read();
                            break;
                        }

                        }
                        
                        
                    }
                    
                    printf("To close the file follow these instructions:\r\n\r\n");
                    printf("Right mouse click on the upper left of the PuTTY window\r\n");
                    printf("Select:\tChange settings...\r\n");
                    printf("Select:\tLogging\r\n");
                    printf("Select:\tSession logging: None\r\n");
                    printf("Select:\tApply\r\n");
                    break;
                }

                case 'P': {
                    printf("Press any key to stop playback\r\n");
                    redIdx = 0;
                    blueIdx = 0;

                    // Read the first block
                    SDCARD_ReadBlock(sdCardAddress, blueBuffer);

                    sdCardAddress += BLOCK_SIZE;

                    if (sdCardAddress >= 0x04000000) {
                        sdCardAddress = 0x00000000;
                    }

                    systemState = R_FILL_B_PLAY;

                    for (;;) {
                        // Fill red buffer
                        SDCARD_ReadBlock(sdCardAddress, redBuffer);

                        sdCardAddress += BLOCK_SIZE;

                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }

                        // Await state change
                        while (systemState == R_FILL_B_PLAY);

                        // Fill blue buffer
                        SDCARD_ReadBlock(sdCardAddress, blueBuffer);

                        sdCardAddress += BLOCK_SIZE;

                        if (sdCardAddress >= 0x04000000) {
                            sdCardAddress = 0x00000000;
                        }

                        // Await state change
                        while (systemState == B_FILL_R_PLAY);

                        if (EUSART1_IsRxReady()) {
                            char _ = EUSART1_Read();
                            systemState = IDLE;

                            break;
                        }
                    }
                    break;
                }
                    
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


//----------------------------------------------
// As configured, we are hoping to get a toggle
// every 100us - this will require some work.
//
// You will be starting an ADC conversion here and
// storing the results (when you reenter) into a global
// variable and setting a flag, alerting main that 
// it can read a new value.
//
// !!!MAKE SURE THAT TMR0 has 0 TIMER PERIOD in MCC!!!!
//----------------------------------------------
#define WASTING_TIME    true
#define BASIC_TIME      false

void myTMR0ISR(void) {
    switch (systemState) {
        case IDLE:
            break;
            
        case R_FILL_B_WRITE:
        case B_FILL_R_WRITE: {
            // Start new conversion
            ADCON0bits.GO_NOT_DONE = 1;
            uint8_t micReadInVal = ADRESH;
            
            // If currently filling red buffer write to red
            if (systemState == R_FILL_B_WRITE) {
                redBuffer[redIdx] = micReadInVal;
                redIdx++;
                if (redIdx == BLOCK_SIZE) {
                    // Swap buffers if necessary
                    redIdx = 0;
                    systemState = B_FILL_R_WRITE;
                }
            // If not writing to red, write to blue
            } else {
                blueBuffer[blueIdx] = micReadInVal;
                blueIdx++;
                if (blueIdx == BLOCK_SIZE) {
                    // Swap buffers if necessary
                    blueIdx = 0;
                    systemState = R_FILL_B_WRITE;
                }
            }
            break;
        }
            
        case R_FILL_B_PLAY: {
            EPWM1_LoadDutyValue(blueBuffer[blueIdx]);
            blueIdx++;

            if (blueIdx == BLOCK_SIZE) {
                blueIdx = 0;
                systemState = B_FILL_R_PLAY;
            }
            break;
        }

        case B_FILL_R_PLAY: {
            EPWM1_LoadDutyValue(redBuffer[redIdx]);
            redIdx++;

            if (redIdx == BLOCK_SIZE) {
                redIdx = 0;
                systemState = R_FILL_B_PLAY;
            }
        }
    }
    
    // Set up next interrupt]
    TMR0_CounterSet(TMR0_CounterGet() + (0x10000 - sampleRate));
    INTCONbits.TMR0IF = 0;
}

/* end of file */