#include <application.h>

twr_button_t button;
twr_led_t lcdLedR;
twr_led_t lcdLedG;
twr_led_t lcdLedB;
twr_led_t *leds[] = { &lcdLedR, &lcdLedG, &lcdLedB };
char *colors[] = {"RED", "GREEN", "BLUE"};
int mode = 0;
twr_gfx_t *pgfx;

void button_event_handler(twr_button_t *self, twr_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;
   

    if (event == TWR_BUTTON_EVENT_PRESS) {
        twr_gfx_clear(pgfx);
        twr_led_pulse(leds[mode], 2500);
        char hello[16] = "";
        snprintf(hello, sizeof(hello), "%s", colors[mode]);
        int left = (128 - twr_gfx_calc_string_width(pgfx, hello)) / 2;
        int top =  (128 - 32) / 2;
        twr_log_debug("%s (%i, %i)", hello, left, top);
        twr_gfx_draw_string(pgfx, left, top, hello, true);

        twr_gfx_update(pgfx);
        mode = (mode + 1) % 3;
    }
}

void application_init(void)
{
    twr_log_init(TWR_LOG_LEVEL_DEBUG, TWR_LOG_TIMESTAMP_ABS);

    twr_button_init(&button, TWR_GPIO_BUTTON, TWR_GPIO_PULL_DOWN, false);
    twr_button_set_event_handler(&button, button_event_handler, NULL);

    const twr_led_driver_t* driver = twr_module_lcd_get_led_driver();
    twr_led_init_virtual(&lcdLedR, TWR_MODULE_LCD_LED_RED, driver, 1);
    twr_led_init_virtual(&lcdLedG, TWR_MODULE_LCD_LED_GREEN, driver, 1);
    twr_led_init_virtual(&lcdLedB, TWR_MODULE_LCD_LED_BLUE, driver, 1);

    twr_module_lcd_init();
    pgfx = twr_module_lcd_get_gfx();
    twr_gfx_set_font(pgfx, &twr_font_ubuntu_33);
}