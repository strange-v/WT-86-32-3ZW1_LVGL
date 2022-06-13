#include <display_driver.h>

/* Update TFT */
void IRAM_ATTR flush_pixels(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t len = w * h;

    lcd.startWrite();                            /* Start new TFT transaction */
    lcd.setAddrWindow(area->x1, area->y1, w, h); /* set the working window */
    //lcd.writePixels((uint16_t *)color_p, len);   /* Write words at once */
    lcd.writePixels((uint16_t *)&color_p->full, len, true);
    lcd.endWrite();                              /* terminate TFT transaction */

    lv_disp_flush_ready(disp);
}

/* Read the touchpad */
void IRAM_ATTR touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch(&touchX, &touchY);

    if (touched)
    {
        data->point.x = touchX;
        data->point.y = touchY;
        data->state = LV_INDEV_STATE_PR;

        Serial.printf("Touch x: %d, y: %d\n", touchX, touchY);
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

void calibrate_touch()
{
    if (lcd.touch())
    {
        // Draw a guide text on the screen.
        lcd.setTextDatum(textdatum_t::middle_center);
        lcd.drawString("touch the arrow marker.", lcd.width() >> 1, lcd.height() >> 1);
        lcd.setTextDatum(textdatum_t::top_left);

        // If you use touch, calibrate. Touch the tips of the arrows displayed in the four corners of the screen in order.
        uint16_t data[8];
        lcd.calibrateTouch(data, TFT_WHITE, TFT_BLACK, std::max(lcd.width(), lcd.height()) >> 3);

        Serial.print("Calibration data: {");
        for (int i = 0; i < 8; ++i)
        {
            Serial.printf("%d, ", data[i]);
        }
        Serial.print("}\n");
        lcd.fillScreen(TFT_BLACK);
    }
}