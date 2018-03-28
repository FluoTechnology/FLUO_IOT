#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define  HW_VERSION   "V01"

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16

#define analogInputToDigitalPin(p) 	(((p)<20)?(esp32_adc2gpio[(p)]):-1)
#define digitalPinToInterrupt(p)    		(((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         		(p < 34)

// PRIVATE
static const uint8_t TX = 1;
static const uint8_t RX = 3;

static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 18;

static const uint8_t LED_STATUS = 25; 
static const uint8_t LED_LINK   = 2; 
static const uint8_t LED_CLOUD  = 4;  
static const uint8_t LED_BLE    = 21;  

//PUBLIC
static const uint8_t BUTTON_APRST = 35;  
static const uint8_t BUTTON_CHECK = 27;
static const uint8_t SPI_EN = 26; 
static const uint8_t INTERRUPT_644P = 15; 
static const uint8_t RESET_644P = 22; 

static const uint8_t SDCD = 34;  // card detect
static const uint8_t SD_CS = 5;   // chip select

static const uint8_t LED_BUILTIN = 25;

#endif /* Pins_Arduino_h */
