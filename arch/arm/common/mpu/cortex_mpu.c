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

#include "cortex_mpu.h"
#include "los_task.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void osMpuEnable()
{
    /* Enable MPU
     * Use the default memory map as 
     * a background region.
     */
    MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;

    /* Make sure that all the registers are set before proceeding */
    __DSB();
    __ISB();
}

void osMpuDisable()
{
    /* Disable MPU */
    MPU->CTRL = 0;
}

UINT32 osMPUCheckParam(MPU_REGION_INFO_S *pstMPURegInfo)
{
    if(pstMPURegInfo == NULL)
        return LOS_ERRNO_MPU_NULL_PARAM;

    if(!IS_MPU_REGION_ENABLE(pstMPURegInfo->bEnable))
        return LOS_ERRNO_MPU_INVALID_ENABLE;

    if(!IS_MPU_REGION_NUMBER(pstMPURegInfo->ucNumber))
        return LOS_ERRNO_MPU_INVALID_NUMBER;

    if(!IS_MPU_INSTRUCTION_ACCESS(pstMPURegInfo->ucXN))
        return LOS_ERRNO_MPU_INVALID_XN;

    if(!IS_MPU_ACCESS_SHAREABLE(pstMPURegInfo->bShareable))
        return LOS_ERRNO_MPU_INVALID_SHAREABLE;

    if(!IS_MPU_ACCESS_CACHEABLE(pstMPURegInfo->bCacheable))
        return LOS_ERRNO_MPU_INVALID_CACHEABLE;

    if(!IS_MPU_ACCESS_BUFFERABLE(pstMPURegInfo->bBufferable))
        return LOS_ERRNO_MPU_INVALID_BUFFERABLE;

    if(!IS_MPU_TEX_LEVEL(pstMPURegInfo->ucTEX))
        return LOS_ERRNO_MPU_INVALID_TEX;

    if(!IS_MPU_REGION_PERMISSION_ATTRIBUTE(pstMPURegInfo->ucAP))
        return LOS_ERRNO_MPU_INVALID_AP;

    if(!IS_MPU_REGION_SIZE((pstMPURegInfo->ucSize >> 5) + 3))
        return LOS_ERRNO_MPU_INVALID_SIZE;

    if(!IS_MPU_REGION_ADDR_ALIGN(pstMPURegInfo->uwBaseAddr, pstMPURegInfo->ucSize))
        return LOS_ERRNO_MPU_INVALID_SIZE;

    return LOS_OK;
}


void LOS_ConfigMpuRegion(MPU_REGION_INFO_S *pstMPURegInfo)
{
    if(osMPUCheckParam(pstMPURegInfo) != LOS_OK)
        return;

    MPU->RNR = pstMPURegInfo->ucNumber;
    if(pstMPURegInfo->bEnable)
    {
        MPU->RBAR = pstMPURegInfo->uwBaseAddr;
        MPU->RASR = ((UINT32)pstMPURegInfo->ucXN                << MPU_RASR_XN_Pos)   |
                    ((UINT32)pstMPURegInfo->ucAP                << MPU_RASR_AP_Pos)   |
                    ((UINT32)pstMPURegInfo->ucTEX               << MPU_RASR_TEX_Pos)  |
                    ((UINT32)pstMPURegInfo->bShareable          << MPU_RASR_S_Pos)    |
                    ((UINT32)pstMPURegInfo->bCacheable          << MPU_RASR_C_Pos)    |
                    ((UINT32)pstMPURegInfo->bBufferable         << MPU_RASR_B_Pos)    |
                    ((UINT32)pstMPURegInfo->bSubRegDisable      << MPU_RASR_SRD_Pos)  |
                    ((((UINT32)pstMPURegInfo->ucSize >> 5) + 3) << MPU_RASR_SIZE_Pos) |
                    ((UINT32)pstMPURegInfo->bEnable             << MPU_RASR_ENABLE_Pos);
    }
    else
    {
        MPU->RBAR = 0x00;
        MPU->RASR = 0x00;
    }
}

void osConfigStackGuard(LOS_TASK_CB *pstTaskCB)
{
    MPU_REGION_INFO_S guard_region;
    
    guard_region.ucNumber = 0;
    guard_region.ucSize = MPU_GUARD_ALIGN_AND_SIZE;
    guard_region.uwBaseAddr = ALIGN((UINT32)pstTaskCB->uwTopOfStack,MPU_GUARD_ALIGN_AND_SIZE);
    guard_region.bEnable = MPU_REGION_ENABLE;
    guard_region.bShareable = MPU_ACCESS_SHAREABLE;
    guard_region.bCacheable = MPU_ACCESS_CACHEABLE;
    guard_region.bBufferable = MPU_ACCESS_BUFFERABLE;
    guard_region.bSubRegDisable = 0;
    guard_region.ucTEX = MPU_TEX_LEVEL1;
    guard_region.ucAP = MPU_REGION_PRIV_RO;
    guard_region.ucXN = MPU_INSTRUCTION_ACCESS_DISABLE;
    
    osMpuDisable();
    LOS_ConfigMpuRegion(&guard_region);
    osMpuEnable();
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
