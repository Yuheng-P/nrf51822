#include <stdio.h>
#include <string.h>
#include "hw_uart.h"
#include "hw_timer.h"
#include "nrf_delay.h"

const app_uart_comm_params_t uart_params = {
    UART_RX,
    UART_TX,
    UART_RTS,
    UART_CTS,

    APP_UART_FLOW_CONTROL_DISABLED,
    UART_HWFC,
    UART_BAUDRATE_BAUDRATE_Baud115200
};

typedef enum {
    TX_TRANSING,
    TX_DONE,
}TX_STATE;


static uint8_t uart_recv_buf[UART_RECV_BUF_LEN] = {0};
static uint32_t uart_recv_idx = 0;

static uint8_t uart_event_buf[UART_EVENT_BUF_LEN] = {0};
static uint32_t uart_event_idx = 0;

static uint8_t * resp_str = NULL;
static uint8_t tx_state = TX_TRANSING;

static net_event event_list[5];
static uint8_t event_count = 0;

static callback event_cb = NULL;

void net_event_register(net_event * e)
{    
    if (event_count >= 5)
        return;
    
    memcpy(&event_list[event_count++], e, sizeof(net_event));
}

void uart_evt_hdl(app_uart_evt_t * e)
{
    int list_idx = 0;
    
    switch(e->evt_type)
    {
        case APP_UART_DATA:
            
            if (event_cb)
            {
                if (event_cb(e->data.value)) //when return not 0, mean that event data handler done!
                    event_cb = NULL;
                break;
            }
            
            uart_event_buf[uart_event_idx++] = e->data.value;
            for (list_idx = 0; list_idx < event_count; list_idx++)
            {
                if (uart_event_idx >= strlen(event_list[list_idx].perfix) && 
                    0 == memcmp(uart_event_buf + uart_event_idx - strlen(event_list[list_idx].perfix) , event_list[list_idx].perfix, strlen(event_list[list_idx].perfix)))
                {
                    event_cb = event_list[list_idx].cb;
                    break;
                }
            }
            
            uart_recv_buf[uart_recv_idx++] = e->data.value;
            if (resp_str && uart_recv_idx >= strlen((char*)resp_str) &&
                0 == memcmp(uart_recv_buf + uart_recv_idx - strlen((char *)resp_str) , resp_str, strlen((char *) resp_str)))
            {
                tx_state = TX_DONE;
                memset(uart_recv_buf, 0, UART_RECV_BUF_LEN);
                uart_recv_idx = 0;

                //reset uart_event_buf
                memset(uart_event_buf, 0, UART_EVENT_BUF_LEN);
                uart_event_idx = 0;
                break;
            }
            break;
        case APP_UART_TX_EMPTY:
            break;
        default:
            break;
    }
}

int32_t hw_uart_init()
{
    int ret = 0;
    APP_UART_INIT(&uart_params, uart_evt_hdl, APP_IRQ_PRIORITY_LOW, ret);
    return ret;
}

#define RESP_TIMEOUT 10000
int32_t hw_uart_send(uint8_t * buf, uint32_t len, uint8_t * resp)
{
    uint32_t timeout = RESP_TIMEOUT / 5; // 10s
    int ret = 0;

    resp_str = resp;
    tx_state = TX_TRANSING;
    app_uart_buf_put(buf, len);
    while(resp && tx_state != TX_DONE && timeout){
            nrf_delay_ms(5);
            timeout--;
    };
    resp_str = NULL;
    
    if (0 == timeout)
        ret = -1;
    

    return ret;
}

/* define fputc */
#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    // TODO
    return ch;
}
#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */
__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    // TODO
    return len;
}
#endif

