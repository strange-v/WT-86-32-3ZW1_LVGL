#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <lvgl.h>
#include <LGFX.hpp>

extern LGFX lcd;

void IRAM_ATTR flush_pixels(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);
void IRAM_ATTR touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void calibrate_touch();

#endif //DISPLAY_DRIVER_H