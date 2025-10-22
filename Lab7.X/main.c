// Written by Westly Bouchard and Christian Brennan for EENG 383 Fall 2025

#include "mcc_generated_files/system/system.h"
#pragma warning disable 520     // warning: (520) function "xyz" is never called  3
#pragma warning disable 1498    // fputc.c:16:: warning: (1498) pointer (unknown)
#define MAX_BUFFER_SIZE 255
#define LED_ON      25 
#define LED_OFF     0

enum EUSARTStates {IDLE, RXING_MSG, LAST_BYTE};

typedef enum { TX_IDLE, TX_START_BIT, TX_DATA_BITS, TX_STOP_BIT } TXState;

//New Custom functions
void myEUSART2ISR(void);
void transmitByteData(char letter);
void sendStartBit();
void sendStopBit();
void printBuffer();
void createWriteBuffer();
void transmitTxBuffer();

char IRrecieveBuffer[MAX_BUFFER_SIZE];
uint8_t receiveBusy = false;
uint8_t receiveNewMessage = false;
uint16_t bitPeriod[6] = {53333, 13333, 6666, 3333, 1666, 833};
uint8_t baudRateSelected = 1;
uint8_t rxBufIDX = 0;

void myTMR1ISR(void);
uint8_t transmitStart = false;
uint8_t transmitBusy = false;
char IRtransmitBuffer[MAX_BUFFER_SIZE];


uint8_t userEnter8bit(void);
void monitor(void);
void txDecode(void);
void rxDecode(void);
uint8_t sourceAddr = '1';
uint8_t destAddr = 'F';


//----------------------------------------------
// Main "function"
//----------------------------------------------

