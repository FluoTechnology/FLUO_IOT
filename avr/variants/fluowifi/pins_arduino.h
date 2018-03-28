#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

//SPI
#define PIN_SPI_SS   (10)

#define PIN_SPI_MISO (14)
#define PIN_SPI_SCK  (15)
#define PIN_SPI_MOSI (16)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

//I2C
#define PIN_WIRE_SDA (17)
#define PIN_WIRE_SCL (18)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

//FLUOTUBE INT
#define TX_VIRTUALSERIAL  20
#define RX_VIRTUALSERIAL  21

static const uint8_t esp32Interrupt = 30;

//ADC
#define PIN_A0 (24)
#define PIN_A1 (25)
#define PIN_A2 (26)
#define PIN_A3 (27)
#define PIN_A4 (28)
#define PIN_A5 (29)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;

//LED
#define LED_BUILTIN 13


// ATMEL ATMEGA644P
//
//                        +---\/---+
//            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
//            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
//       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
//        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
//     SS PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
//       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
//       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
//        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
//                  RST  9|        |32  AREF
//                  VCC 10|        |31  GND
//                  GND 11|        |30  AVCC
//                XTAL2 12|        |29  PC7 (D 23)
//                XTAL1 13|        |28  PC6 (D 22)
//       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
//       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
//  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
//  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
//       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
//       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
//       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
//                        +--------+
//
#define NUM_DIGITAL_PINS            31

#define NUM_ANALOG_INPUTS           5

#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? 29 - (p): -1)
#define analogPinToChannel(p)       ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 24 ? 29 - (p) : -1)

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11 )


#define digitalPinToPCICR(p)        (((p) >= 0 && (p) <= 31) ? (&PCICR) : ((uint8_t *)0))

#define digitalPinToPCICRbit(p)     ( (((p) >=  0) && ((p) <=  6)) ? 3 : \
                                    ( ((p) == 7) ? 2 :  \
                                    ( ((p) == 8) ? 1 :  \ 
                                    ( ((p) == 9) ? 3 :  \
                                    ( (((p) >= 10) && ((p) <=  13)) ? 1 : \
                                    ( ((p) == 20) ? 2 :  \
                                    ( ((p) == 21) ? 2 :  \
                                    ( ((p) == 30) ? 0 : \
                                    0 ) ) ) ) ) ) ) )


#define digitalPinToPCMSK(p)        ( (((p) >=  0) && ((p) <=  6)) ? (&PCMSK3) : \
                                    ( ((p) == 7) ? (&PCMSK2) :  \
                                    ( ((p) == 8) ? (&PCMSK1) :  \ 
                                    ( ((p) == 9) ? (&PCMSK3) :  \
                                    ( (((p) >= 10) && ((p) <=  13)) ? (&PCMSK1) : \
                                    ( ((p) == 20) ? (&PCMSK2) : \
                                    ( ((p) == 21) ? (&PCMSK2) : \
                                    ( ((p) == 30) ? (&PCMSK0) : \
                                    ((uint8_t *)0) ) ) ) ) ) ) ) )


