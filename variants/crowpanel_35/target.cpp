#include <Arduino.h>
#include "target.h"

CrowPanel35Board board;

// CrowPanel uses custom SPI pins for LoRa
static SPIClass spi;
RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, spi);

WRAPPER_CLASS radio_driver(radio, board);

ESP32RTCClock fallback_clock;
AutoDiscoverRTCClock rtc_clock(fallback_clock);

EnvironmentSensorManager sensors;

#ifdef DISPLAY_CLASS
  DISPLAY_CLASS display;
  #ifdef PIN_USER_BTN
    MomentaryButton user_btn(PIN_USER_BTN, 1000, true);
  #endif
#endif

bool radio_init() {
  // Wait for USB CDC to be ready (ESP32-S3 native USB)
  delay(1000);
  Serial.println("\n\n=== CrowPanel 3.5 MeshCore ===");
  Serial.println("Initializing...");

  fallback_clock.begin();
  rtc_clock.begin(Wire);

  Serial.println("Initializing LoRa radio...");
  bool result = radio.std_init(&spi);
  if (result) {
    Serial.println("LoRa radio ready");
  } else {
    Serial.println("LoRa radio FAILED!");
  }
  return result;
}

uint32_t radio_get_rng_seed() {
  return radio.random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio.setFrequency(freq);
  radio.setSpreadingFactor(sf);
  radio.setBandwidth(bw);
  radio.setCodingRate(cr);
}

void radio_set_tx_power(uint8_t dbm) {
  radio.setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng);  // create new random identity
}
