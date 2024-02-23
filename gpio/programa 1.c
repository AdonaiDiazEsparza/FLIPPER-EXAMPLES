/*
*   This was my first program in FlipperZero 
*   Hello World in electronics, turning on/off led
*
*/

#include <furi_hal.h>

int app_main(void* p) {
    UNUSED(p);

    // Eneable pin A7 for output (Led on the board, push-pull)
    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeOutputPushPull);

    do {
        furi_hal_gpio_write(&gpio_ext_pa7, true); // Led On
        furi_delay_ms(1000); // 1 sec
        furi_hal_gpio_write(&gpio_ext_pa7, false); // Led Off
        furi_delay_ms(1000); // 1 sec
    } while(furi_hal_gpio_read(&gpio_button_back));

    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeAnalog); // Led off (Floating)

    return 0;
}