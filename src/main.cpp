#include <lvgl.h>
#include <LGFX.hpp>
#include <display_driver.h>

LGFX lcd;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf = (lv_color_t *)heap_caps_malloc(TFT_WIDTH * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);

lv_obj_t *roller1;
static lv_obj_t *meter;
lv_anim_t a;
lv_anim_t b;
lv_meter_indicator_t *indic3;
lv_meter_indicator_t *indic;

static lv_style_t style_btn;
lv_theme_t *th;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

#pragma region DEMO

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected month: %s\n", buf);
        Serial.println(buf);
    }
}

/**
 * An infinite roller
 */
void lv_example_roller_1(void)
{
    roller1 = lv_roller_create(lv_scr_act());

    lv_obj_set_style_text_font(roller1, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_bg_color(roller1, lv_color_hex(0xFF8A65), LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

    lv_roller_set_options(roller1,
        LV_SYMBOL_GPS "  GPS\n" LV_SYMBOL_WIFI "  WIFI\n" LV_SYMBOL_BLUETOOTH "  BLUE\n" LV_SYMBOL_POWER "  POWER\n" LV_SYMBOL_BELL "  INFO\n" LV_SYMBOL_CHARGE "  CHARGE",
        LV_ROLLER_MODE_NORMAL);

    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_center(roller1);
    lv_obj_add_event_cb(roller1, event_handler, LV_EVENT_ALL, NULL);
}

void lv_example_style_13(void)
{
    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_color(&style_indic, lv_color_hex(0xD1F2EB));
    lv_style_set_bg_grad_color(&style_indic, lv_color_hex(0xEC7063));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);

    static lv_style_t style_indic_pr;
    lv_style_init(&style_indic_pr);
    lv_style_set_shadow_color(&style_indic_pr, lv_color_hex(0xFEF9E7));
    lv_style_set_shadow_width(&style_indic_pr, 8);
    lv_style_set_shadow_spread(&style_indic_pr, 3);

    /*Create an object with the new style_pr*/
    lv_obj_t *obj = lv_slider_create(lv_scr_act());
    lv_obj_add_style(obj, &style_indic, LV_PART_INDICATOR);
    lv_obj_add_style(obj, &style_indic_pr, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_slider_set_value(obj, 70, LV_ANIM_OFF);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 130);
}

static void set_arc_angle(lv_obj_t *obj, int16_t v)
{
    lv_arc_set_value(obj, v);
}

/**
 * Create an arc which acts as a loader.
 */
void lv_example_arc_2(void)
{

    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    /*Create an Arc*/
    lv_obj_t *arc = lv_arc_create(lv_scr_act());
    lv_obj_set_size(arc, 220, 220);
    lv_obj_set_style_bg_color(arc, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_PART_KNOB);

    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE); /*To not allow adjusting by click*/
    lv_obj_center(arc);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)set_arc_angle);
    lv_anim_set_time(&a, 30000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); /*Just for the demo*/
    lv_anim_set_repeat_delay(&a, 1);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_start(&a);
}

void lv_example_arc_1(void)
{
    /*Create an Arc*/
    lv_obj_t *arc = lv_arc_create(lv_scr_act());
    lv_obj_set_style_bg_color(arc, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_PART_KNOB);
    lv_obj_set_size(arc, 220, 220);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 40);
    lv_obj_center(arc);
}

#pragma endregion // DEMO

void setup()
{
    Serial.begin(115200);
    Serial.printf("Version: %d.%d.%d\n", lv_version_major(), lv_version_minor(), lv_version_patch());

    lv_init();
    lv_theme_default_init(NULL, lv_color_hex(0xFFEB3B), lv_color_hex(0xFF7043), 1, LV_FONT_DEFAULT);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    lcd.init();

    uint16_t data[8]{1020, 44, 1020, 645, 5, 44, 3, 647};
    lcd.setTouchCalibrate(data);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = flush_pixels;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);

    lv_example_arc_2();
    lv_example_style_13();
    lv_example_roller_1();
}

unsigned long timer1;
void loop()
{
    lv_timer_handler();

    if (millis() - timer1 > 2000)
    {
        timer1 = millis();
        // lv_roller_set_selected(roller1, random(0, 6), LV_ANIM_ON);
    }
}
