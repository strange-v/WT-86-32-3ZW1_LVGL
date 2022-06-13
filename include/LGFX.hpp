#ifndef LGFX_H
#define LGFX_H

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  //lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_GSLx680_320x320 _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();

      cfg.i2s_port = I2S_NUM_0;
      cfg.freq_write = SPI_FREQUENCY;
      cfg.pin_wr = TFT_WR;
      cfg.pin_rd = TFT_RD;
      cfg.pin_rs = TFT_DC;

      cfg.pin_d0 = TFT_D0;
      cfg.pin_d1 = TFT_D1;
      cfg.pin_d2 = TFT_D2;
      cfg.pin_d3 = TFT_D3;
      cfg.pin_d4 = TFT_D4;
      cfg.pin_d5 = TFT_D5;
      cfg.pin_d6 = TFT_D6;
      cfg.pin_d7 = TFT_D7;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {                                      // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。

      cfg.pin_cs = TFT_CS;   // CS要拉低
      cfg.pin_rst = TFT_RST; // RST和开发板RST相连
      cfg.pin_busy = -1;     // BUSYが接続されているピン番号 (-1 = disable)

      // The following setting values are set to general initial values for each panel, so please comment out any unknown items and try them.

      cfg.panel_width = TFT_WIDTH;    // Actually displayable width
      cfg.panel_height = TFT_WIDTH;   // Height that can actually be displayed
      cfg.memory_width = TFT_WIDTH;   // Maximum width supported by driver IC
      cfg.memory_height = TFT_WIDTH;  // Maximum height supported by driver IC
      cfg.offset_x = 0;               // Amount of offset in the X direction of the panel
      cfg.offset_y = 0;               // Amount of offset in the Y direction of the panel
      cfg.offset_rotation = TFT_ROTATION; // Offset of the rotation 0 ~ 7 (4 ~ 7 is upside down)
      cfg.dummy_read_pixel = 8;       // Number of dummy read bits before pixel read
      cfg.dummy_read_bits = 1;        // bits of dummy read before reading data other than pixels
      cfg.readable = false;           // true if data can be read
      cfg.invert = INVERT_COLORS;     // true if the light and darkness of the panel is reversed
      cfg.rgb_order = false;          // true if the red and blue of the panel are swapped
      cfg.dlen_16bit = false;         // true for panels that send data length in 16-bit units
      cfg.bus_shared = false;          // true if the bus is shared with the SD card

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = TFT_BCKL;
      cfg.invert = false;
      cfg.freq = BACKLIGHT_FREQUENCY;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    { // タッチスクリーン制御の設定を行います。（必要なければ削除）
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;
      cfg.x_max = 319;
      cfg.y_min = 0;
      cfg.y_max = 319;
      cfg.pin_int = TOUCH_IRQ;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;

      cfg.i2c_port = I2C_TOUCH_PORT;    // 使用するI2Cを選択 (0 or 1)
      cfg.i2c_addr = I2C_TOUCH_ADDRESS; // I2Cデバイスアドレス番号
      cfg.pin_sda = TOUCH_SDA;          // SDAが接続されているピン番号
      cfg.pin_scl = TOUCH_SCL;          // SCLが接続されているピン番号
      cfg.freq = I2C_TOUCH_FREQUENCY;   // I2Cクロックを設定

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // タッチスクリーンをパネルにセットします。
    }

    setPanel(&_panel_instance);
  }
};

#endif //LGFX_H