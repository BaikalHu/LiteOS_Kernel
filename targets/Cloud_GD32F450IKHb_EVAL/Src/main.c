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
#include "main.h"
#include "sys_init.h"


#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"

static UINT32 g_TaskID1, g_TaskID2, g_TaskID3;


LITE_OS_SEC_TEXT VOID TEST1(VOID)
{
    while (1)
    {
        printf("test1\r\n");
        //usart_data_transmit(EVAL_COM1, 'a');
        //while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
        LOS_TaskDelay(10);
    }
}

LITE_OS_SEC_TEXT VOID TEST2(VOID)
{
    while (1)
    {
        printf("test2\r\n");
        //usart_data_transmit(EVAL_COM1, 'b');
        //while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
        LOS_TaskDelay(20);
    }
}

LITE_OS_SEC_TEXT VOID TEST3(VOID)
{
    while (1)
    {
        printf("test3\r\n");
        //usart_data_transmit(EVAL_COM1, 'c');
        //while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));

        LOS_TaskDelay(30);
    }
}

void LOS_BoadExampleEntry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST1;
    stTaskInitParam.uwStackSize = 0x330;
    stTaskInitParam.pcName = "Test1";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID1, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST2;
    stTaskInitParam.uwStackSize = 0x330;
    stTaskInitParam.pcName = "Test2";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID2, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST3;
    stTaskInitParam.uwStackSize = 0x330;
    stTaskInitParam.pcName = "Test3";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID3, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    return;
}


VOID HardWare_Init(VOID)
{
    gd_eval_com_init(EVAL_COM1);
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    hal_trng_config();
    //dwt_delay_init(SystemCoreClock);
}

int main(void)
{
    UINT32 uwRet = LOS_OK;
    HardWare_Init();
    //printf("hello word\r\n");
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    extern UINT32 create_work_tasks(VOID);
    uwRet = create_work_tasks();
    //LOS_BoadExampleEntry();
    //#include "los_demo_entry.h"
    //LOS_Demo_Entry();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }


    (void)LOS_Start();
    return 0;
}
