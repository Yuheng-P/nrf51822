#include <stdio.h>
#include "hw_mailbox.h"

const app_mailbox_t * gp_mb = NULL;
void hw_mailbox_init(const app_mailbox_t * mb)
{
    gp_mb = mb;
    app_mailbox_create(mb);
}

uint32_t hw_mb_put(void * item)
{
    return app_mailbox_put(gp_mb, item);
}

uint32_t hw_mb_get(void * item)
{
    return app_mailbox_get(gp_mb, item);
}
