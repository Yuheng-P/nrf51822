#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <stdio.h>
#include "at_api_interface.h"

#if 0
#define WIFI_SSID   "iPhone"
#define WIFI_PASSWD  "songjiannan"
#else
#define WIFI_SSID              "TP-LINK_80D8BE"
#define WIFI_PASSWD            "87654321"
#endif

#define ESP8266_MAILBOX_Q_SIZE    3
#define ESP8266_MAILBOX_ITEM_SIZE sizeof(mailbox_buf)
    
typedef struct _cmd {
    char * cmd;
    char * resp;
}CMD;

typedef enum _net_mode {
    STA = 1,
    AP, 
    STA_AP,
}enum_net_mode;


typedef struct _mailbox_buf{
    uint32_t len;
    uint8_t * addr;
}mailbox_buf;

typedef enum {
    CMD_AT = 0,
    CMD_RST,
    CMD_ECHO_OFF,
    CMD_CWMODE,
    CMD_JOINAP,
    CMD_CIPMUX,
    CMD_CONN,
    CMD_SEND,
    CMD_CLOSE,
    CMD_MAX
}E_CMD;


#endif /* __ESP8266_H__ */

