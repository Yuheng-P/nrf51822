/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "atiny_adapter.h"
#include "mem_manager.h"
#include "hw_uart.h"
#include <stdarg.h>

#define ATINY_CNT_MAX_WAITTIME 0xFFFFFFFF
#define LOG_BUF_SIZE (256)

extern  uint64_t g_kerneltickscount;
static uint64_t osKernelGetTickCount (void)
{
    uint64_t ticks = 0;

    {
        ticks = g_kerneltickscount;
    }
    return ticks;
}

uint64_t atiny_gettime_ms(void)
{
    return osKernelGetTickCount();
}

void* atiny_malloc(size_t size)
{
    return nrf_malloc(size);
}

void atiny_free(void* ptr)
{
    (void)nrf_free(ptr);
}

int atiny_snprintf(char* buf, unsigned int size, const char* format, ...)
{
    int     ret;
    va_list args;

    va_start(args, format);
    ret = vsnprintf(buf, size, format, args);
    va_end(args);

    return ret;
}

int atiny_printf(const char* format, ...)
{
    int ret;
#if 0

    char str_buf[LOG_BUF_SIZE] = {0};
    va_list list;

    memset(str_buf, 0, LOG_BUF_SIZE);
    va_start(list, format);
    ret = vsnprintf(str_buf, LOG_BUF_SIZE, format, list);
    va_end(list);
    
    printf("%s", str_buf);
#endif
    return ret;
}

char *atiny_strdup(const char *ch)
{
    char *copy;
    size_t length;

    if(NULL == ch)
        return NULL;

    length = strlen(ch);
    copy= (char *)atiny_malloc(length + 1);
    if(NULL == copy)
        return NULL;
    strncpy(copy, ch, length);
    copy[length] = '\0';

    return copy;
}

#if 0

void* atiny_mutex_create(void)
{
    uint32_t uwRet;
    uint32_t uwSemId;

    if (OS_INT_ACTIVE)
    {
        return NULL;
    }

    uwRet = LOS_BinarySemCreate(1, (UINT32*)&uwSemId);

    if (uwRet == LOS_OK)
    {
        return (void*)(GET_SEM(uwSemId));
    }
    else
    {
        return NULL;
    }
}

void atiny_mutex_destroy(void* mutex)
{
    if (OS_INT_ACTIVE)
    {
        return;
    }

    if (mutex == NULL)
    {
        return;
    }

    (void)LOS_SemDelete(((SEM_CB_S*)mutex)->usSemID);
}

void atiny_mutex_lock(void* mutex)
{
    if (mutex == NULL)
    {
        return;
    }

    if (OS_INT_ACTIVE)
    {
        return;
    }

    (void)LOS_SemPend(((SEM_CB_S*)mutex)->usSemID, ATINY_CNT_MAX_WAITTIME);
}

void atiny_mutex_unlock(void* mutex)
{
    if (mutex == NULL)
    {
        return;
    }

    (void)LOS_SemPost(((SEM_CB_S*)mutex)->usSemID);
}

#else

void* atiny_mutex_create(void) { return NULL; }

void atiny_mutex_destroy(void* mutex) { ((void)mutex); }

void atiny_mutex_lock(void* mutex) { ((void)mutex); }

void atiny_mutex_unlock(void* mutex) { ((void)mutex); }

#endif /* LOSCFG_BASE_IPC_SEM == YES */