#define digitalPinToPCMSKbit(p) ( ((p) == 0) ? 0: \
                                ( ((p) == 1) ? 1 : \
                                ( ((p) == 2) ? 2 : \
                                ( ((p) == 3) ? 4 : \
                                ( ((p) == 4) ? 3 : \
                                ( ((p) == 5) ? 5 : \
                                ( ((p) == 6) ? 6 : \
                                ( ((p) == 7) ? 6 : \
                                ( ((p) == 8) ? 2 : \
                                ( ((p) == 9) ? 7 : \
                                ( ((p) == 10) ? 4 : \
                                ( ((p) == 11) ? 3 : \
                                ( ((p) == 12) ? 0 :  \
                                ( ((p) == 13) ? 1 :  \
                                ( ((p) == 20) ? 4 : \
                                ( ((p) == 21) ? 5 : \
                                ( ((p) == 30) ? 7 :  \
                                0 ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) )  

#define digitalPinToInterrupt(p)    ((p) == 2 ? 0 : ((p) == 4 ? 1 : ((p) == 8 ? 2 : NOT_AN_INTERRUPT)))

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
        NOT_A_PORT,
        (uint16_t) &DDRA,
        (uint16_t) &DDRB,
        (uint16_t) &DDRC,
        (uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
        NOT_A_PORT,
        (uint16_t) &PORTA,
        (uint16_t) &PORTB,
        (uint16_t) &PORTC,
        (uint16_t) &PORTD,
};
const uint16_t PROGMEM port_to_input_PGM[] =
{
        NOT_A_PORT,
        (uint16_t) &PINA,
        (uint16_t) &PINB,
        (uint16_t) &PINC,
        (uint16_t) &PIND,
};
const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
        PD, /* 0 - GPIO */
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PC,
        PB,
        PD,
        PB,
        PB,
        PB,
        PB, /* 13 - GPIO */ 

        PB, /* 14 MISO*/
        PB, /* 15 SCK*/
        PB, /* 16 MOSI*/
        PC, /* 17 SDA*/
        PC, /* 18 SCL*/
        PD, /* 19 ---*/

        PC, /* 20 TX Virt */
        PC, /* 21 RX Virt */
        PD, /* 22 --- */
        PD, /* 23 --- */

        PA, /* 24 ADC 0 */
        PA, /* 25 ADC 1 */
        PA, /* 26 ADC 2 */
        PA, /* 27 ADC 3 */
        PA, /* 28 ADC 4 */
        PA, /* 29 ADC 5 */

        PA, /* 30 TubeInterrupt */
        PD  /* 31 */
};
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
        _BV(0), /* 0 */
        _BV(1),
        _BV(2),
        _BV(4),
        _BV(3),
        _BV(5),
        _BV(6),
        _BV(6),
        _BV(2), /* 8 */
        _BV(7),
        _BV(4),
        _BV(3),
        _BV(0),
        _BV(1), /* 13 */

        _BV(6), /* 14 SPI*/
        _BV(7), /* 15 SPI*/
        _BV(5), /* 16 SPI*/
        _BV(1), /* 17 I2C*/
        _BV(0), /* 18 I2C*/
        _BV(7), // ---

        _BV(4), /* 20 Virt*/
        _BV(5), /* 21 Virt*/
        _BV(7), // ---
        _BV(7), // ---

        _BV(5), /* 24 ADC A0 */
        _BV(4), /* 25 ADC A1 */
        _BV(3), /* 26 ADC A2 */
        _BV(2), /* 27 ADC A3 */
        _BV(1), /* 28 ADC A4 */
        _BV(0), /* 29 ADC A5 */

        _BV(7), /* 30 TubeInterrupt */
        _BV(7)  // ---
};
const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
        NOT_ON_TIMER,   /* 0  - PD0 */
        NOT_ON_TIMER,   /* 1  - PD1 */
        NOT_ON_TIMER,   /* 2  - PD2 INT0 */
        TIMER1B,   	/* 3  - PD4 PWM  */
        NOT_ON_TIMER,   /* 4  - PD3 INT1 */
        TIMER1A,        /* 5  - PD5 PWM  */
        TIMER2B,   	/* 6  - PD6 PWM  */
        NOT_ON_TIMER,   /* 7  - PC6 */
        NOT_ON_TIMER,   /* 8  - PB2 - INT2 */
        TIMER2A,   	/* 9  - PD7 - PWM  */
        TIMER0B,   	/* 10 - PB4 - PWM  */
        TIMER0A,   	/* 11 - PB3 - PWM  */
        NOT_ON_TIMER,   /* 12 - PB0 */
        NOT_ON_TIMER,   /* 13 - PB1 */
        NOT_ON_TIMER,   /* 14 - PB6 SPI*/
        NOT_ON_TIMER,   /* 15 - PB7 */
        NOT_ON_TIMER,   /* 16 - PB5 */
        NOT_ON_TIMER,   /* 17 - PA5 ADC*/
        NOT_ON_TIMER,   /* 18 - PA4 */
        NOT_ON_TIMER,   /* 19 - PA3 */
        NOT_ON_TIMER,   /* 20 - PA2 */
        NOT_ON_TIMER,   /* 21 - PA1 */
        NOT_ON_TIMER,   /* 22 - PA0 */
        NOT_ON_TIMER,   /* 23 - PC1 I2C*/
        NOT_ON_TIMER,   /* 24 - PC0 */
        NOT_ON_TIMER,   /* 25 - PA7 INT*/
        NOT_ON_TIMER,   /* 26 - PC5 VIRT*/
        NOT_ON_TIMER,   /* 27 - PC4 VIRT*/
        NOT_ON_TIMER,   /* 28 - PA6 */
        NOT_ON_TIMER,   /* 29 - PC7 */
        NOT_ON_TIMER,   /* 30 - PC3 */
        NOT_ON_TIMER    /* 31 - PC2 */
};
#endif


#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial

#if defined(UBRR1H)
    #define SERIAL_PORT_HARDWARE1       Serial1
    #define SERIAL_PORT_HARDWARE_OPEN   Serial1
#endif

#endif
