#include <stdio.h>
#include <string.h>
#if 0
#include "nrf51.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "app_mailbox.h"
#include "mem_manager.h"
#endif
#include "sys_init.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "internals.h"
#include "atiny_socket.h"
#if defined(WITH_AT_FRAMEWORK)
#include "at_api_interface.h"
#endif

int main()
{
	
	  nrf_gpio_range_cfg_output(18, 19);
    nrf_gpio_range_cfg_output(21, 23);
	//LED_ON(4);
	

    hw_init();
	//hw_uart_send("hello",5,0);
	//while(1){LED_ON(4);LED_ON(3);}

#if defined(WITH_AT_FRAMEWORK) && (defined(USE_SIM900A) || defined(USE_ESP8266))
    at_api_register(&at_interface);
#else

#endif

    LED_ON(0);
    agent_tiny_entry();
}
