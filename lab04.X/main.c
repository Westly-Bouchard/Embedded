//*****************************************************************
// Name:    Westly Bouchard & Christian Brennan
// Date:    Fall 2025
// Lab:     04
// Purp:    Play a song with interrupts.
//
//
// Academic Integrity Statement: I certify that, while others may have
// assisted me in brain storming, debugging and validating this program,
// the program itself is my own work. I understand that submitting code
// which is the work of other individuals is a violation of the course
// Academic Integrity Policy and may result in a zero credit for the
// assignment, course failure and a report to the Academic Dishonesty
// Board. I also understand that if I knowingly give my original work to
// another individual that it could also result in a zero credit for the
// assignment, course failure and a report to the Academic Dishonesty
// Board.
//*****************************************************************

#include "mcc_generated_files/system/system.h"
#pragma warning disable 520     // warning: (520) function "xyz" is never called  
#pragma warning disable 1498    // fputc.c:16:: warning: (1498) pointer (unknown)

#define NUM_OCTAVES 2

#define C4  0
#define CS4 1
#define D4  2
#define DS4 3
#define E4  4
#define F4  5
#define FS4 6
#define G4  7
#define GS4 8
#define A4  9
#define AS4 10
#define B4  11

#define C5  12
#define CS5 13
#define D5  14
#define DS5 15
#define E5  16
#define F5  17
#define FS5 18
#define G5  19
#define GS5 20
#define A5  21
#define AS5 22
#define B5  23

#define WHOLE           62500
#define HALF            31250
#define QUARTER         15625
#define EIGHTH          7813
#define SIXTEENTH       3906
#define THIRTYSECOND    1953

uint16_t scale[NUM_OCTAVES * 12] = {
    30600, // C4
    28883, // C#4
    27262, // D4
    25732, // D#4
    24288, // E4
    22925, // F4
    21638, // F#4
    20424, // G4
    19277, // G#4
    18195, // A4
    17174, // A#4
    16210, // B4
    
    15300, // C5
    14442, // C#5
    13631, // D5
    12866, // D#5
    12144, // E5
    11462, // F5
    10819, // F#5
    10212, // G5
    9639,  // G#5
    9098,  // A5
    8587,  // A#5
    8105   // B5
};

#define NUM_SONGS 6
#define SONG_MAX_LEN 20

// One array for each song
// Points to two arrays, one for notes, one for durations
uint16_t songs[NUM_SONGS][2][SONG_MAX_LEN] = {
    // Song 0 (Song of Storms)
    {
        // Notes
        {D4, F4, D5, D4, F4, D5, E5, F5, E5, F5, E5, C5, A4},
        // Durations
        {
            EIGHTH, EIGHTH, HALF, EIGHTH, EIGHTH, HALF, QUARTER * 1.5,
            EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF
        }
    },
    // Song 1 (Zelda's Lullaby)
    {
        // Notes
        {B4, D5, A4, B4, D5, A4, B4, D5, A5, G5, D5},
        // Durations
        {WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE, HALF, WHOLE}
    },
    // Song 2 (Song of Time)
    {
        // Notes
        {A4, D4, F4, A4, D4, F4, A4, C5, B4, G4, F4, G4, A4, D4, C4, E4, D4},
        // Durations
        {
            HALF, WHOLE, HALF, HALF, WHOLE, HALF, QUARTER, QUARTER, HALF, HALF,
            QUARTER, QUARTER, HALF, HALF, QUARTER, QUARTER, HALF
        }
    },
    // Song 3 (TEST SONG)
    {
        // Notes
        {A4, D4, F4},
        // Durations
        {HALF, HALF, HALF}
    },
    // Song 4 (FAILURE SONG)
    {
        // Notes
        {G4, C4},
        // Durations
        {HALF, WHOLE}
    },
    // Song 5 (SUCCESS SONG)
    {
        // Notes
        {C4, E4, G4, C5},
        // Durations
        {EIGHTH, EIGHTH, EIGHTH, HALF}
    },
};

// Variables to control program state
uint8_t playNote = false;
uint8_t incNote = false;

uint8_t playingSong = false;

uint8_t songNumber = 0;
uint8_t noteIndex = 0;

// Interrupt call-backs
void tmr1ISR(void);
void tmr0ISR(void);

// Delta used for rhythm practice
#define DELTA 9375

// Rhythm practice functionality
void rhythmPractice(void);

// Splash screen, gets printed on reset
char *ascii = "\
         #         \r\n\
        ###        \r\n\
       #####       \r\n\
      #######      \r\n\
     #########     \r\n\
    #         #    \r\n\
   ###       ###   \r\n\
  #####     #####  \r\n\
 #######   ####### \r\n\
######### #########\r\n\
";

//*****************************************************************
//*****************************************************************

