#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

static const uint8_t TX = 43;
static const uint8_t RX = 44;

static const uint8_t SDA = 15;
static const uint8_t SCL = 16;

// Default SPI (directly on board, used for display - unused here)
static const uint8_t SS = -1;
static const uint8_t MOSI = 39;
static const uint8_t MISO = -1;
static const uint8_t SCK = 42;

// LoRa SPI pins (directly on extension slot)
static const uint8_t LORA_MOSI = 3;
static const uint8_t LORA_MISO = 9;
static const uint8_t LORA_SCK = 10;

#endif /* Pins_Arduino_h */
