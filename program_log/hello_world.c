#include "app.h"

/*
*   This program works to show in the LOG "Helloooo" on the terminal every second
*/

int32_t app_main(void* p) {
    UNUSED(p);
    while(furi_hal_gpio_read(&gpio_button_back)) {
        log_info("HELLOOO!!");
        furi_delay_ms(1000);
    }
    return 0;
}