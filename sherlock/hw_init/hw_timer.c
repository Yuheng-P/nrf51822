#include <stdio.h>
#include <string.h>
#include "hw_timer.h"
#include "nrf_drv_clock.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"

APP_TIMER_DEF( my_timer);
APP_TIMER_DEF( my_timer2);

uint64_t g_kerneltickscount = 0;

void app_data_report(void);

// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
static void lfclk_request(void)
{
    nrf_drv_clock_config_t c = NRF_DRV_CLOCK_DEAFULT_CONFIG;
    uint32_t err_code = nrf_drv_clock_init(&c);
    
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request();
}

static void my_timer_handler(void * p_context)
{   
    g_kerneltickscount ++;
}
static void my_timer2_handler(void * p_context)
{
    app_data_report();
}

void hw_timer_init()
{
    lfclk_request();
    
    APP_TIMER_INIT(APP_TIMER_PERSCALER, APP_TIMER_QSIZE, false);
    
    hw_timer_start();
}

void hw_timer_start()
{
    app_timer_create(&my_timer, APP_TIMER_MODE_REPEATED, my_timer_handler);
    app_timer_start(my_timer, APP_TIMER_TICKS(1, APP_TIMER_PERSCALER), NULL);

    app_timer_create(&my_timer2, APP_TIMER_MODE_REPEATED, my_timer2_handler);
    app_timer_start(my_timer2, APP_TIMER_TICKS(2000, APP_TIMER_PERSCALER), NULL);
}