void main(void) {
    uint8_t mask;
    uint8_t i;
    char cmd;
    char letter = '0';
    
    SYSTEM_Initialize();
    
    // Delay so the Baud rate generator is stable and prints the splash screen correctly
    TMR1_CounterSet(0);
    PIR1bits.TMR1IF = 0;
    while (PIR1bits.TMR1IF == 0);
    
    TMR1_OverflowCallbackRegister(myTMR1ISR);

    //Coulston Code
	INTCONbits.RBIE = 0;
	EPWM2_LoadDutyValue(LED_OFF);
	PIE3bits.RC2IE = 0;
	EUSART2_RxCompleteCallbackRegister(myEUSART2ISR);
	PIE3bits.RC2IE = 1;
	INTERRUPT_GlobalInterruptEnable();
	INTERRUPT_PeripheralInterruptEnable();

    //End Coulston Code
    
    EPWM2_LoadDutyValue(LED_OFF);

    // Since EUSART2 is double buffered, clear out any garbage with two reads from those buffers
    if (EUSART2_IsRxReady()) (void) EUSART2_Read();
    if (EUSART2_IsRxReady()) (void) EUSART2_Read();
    
    RCSTA2bits.CREN = 0;
    RCSTA2bits.CREN = 1;

    printf("Lab 7\r\n");
    printf("Transmit and receive an IR packet\r\n");
    printf("Dev'21 Board wiring\r\n");
    printf("Install a jumper wire from RC0 to RB7 ONLY AFTER unplugging PICKit3\r\n");
    printf("Install a jumper over IR_TX header pins\r\n");
    printf("\r\n> "); // print a nice command prompt


    for (;;) {

        if (EUSART1_IsRxReady()) { // wait for incoming data on USART
            cmd = EUSART1_Read();
            switch (cmd) { // and do what it tells you to do
                    //--------------------------------------------
                    // Reply with help menu
                    //--------------------------------------------
                case '?':
                    printf("-------------------------------------------------\r\n");
                    printf("TX msg: %s\r\n", IRtransmitBuffer + 2);
                    printf("My identity: %u\r\n", sourceAddr);
                    printf("TX address: %u\r\n", destAddr);
                    printf("receiveNewMessage = %s\r\n", receiveNewMessage ? "true" : "false");
                    switch (baudRateSelected) {
                        case 0: printf("300 Baud\r\n");
                            break;
                        case 1: printf("1200 Baud\r\n");
                            break;
                        case 2: printf("2400 Baud\r\n");
                            break;
                        case 3: printf("4800 Baud\r\n");
                            break;                                           
                        case 4: printf("9600 Baud\r\n");
                            break;
                        case 5: printf("19200 Baud\r\n");
                            break;
                        default: printf("1200 Baud\r\n");
                            break;
                    }
                    printf("-------------------------------------------------\r\n");
                    printf("?: help menu\r\n");
                    printf("o: k\r\n");
                    printf("Z: Reset processor\r\n");
                    printf("z: Clear the terminal\r\n");
                    printf("b: set baud rate\r\n");
                    printf("m: create a NULL terminated message with SRC and DEST prefix\r\n");
                    printf("s: set source transmit identity\r\n");
                    printf("d: set destination transmit target\r\n");
                    printf("S: Send a message using TMR1 ISR\r\n");
                    printf("R: Receive message using EUSART2 via IR decoder\r\n");
                    printf("M: Monitor all IR traffic\r\n");
                    printf("x/X: decode tx/RX message\r\n");
                    printf("-------------------------------------------------\r\n");
                    break;

                case 'o':
                    printf("o:	ok\r\n");
                    break;
                    
                case 'Z':
                    for (i = 0; i < 40; i++) printf("\n");
                    RESET();
                    break;
                   
                case 'z':
                    for (i = 0; i < 40; i++) printf("\n");
                    break;
                 
                case 'b':
                    printf("Choose the index of the target baud rate\r\n");
                    printf("0: 300 baud\r\n");
                    printf("1: 1200 baud\r\n");
                    printf("2: 2400 baud\r\n");
                    printf("3: 4800 baud\r\n");                    
                    printf("4: 9600 baud\r\n");
                    printf("5: 19200 baud\r\n");
                    while (EUSART1_IsRxReady());
                    while (!EUSART1_IsRxReady());
                    baudRateSelected = EUSART1_Read() - '0';
                    switch (baudRateSelected) {
                        case 0: SPBRGH2 = 0xD0;
                            SPBRG2 = 0x54;
                            break;
                        case 1: SPBRGH2 = 0x34;
                            SPBRG2 = 0x14;
                            break;
                        case 2: SPBRGH2 = 0x1A;
                            SPBRG2 = 0x0A;
                            break;
                        case 3: SPBRGH2 = 0x0D;
                            SPBRG2 = 0x05;
                            break;                            
                        case 4: SPBRGH2 = 0x06;
                            SPBRG2 = 0x82;
                            break;
                        case 5: SPBRGH2 = 0x03;
                            SPBRG2 = 0x40;
                            break;
                        default: SPBRGH2 = 0x34;
                            SPBRG2 = 0x14;
                            break;
                    }
                    printf("Baud rate assigned %02x:%02x\r\n", SPBRGH2, SPBRG2);
                    break;

                case 'm':
                    createWriteBuffer();
                    break;
                    
                case 's':
                    printf("Enter source address: ");
                    sourceAddr = userEnter8bit();
                    break;
                    
                case 'd':
                    printf("Enter dest address: ");
                    destAddr = userEnter8bit();
                    break;
                    
                case 'S': {
                    printf("Transmitted\r\n");
                    printf("\tMessage:            ");
        
                    uint8_t i = 2;
                    for ( ; i < MAX_BUFFER_SIZE - 1; i++) {
                        printf("%c", IRtransmitBuffer[i]);
                        if(IRtransmitBuffer[i] == '\0') {
                            break;
                        }
                    }
        
                    printf("\r\n\tChecksum computed:  %u\r\n", IRtransmitBuffer[i + 1]);
                    printf("\tSender address:     %u\r\n", IRtransmitBuffer[0]);
                    printf("\tTarget address:     %u\r\n", IRtransmitBuffer[1]);
                    transmitStart = true;
                    break;
                }
                    
                case 'R':
                    printBuffer();
                    break;
                    
                case 'M':
                    monitor();
                    break;
                    
                case 'x':
                    txDecode();
                    break;
                    
                case 'X':
                    rxDecode();
                    break;
            
                case 'r':
                    RCSTA2bits.CREN = 0; // Try restarting EUSART2
                    RCSTA2bits.CREN = 1;
                    printf("Just reset EUSART2\r\n");
                    break;

                default:
                    printf("Unknown key %c\r\n", cmd);
                    break;

            } // end switch            
        } // end if
    } // end while 
} // end main

void printBuffer() {
    if(receiveNewMessage == true) {
        printf("Received\r\n");
        printf("\tMessage:            ");
        
        uint8_t i;
        for (i = 2 ; i < MAX_BUFFER_SIZE - 1; i++) {
            printf("%c", IRrecieveBuffer[i]);
            if(IRrecieveBuffer[i] == '\0') {
                break;
            }
        }
        
        printf("\r\n\tChecksum computed:  %u\r\n", IRrecieveBuffer[i + 1]);
        printf("\tChecksum received:  %u\r\n", IRrecieveBuffer[i + 1]);
        printf("\tSender address:     %u\r\n", IRrecieveBuffer[0]);
        printf("\tTarget address:     %u\r\n", IRrecieveBuffer[1]);
        
        receiveNewMessage = false;
    } else {
        printf("There is no new message!\r\n");
    }
}

void myEUSART2ISR(void) {
    static enum EUSARTStates rxState = IDLE;
    if(rxState == IDLE) {
        rxBufIDX = 0;
        IRrecieveBuffer[rxBufIDX] = RCREG2;
        rxState = RXING_MSG;
        receiveBusy = true;
        rxBufIDX++;
    } else if (rxState == RXING_MSG) {
        IRrecieveBuffer[rxBufIDX] = RCREG2;
        if(IRrecieveBuffer[rxBufIDX] == '\0') {
            rxState = LAST_BYTE;
        }
        rxBufIDX++;
    } else if (rxState == LAST_BYTE) {
        IRrecieveBuffer[rxBufIDX] = RCREG2;
        rxState = IDLE;
        receiveBusy = false;
        receiveNewMessage = true;
        rxBufIDX = 0;
    }
}