void main(void) {

    uint8_t i;
    char cmd;

    SYSTEM_Initialize();

    // Not necessary, but this delay allows the baud rate generator to 
    // stabilize before printing the splash screen on reset. If you are going to
    // do this, then make sure to put delay BEFORE enabling TMR interrupt.
    TMR1_CounterSet(0x0000);
    PIR1bits.TMR1IF = 0;
    while (PIR1bits.TMR1IF == 0);

    // Register interrupts
    TMR0_OverflowCallbackRegister(tmr0ISR);
    TMR1_OverflowCallbackRegister(tmr1ISR);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    printf("\r\n%s", ascii);
    printf("Lab 04\r\n");
    printf("Interrupt Music Box\r\n");
    printf("Dev'21\r\n");
    printf("Board wiring\r\n");
    printf("RB5 -> LPFin and install LPFout/AMPin jumper");
    printf("\r\n> "); // print a nice command prompt

    for (;;) {

        if (EUSART1_IsRxReady()) { // wait for incoming data on USART
            cmd = EUSART1_Read();
            switch (cmd) { // and do what it tells you to do
                case '?':
                    printf("\r\n%s", ascii);
                    printf("------------------------------\r\n");
                    printf("        TMR1 = 0x%04x\r\n", TMR1_CounterGet());
                    printf("------------------------------\r\n");
                    printf("?: Help menu\r\n");
                    printf("o: k\r\n");
                    printf("Z: Reset processor\r\n");
                    printf("z: Clear the terminal\r\n");
                    printf("p: play song once\r\n");
                    printf("r: Rhythm practice\r\n");
                    printf("------------------------------\r\n");
                    break;

                    //--------------------------------------------
                    // Simple handshake with the development board
                    //--------------------------------------------                                          
                case 'o':
                    printf("k.\r\n");
                    break;

                    //--------------------------------------------
                    // Reset the processor after clearing the terminal
                    //--------------------------------------------                      
                case 'Z':
                    for (i = 0; i < 40; i++) printf("\r\n");
                    RESET();
                    break;

                    //--------------------------------------------
                    // Clear the terminal
                    //--------------------------------------------                      
                case 'z':
                    for (i = 0; i < 40; i++) printf("\r\n");
                    break;

                    //--------------------------------------------
                    // Play song once
                    //--------------------------------------------                      
                case 'p':
                    printf("Playing song %u!\r\n", songNumber);
                    playingSong = true;
                    break;
                    
                    //--------------------------------------------
                    // Rhythm practice
                    //--------------------------------------------                      
                case 'r':
                    printf("Rhythm practice!\r\n");
                    rhythmPractice();
                    break;
                    
                    //--------------------------------------------
                    // If something unknown is hit, tell user
                    //--------------------------------------------
                default:
                    printf("Unknown key %c\r\n", cmd);
                    break;
            } // end switch
        } // end if        
    } // end infinite loop    
} // end main



//*****************************************************************
//*****************************************************************

void tmr1ISR(void) {
    // If playing a note, set the count for the proper frequency and
    // toggle the speaker pin
    if (playNote) {
        TMR1_CounterSet(TMR1_MAX_COUNT - scale[songs[songNumber][0][noteIndex]]);
        SPEAKER_PIN_Toggle();
    }
}

void tmr0ISR(void) {
    static uint8_t wasResting = true;
    
    if (playingSong) {
        if (wasResting) {
            wasResting = false;
            
            playNote = true;
            TMR0_CounterSet(TMR0_MAX_COUNT - songs[songNumber][1][noteIndex]);
        } else {
            wasResting = true;
            
            playNote = false;
            
            SPEAKER_PIN_SetLow();
            
            noteIndex++;
            
            // Check if song is done:
            if (noteIndex == SONG_MAX_LEN) {
                playingSong = false;
                noteIndex = 0;
                songNumber = (songNumber + 1) % 3;
            }
            
            TMR0_CounterSet(TMR0_MAX_COUNT - THIRTYSECOND);
        }
    }
}

void rhythmPractice(void) {
    // If anything was playing, stop it
    playingSong = false;
    playNote = false;
    
    // Give instructions
    printf("Listen to the beat pattern. Press any key to continue.\r\n");
    while (!EUSART1_IsRxReady()); // Wait for key press
    (void) EUSART1_Read(); // read character to clear flag
    songNumber = 3;
    playingSong = true;
    while (playingSong);
    
    // Disable timer 0 interrupts so we can use it to measure durations
    TMR0_TMRInterruptDisable();
    
    printf("Use the upper button to reproduce this pattern.\r\n");
    
    // Reset song number because the interrupt will automatically increment it
    songNumber = 3;
    // User input
    for (uint8_t i = 0; i < 3; i++) {
        // Reset timer
        TMR0_Stop();
        TMR0_CounterSet(0);
        
        // Wait for user to press button
        while (TOP_BUTTON_GetValue());
        noteIndex = i;
        playNote = true;
        TMR0_Start();
        while (!TOP_BUTTON_GetValue());
        TMR0_Stop();
        playNote = false;
        
        uint16_t duration = TMR0_CounterGet();
        
        // If the duration was outside of the range, play failure music
        if (duration < songs[3][1][i] - DELTA || duration > songs[3][1][i] + DELTA) {
            printf("You lose!\r\n");
            TMR0_TMRInterruptEnable();
            TMR0_Start();
            songNumber = 4;
            noteIndex = 0;
            playingSong = true;
            return;
        }
    } // End for loop
    
    // If the user successfully made it through the challenge, play victory song
    printf("Winner!\r\n");
    TMR0_TMRInterruptEnable();
    TMR0_Start();
    songNumber = 5;
    noteIndex = 0;
    playingSong = true;
}