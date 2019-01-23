#include <stdio.h>

#include "esp8266.h"
#include "sys_init.h"
#include "atiny_socket.h"
#if defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)
APP_MAILBOX_DEF(esp8266_mb, ESP8266_MAILBOX_Q_SIZE, ESP8266_MAILBOX_ITEM_SIZE);

CMD esp8266_cmd[] = {
        {"AT\r\n", "OK"},
        {"AT+RST\r\n", "ready"},
        {"ATE0\r\n", "OK"},
        {"AT+CWMODE_CUR", "OK"},
        {"AT+CWJAP_CUR", "OK"},
        {"AT+CIPMUX", "OK"},
        {"AT+CIPSTART", "OK"},
        {"AT+CIPSEND", "SEND OK"},
        {"AT+CIPCLOSE", "OK"},
        {NULL, NULL},
};

int esp8266_send_cmd(E_CMD e)
{
    if (e >= CMD_MAX)
        return -1;
        
    return hw_uart_send((uint8_t *)esp8266_cmd[e].cmd, strlen((char*)esp8266_cmd[e].cmd), (uint8_t *)esp8266_cmd[e].resp);
}

void esp8266_choose_net_mode(enum_net_mode m)
{
    if (m > STA_AP)
        return ;
    
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d\r\n", esp8266_cmd[CMD_CWMODE].cmd, m);
    hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CWMODE].resp);
    return ;
}

int esp8266_joinap(char * ssid, char * passwd)
{
    if (NULL == ssid || NULL == passwd)
        return -1;
        
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=\"%s\",\"%s\"\r\n", esp8266_cmd[CMD_JOINAP].cmd, ssid, passwd);
    return hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_JOINAP].resp);
}

void esp8266_set_mux_mode(int mux_mode)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d\r\n", esp8266_cmd[CMD_CIPMUX].cmd, mux_mode);
    hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CIPMUX].resp);
}

int32_t esp8266_event_handler(uint8_t byte)
{
    static int state = 0;  //0: get id, 1: get len, 2:get data
    static int len = 0;
    static char buf[1024] = {0};
    static int buf_count = 0;
    
    LED_TOGGLE(4);
    
    switch (state)
    {
        case 0:  //byte = ','
           len = 0;
            memset(buf, 0, 1024);
            buf_count = 0;
            state ++;
            break;

        case 1: //byte = len:
            if (byte >= '0' && byte <= '9')
                len = len * 10 + byte - '0';
            else
                state ++;
            break;
        case 2: //byte = data
            if (buf_count < len)
                buf[buf_count ++] = byte;
           
            if (buf_count == len)
            {
                state ++;
                mailbox_buf b;
                
                if (len > 0) 
                {
                    b.len = len;
                    b.addr = atiny_malloc(len);
                    memcpy(b.addr, buf, len);
                    
                    hw_mb_put(&b);
                }
            }
            break; 
        default:
            break;
    }
    if (state > 2)
    {
        state = 0;
        return 1;
    }
    
    return 0;
}

int32_t esp8266_init(void)
{
    nrf_delay_ms(2000);
    net_event e = {
        .perfix = "+IPD",
        .suffix = NULL,
        .cb = esp8266_event_handler,
    };
    
    hw_mailbox_init(&esp8266_mb);
    net_event_register(&e);
    
    esp8266_send_cmd(CMD_AT);
    esp8266_send_cmd(CMD_RST);
    esp8266_send_cmd(CMD_ECHO_OFF);
    esp8266_choose_net_mode(STA);
    LED_ON(0);
    esp8266_joinap(WIFI_SSID, WIFI_PASSWD);
    esp8266_set_mux_mode(0);   //signle
    LED_ON(1);
    
    return 0;
}

int32_t esp8266_connect(const int8_t* host, const int8_t* port, int32_t proto)
{
    char cmd[64] = {0};

    snprintf(cmd, 64, "%s=\"%s\",\"%s\",%s\r\n", esp8266_cmd[CMD_CONN].cmd, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    return hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CONN].resp);
}

int32_t esp8266_recv_timeout(int32_t id, int8_t * buf, uint32_t len,
                           int32_t timeout)
{
    mailbox_buf b = {0, NULL};
    timeout /= 5;
    
    while(timeout == 0xffffffff || timeout > 0)
    {
        if (hw_mb_get(&b) == NRF_SUCCESS && b.len > 0)
        {
            memcpy(buf, b.addr, b.len);
            atiny_free(b.addr);
            break;
        }
        nrf_delay_ms(5);
        if (timeout != 0xffffffff)
            timeout --;
    }
    return b.len;
}

int32_t esp8266_recv(int32_t id, int8_t * buf, uint32_t len)
{
    esp8266_recv_timeout(id, buf, len, 0xffffffff);
    return len; 
}

int32_t esp8266_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
    char cmd[64] = {0};

    snprintf(cmd, 64, "%s=%lu\r\n", esp8266_cmd[CMD_SEND].cmd, len);
         
    hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)">");
         
    hw_uart_send((uint8_t *)buf, len, (uint8_t *)esp8266_cmd[CMD_SEND].resp);
    return 0;
}

int32_t esp8266_close(int32_t id)
{
        char cmd[64] = {0};
        snprintf(cmd, 64, "%s\r\n", esp8266_cmd[CMD_CLOSE].cmd);
        return hw_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CLOSE].resp);
}

at_adaptor_api at_interface = {
    .init = esp8266_init,
    .connect = esp8266_connect,
    .send = esp8266_send,
    .recv_timeout = esp8266_recv_timeout,

    .recv = esp8266_recv,    
    .close = esp8266_close,    
};
#endif
