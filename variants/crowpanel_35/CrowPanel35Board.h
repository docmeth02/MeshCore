#pragma once

#include <Arduino.h>
#include <helpers/ESP32Board.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>

// CrowPanel 3.5" Advance specific pins
#define PIN_LORA_ENABLE     45   // Must be LOW to enable LoRa (disables microphone)
#define PIN_DISPLAY_BL      38   // Display backlight
#define PIN_TOUCH_RST       48   // Touch controller reset

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

  void powerOff() override {
    // Turn off display backlight
    pinMode(PIN_DISPLAY_BL, OUTPUT);
    digitalWrite(PIN_DISPLAY_BL, LOW);
    gpio_hold_en((gpio_num_t)PIN_DISPLAY_BL);

    // Hold touch controller in reset to prevent spurious interrupts
    pinMode(PIN_TOUCH_RST, OUTPUT);
    digitalWrite(PIN_TOUCH_RST, LOW);
    gpio_hold_en((gpio_num_t)PIN_TOUCH_RST);

    // Disable ALL wake sources - only power cycle will wake device
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    // Enter deep sleep - this never returns
    esp_deep_sleep_start();
  }

  const char* getManufacturerName() const override {
    return "CrowPanel 3.5";
  }
};
