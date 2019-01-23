#ifndef __HW_UART_H__
#define __HW_UART_H__

#include "app_uart.h"
#include "nrf_gpio.h"
#if 0
#define UART_DEBUG
#else
#define UART_MODULE
#endif

#ifdef UART_DEBUG
#define UART_TX         9
#define UART_RX         11
#define UART_CTS        10
#define UART_RTS        8
#define UART_HWFC       false
#endif

#ifdef UART_MODULE
#define UART_TX         9
#define UART_RX         11
#define UART_CTS        10
#define UART_RTS        8
#define UART_HWFC       false
#endif


#define LED_ON(idx) do{ nrf_gpio_pin_set(18 + idx);}while(0)
#define LED_OFF(idx)  do{ nrf_gpio_pin_clear(18 + idx);}while(0)
#define LED_TOGGLE(idx)  do { nrf_gpio_pin_toggle(18 + idx);} while(0)

#define UART_RECV_BUF_LEN    512
#define UART_EVENT_BUF_LEN    512

typedef int32_t (*callback)(uint8_t byte);

typedef struct _net_event {
    char * perfix;
    char * suffix;
    callback cb;
}net_event;
    

int32_t hw_uart_init(void);
int32_t hw_uart_send(uint8_t * buf, uint32_t len, uint8_t * resp);
void net_event_register(net_event * e);
#endif /* __HW_UART_H__ */
