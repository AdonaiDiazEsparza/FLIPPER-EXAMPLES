#include <furi.h>
#include <furi_hal.h>
#include <furi/core/log.h>
#include <furi_hal_spi_config.h>
#include <furi_hal_spi.h>
#include <furi_hal_bus.h>

#define LOG_TAG "HWSPACE"
#define LOG_SHOW true
#define TIMEOUT 100
#define CS &gpio_ext_pa4
#define SCK &gpio_ext_pb3
#define MOSI &gpio_ext_pa7
#define MISO &gpio_ext_pa6

#define BUS &furi_hal_spi_bus_r
#define SpeedBus &furi_hal_spi_preset_1edge_low_8m // 8MHz the clock of the flipper is 64MHz

#define log_info(format, ...) \
    if(LOG_SHOW) FURI_LOG_I(LOG_TAG, format, ##__VA_ARGS__)

#define log_exception(format, ...) \
    if(LOG_SHOW) FURI_LOG_E(LOG_TAG, format, ##__VA_ARGS__)

#define log_warning(format, ...) \
    if(LOG_SHOW) FURI_LOG_W(LOG_TAG, format, ##__VA_ARGS__)

//---------------------------------------------------------------------------------------------------------
//  This part of the code has been copied from the library maybe this works
//  I'll prove it
//---------------------------------------------------------------------------------------------------------

inline static void furi_hal_spi_bus_r_handle_event_callback(
    FuriHalSpiBusHandle* handle,
    FuriHalSpiBusHandleEvent event,
    const LL_SPI_InitTypeDef* preset) {
    if(event == FuriHalSpiBusHandleEventInit) {
        furi_hal_gpio_write(handle->cs, true);
        furi_hal_gpio_init(handle->cs, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    } else if(event == FuriHalSpiBusHandleEventDeinit) {
        furi_hal_gpio_write(handle->cs, true);
        furi_hal_gpio_init(handle->cs, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    } else if(event == FuriHalSpiBusHandleEventActivate) {
        LL_SPI_Init(handle->bus->spi, (LL_SPI_InitTypeDef*)preset);
        LL_SPI_SetRxFIFOThreshold(handle->bus->spi, LL_SPI_RX_FIFO_TH_QUARTER);
        LL_SPI_Enable(handle->bus->spi);

        furi_hal_gpio_init_ex(
            handle->miso,
            GpioModeAltFunctionPushPull,
            GpioPullNo,
            GpioSpeedVeryHigh,
            GpioAltFn5SPI1);
        furi_hal_gpio_init_ex(
            handle->mosi,
            GpioModeAltFunctionPushPull,
            GpioPullNo,
            GpioSpeedVeryHigh,
            GpioAltFn5SPI1);
        furi_hal_gpio_init_ex(
            handle->sck,
            GpioModeAltFunctionPushPull,
            GpioPullNo,
            GpioSpeedVeryHigh,
            GpioAltFn5SPI1);

        furi_hal_gpio_write(handle->cs, false);
    } else if(event == FuriHalSpiBusHandleEventDeactivate) {
        furi_hal_gpio_write(handle->cs, true);

        furi_hal_gpio_init(handle->miso, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        furi_hal_gpio_init(handle->mosi, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        furi_hal_gpio_init(handle->sck, GpioModeAnalog, GpioPullNo, GpioSpeedLow);

        LL_SPI_Disable(handle->bus->spi);
    }
}

//---------------------------------------------------------------------------------------------------------
//  Here starts my program
//  
//---------------------------------------------------------------------------------------------------------

// The callback for the SPI
static void spi_bus_callback(FuriHalSpiBusHandle* handle, FuriHalSpiBusHandleEvent event) {
    furi_hal_spi_bus_r_handle_event_callback(handle, event, SpeedBus);
}

// This the function that initialize the SPI protocol
FuriHalSpiBusHandle* spi_alloc() {
    FuriHalSpiBusHandle* spi = malloc(sizeof(FuriHalSpiBusHandle));
    spi->bus = BUS;
    spi->callback = &spi_bus_callback;
    spi->cs = CS;
    spi->miso = MISO;
    spi->mosi = MOSI;
    spi->sck = SCK;
    return spi;
}

// This function works to send data by the SPI protocol
static void send_data(FuriHalSpiBusHandle* spi) {
    uint8_t buffer[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    furi_hal_spi_acquire(spi);
    if(furi_hal_spi_bus_tx(spi, buffer, sizeof(buffer), TIMEOUT)) {
        log_info("The values are sent");
    } else {
        log_exception("Values wasnt sent");
    }

    furi_hal_spi_release(spi);
}

// This the main function
int32_t app_main(void* p) {
    UNUSED(p);

    FuriHalSpiBusHandle* spi = spi_alloc();
    furi_hal_spi_bus_handle_init(spi);

    send_data(spi);
    log_info("Valores Enviados");

    furi_hal_spi_bus_handle_deinit(spi);

    return 0;
}