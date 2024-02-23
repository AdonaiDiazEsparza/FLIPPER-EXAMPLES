#include <furi_hal.h>

int app_main(void* p) {
    UNUSED(p);

    // Eneable pin A7 for output (Led on the board, push-pull)
    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeOutputPushPull);

    while(furi_hal_gpio_read(&gpio_button_back)) {
        if(furi_hal_gpio_read(&gpio_button_ok)) furi_hal_gpio_write(&gpio_ext_pa7, false);
        else furi_hal_gpio_write(&gpio_ext_pa7, true);
    }

    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeAnalog);

    return 0;
}