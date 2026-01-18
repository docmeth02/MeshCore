#pragma once

#include <Arduino.h>
#include <helpers/ESP32Board.h>

// CrowPanel 3.5" Advance specific pins
#define PIN_LORA_ENABLE     45   // Must be LOW to enable LoRa (disables microphone)
#define PIN_DISPLAY_BL      38   // Display backlight

class CrowPanel35Board : public ESP32Board {
public:
  void begin() {
    // Enable LoRa module (GPIO 45 LOW disables mic, enables LoRa)
    pinMode(PIN_LORA_ENABLE, OUTPUT);
    digitalWrite(PIN_LORA_ENABLE, LOW);

    // Backlight controlled by LovyanGFX when display is enabled
    // For non-display builds, turn off to save power
#ifndef DISPLAY_CLASS
    pinMode(PIN_DISPLAY_BL, OUTPUT);
    digitalWrite(PIN_DISPLAY_BL, LOW);
#endif

    ESP32Board::begin();
  }

  const char* getManufacturerName() const override {
    return "CrowPanel 3.5";
  }
};
