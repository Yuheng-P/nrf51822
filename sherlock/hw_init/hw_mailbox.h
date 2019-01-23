#ifndef __HW_MAILBOX_H__
#define __HW_MAILBOX_H__

#include "app_mailbox.h"

void hw_mailbox_init(const app_mailbox_t * mb);
uint32_t hw_mb_put(void * item);
uint32_t hw_mb_get(void * item);

#endif /* __HW_MAILBOX_H__ */
