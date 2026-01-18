#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <helpers/ui/LGFXDisplay.h>

class LGFX_CrowPanel35 : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_GT911 _touch_instance;

public:
  LGFX_CrowPanel35(void) {
    // SPI bus configuration
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 60000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = 42;
      cfg.pin_mosi = 39;
      cfg.pin_miso = -1;
      cfg.pin_dc = 41;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // Panel configuration
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 40;
      cfg.pin_rst = -1;
      cfg.pin_busy = -1;
      cfg.memory_width = 320;
      cfg.memory_height = 480;
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 9;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }

    // Backlight configuration
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = 38;
      cfg.invert = false;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    // Touch controller configuration (GT911)
    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = 319;
      cfg.y_min = 0;
      cfg.y_max = 479;
      cfg.pin_int = 47;
      cfg.pin_rst = 48;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      cfg.i2c_port = 0;
      cfg.i2c_addr = 0x5D;
      cfg.pin_sda = 15;
      cfg.pin_scl = 16;
      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};

class CrowPanel35Display : public LGFXDisplay {
  LGFX_CrowPanel35 disp;
public:
  CrowPanel35Display() : LGFXDisplay(320, 480, disp) {}

  bool begin() override {
    turnOn();
    display->init();
    display->setRotation(0);  // Portrait mode
    display->setBrightness(128);
    display->setColorDepth(8);
    display->setTextColor(TFT_WHITE);

    buffer.setColorDepth(8);
    buffer.setPsram(true);
    buffer.createSprite(width(), height());

    return true;
  }

  void endFrame() override {
    display->startWrite();
#if UI_ZOOM != 1
    // Scale from origin (0,0) to fill display
    float matrix[6] = {(float)UI_ZOOM, 0, 0, 0, (float)UI_ZOOM, 0};
    buffer.pushAffine(display, matrix);
#else
    buffer.pushSprite(display, 0, 0);
#endif
    display->endWrite();
  }
};