void createWriteBuffer() {
    uint8_t checkSum = 0;
    char cmd = 0;
    // setup before starting to read in case 'm'
    printf("Please enter your message\r\n>");
    IRtransmitBuffer[0] = sourceAddr;
    IRtransmitBuffer[1] = destAddr;
	while (EUSART1_IsRxReady()); // waiting for 'm' to clear
	for (uint8_t i = 2; i < MAX_BUFFER_SIZE - 1; i++) {
		while (!EUSART1_IsRxReady()); // waiting until character is received
		cmd = EUSART1_Read();
        if(cmd == '\r') {
            IRtransmitBuffer[i] = '\0';
            IRtransmitBuffer[i+1] = checkSum;
            break;
        } else {
            IRtransmitBuffer[i] = cmd;
            checkSum += (uint8_t)cmd;
            printf("%c",cmd);
        }
		// leave loop if recieve '\r'
		// append cmd to buffer, add to checksum, print cmd
    }
    printf("\n\rThank you message has been stored\r\n");
}

uint8_t userEnter8bit(void) {
    uint8_t ret = 0;
    char cmd = 0;
    while(EUSART1_IsRxReady());
    for (uint8_t i = 0; i < 3; i++) {
        while (!EUSART1_IsRxReady());
        cmd = EUSART1_Read();
        if (cmd == '\r') break;
        else {
            ret *= 10;
            ret += (cmd - '0');
            printf("%c", cmd);
        }
    }
    
    printf("\r\n");
    return ret;
}

void myTMR1ISR(void) {
    static TXState state = TX_IDLE;
    static uint8_t mask = 0b00000001;
    
    static bool checkSumSent = false;
    
    static uint8_t index = 0;
    
    switch (state) {
        case TX_IDLE: {
            if (transmitStart) {
                state = TX_START_BIT;
                index = 0;
                checkSumSent = false;
            }
            break;
        }
        
        case TX_START_BIT: {
            EPWM2_LoadDutyValue(LED_ON);
            TMR1_CounterSet(TMR1_MAX_COUNT - bitPeriod[baudRateSelected]);
            state = TX_DATA_BITS;
            break;
        }
        
        case TX_DATA_BITS: {
            
            if ((IRtransmitBuffer[index] & mask) != 0) EPWM2_LoadDutyValue(LED_OFF);
            else EPWM2_LoadDutyValue(LED_ON);
            
            mask = mask << 1;
            TMR1_CounterSet(TMR1_MAX_COUNT - bitPeriod[baudRateSelected]);
            
            if (mask == 0) {
                mask = 0b00000001;
                state = TX_STOP_BIT;
            }
            
            break;
        }
        
        case TX_STOP_BIT: {
            EPWM2_LoadDutyValue(LED_OFF);
            
            if (checkSumSent) {
                state = TX_IDLE;
                transmitStart = false;
                break;
            }
            
            if (IRtransmitBuffer[index] == '\0') {
                checkSumSent = true;
            }
            
            state = TX_START_BIT;
            index++;
            break;
        }
    }
}

void monitor(void) {
    printf("Hit any key to exit\r\n");
    printf("SRC DEST    CHECK   MESSAGE\r\n");
   
    while (true) {
        if (receiveNewMessage) {
            uint8_t i;
            for (i = 0; i < MAX_BUFFER_SIZE; i++) {
                if (IRrecieveBuffer[i] == '\0') {
                    i++;
                    break;
                }
            }
    
            printf("%3u %3u     %5u   %s\r\n",
                    IRrecieveBuffer[0],
                    IRrecieveBuffer[1],
                    IRrecieveBuffer[i],
                    IRrecieveBuffer + 2);
            
            receiveNewMessage = false;
        }
        
        if (EUSART1_IsRxReady()) {
            EUSART1_Read();
            break;
        }
    }
}

void txDecode(void) {
    printf("TX buffer contents\r\n");
    for (uint8_t i = 0; i < MAX_BUFFER_SIZE; i++) {
        printf("\t%2u:     %2x    %c\r\n", i, IRtransmitBuffer[i], IRtransmitBuffer[i]);
        
        if (i != 0 && IRtransmitBuffer[i - 1] == '\0') break;
    }
}

void rxDecode(void) {
    printf("RX buffer contents\r\n");
    for (uint8_t i = 0; i < MAX_BUFFER_SIZE; i++) {
        printf("\t%2u:     %2x    %c\r\n", i, IRrecieveBuffer[i], IRrecieveBuffer[i]);
        
        if (i != 0 && IRrecieveBuffer[i - 1] == '\0') break;
    }
}