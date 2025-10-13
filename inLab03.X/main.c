//*****************************************************************
// Name:    Westly Bouchard & Christian Brennan
// Date:    Fall 2025
// Lab:     03
// Purp: Play a song
//
// Assisted:
// Assisted by: Technical Documents
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

#define NUM_OCTAVES 2
#define SONG_LENGTH 42

#define E4 1
#define F4 2
#define G4 4
#define A5 6
#define B5 8
#define C5 9
#define D5 11
#define E5 13
#define F5 14
#define G5 16
#define A6 18
#define B6 20
#define C6 21
#define D6 23

#define WHOLE 62500
#define HALF 31250
#define QUARTER 15625
#define EIGHTH 7813
#define SIXTEENTH 3906
#define THIRTYSECOND 1953

void main(void) {

    uint16_t scale[NUM_OCTAVES * 12] = {
        12864, // D#4
        12144, // E4
        11472, // F4
        10816, // F#4
        10208, // G4
        9632,  // G#4
        9088,  // A5
        8592,  // A#5
        8112,  // B5
        7648,  // C5
        7232,  // C#5
        6816,  // D5
        6432,  // D#5
        6080,  // E5
        5728,  // F5
        5408,  // F#5
        5104,  // G5
        4816,  // G#5
        4560,  // A6
        4288,  // A#6
        4048,  // B6
        3824,  // C6
        3616,  // C#6
        3408,  // D6
    };
    
    uint16_t notes[SONG_LENGTH] = {
        G5, G5, D5, D5, E5, E5, D5,
        C5, C5, B5, B5, A5, A5, G5,
        D5, D5, C5, C5, B5, B5, A5,
        D5, D5, C5, C5, B5, B5, A5,
        G5, G5, D5, D5, E5, E5, D5,
        C5, C5, B5, B5, A5, A5, G5
    };
    
    uint16_t durations[SONG_LENGTH] = {
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
        QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF
    };
    
    
    SYSTEM_Initialize();
    
    for (;;) {   
        if (TOP_BUTTON_GetValue() == 0) {
            while (TOP_BUTTON_GetValue() == 0);
            for (int i = 0; i < SONG_LENGTH; i++) {
                Timer0_OverflowStatusClear();
                Timer0_CounterSet(TMR0_MAX_COUNT - durations[i]);
                while (Timer0_OverflowStatusGet() == false) {
                    Timer1_OverflowStatusClear();
                    Timer1_CounterSet(TMR1_MAX_COUNT - scale[notes[i]]);
                    while(Timer1_OverflowStatusGet() == false);
                    SPEAKER_PIN_Toggle();
                }
                SPEAKER_PIN_SetLow();
                Timer0_OverflowStatusClear();
                Timer0_CounterSet(TMR0_MAX_COUNT - THIRTYSECOND);
                while(Timer0_OverflowStatusGet() == false);
            }
        }

    } // end infinite loop
} // end main
