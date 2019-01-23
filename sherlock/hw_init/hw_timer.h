#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__

#include "nrf51.h"
#include "app_timer.h"

#define APP_TIMER_PERSCALER          0
#define APP_TIMER_QSIZE              3


void hw_timer_init(void);
void hw_timer_start(void);
#endif /* __HW_TIMER_H__ */
