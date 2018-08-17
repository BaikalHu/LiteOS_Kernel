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

#ifndef _CORTEX_MPU_H
#define _CORTEX_MPU_H

//#include "los_base.h"
#include "los_err.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* CORTEX_MPU_GUARD_Region SIZE */
#define MPU_GUARD_ALIGN_AND_SIZE         ((UINT8)32)

/* CORTEX_MPU_Region_Enable CORTEX MPU Region Enable */
#define  MPU_REGION_ENABLE     ((UINT8)0x01U)
#define  MPU_REGION_DISABLE    ((UINT8)0x00U)

/* CORTEX_MPU_Instruction_Access CORTEX MPU Instruction Access */
#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((UINT8)0x00U)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((UINT8)0x01U)

/* CORTEX_MPU_Access_Shareable CORTEX MPU Instruction Access Shareable */
#define  MPU_ACCESS_SHAREABLE        ((UINT8)0x01U)
#define  MPU_ACCESS_NOT_SHAREABLE    ((UINT8)0x00U)

/* CORTEX_MPU_Access_Cacheable CORTEX MPU Instruction Access Cacheable */
#define  MPU_ACCESS_CACHEABLE         ((UINT8)0x01U)
#define  MPU_ACCESS_NOT_CACHEABLE     ((UINT8)0x00U)

/* CORTEX_MPU_Access_Bufferable CORTEX MPU Instruction Access Bufferable */
#define  MPU_ACCESS_BUFFERABLE         ((UINT8)0x01U)
#define  MPU_ACCESS_NOT_BUFFERABLE     ((UINT8)0x00U)

/* CORTEX_MPU_TEX_Levels MPU TEX Levels */
#define  MPU_TEX_LEVEL0    ((UINT8)0x00U)
#define  MPU_TEX_LEVEL1    ((UINT8)0x01U)
#define  MPU_TEX_LEVEL2    ((UINT8)0x02U)

/* CORTEX_MPU_Region_Size CORTEX MPU Region Size */
#define   MPU_REGION_SIZE_32B      ((UINT8)0x04U)
#define   MPU_REGION_SIZE_64B      ((UINT8)0x05U)
#define   MPU_REGION_SIZE_128B     ((UINT8)0x06U) 
#define   MPU_REGION_SIZE_256B     ((UINT8)0x07U) 
#define   MPU_REGION_SIZE_512B     ((UINT8)0x08U) 
#define   MPU_REGION_SIZE_1KB      ((UINT8)0x09U)  
#define   MPU_REGION_SIZE_2KB      ((UINT8)0x0AU)
#define   MPU_REGION_SIZE_4KB      ((UINT8)0x0BU) 
#define   MPU_REGION_SIZE_8KB      ((UINT8)0x0CU) 
#define   MPU_REGION_SIZE_16KB     ((UINT8)0x0DU) 
#define   MPU_REGION_SIZE_32KB     ((UINT8)0x0EU) 
#define   MPU_REGION_SIZE_64KB     ((UINT8)0x0FU) 
#define   MPU_REGION_SIZE_128KB    ((UINT8)0x10U)
#define   MPU_REGION_SIZE_256KB    ((UINT8)0x11U)
#define   MPU_REGION_SIZE_512KB    ((UINT8)0x12U)
#define   MPU_REGION_SIZE_1MB      ((UINT8)0x13U) 
#define   MPU_REGION_SIZE_2MB      ((UINT8)0x14U) 
#define   MPU_REGION_SIZE_4MB      ((UINT8)0x15U) 
#define   MPU_REGION_SIZE_8MB      ((UINT8)0x16U) 
#define   MPU_REGION_SIZE_16MB     ((UINT8)0x17U)
#define   MPU_REGION_SIZE_32MB     ((UINT8)0x18U)
#define   MPU_REGION_SIZE_64MB     ((UINT8)0x19U)
#define   MPU_REGION_SIZE_128MB    ((UINT8)0x1AU)
#define   MPU_REGION_SIZE_256MB    ((UINT8)0x1BU)
#define   MPU_REGION_SIZE_512MB    ((UINT8)0x1CU)
#define   MPU_REGION_SIZE_1GB      ((UINT8)0x1DU) 
#define   MPU_REGION_SIZE_2GB      ((UINT8)0x1EU) 
#define   MPU_REGION_SIZE_4GB      ((UINT8)0x1FU)

/* CORTEX_MPU_Region_Permission_Attributes CORTEX MPU Region Permission Attributes */
#define  MPU_REGION_NO_ACCESS      ((UINT8)0x00U)  
#define  MPU_REGION_PRIV_RW        ((UINT8)0x01U) 
#define  MPU_REGION_PRIV_RW_URO    ((UINT8)0x02U)  
#define  MPU_REGION_FULL_ACCESS    ((UINT8)0x03U)  
#define  MPU_REGION_PRIV_RO        ((UINT8)0x05U) 
#define  MPU_REGION_PRIV_RO_URO    ((UINT8)0x06U)

/* CORTEX_MPU_Region_Number CORTEX MPU Region Number */
#define  MPU_REGION_NUMBER0    ((UINT8)0x00U)  
#define  MPU_REGION_NUMBER1    ((UINT8)0x01U) 
#define  MPU_REGION_NUMBER2    ((UINT8)0x02U)  
#define  MPU_REGION_NUMBER3    ((UINT8)0x03U)  
#define  MPU_REGION_NUMBER4    ((UINT8)0x04U) 
#define  MPU_REGION_NUMBER5    ((UINT8)0x05U)
#define  MPU_REGION_NUMBER6    ((UINT8)0x06U)
#define  MPU_REGION_NUMBER7    ((UINT8)0x07U)

#define IS_MPU_REGION_ENABLE(STATE) (((STATE) == MPU_REGION_ENABLE) || \
                                     ((STATE) == MPU_REGION_DISABLE))

#define IS_MPU_INSTRUCTION_ACCESS(STATE) (((STATE) == MPU_INSTRUCTION_ACCESS_ENABLE) || \
                                          ((STATE) == MPU_INSTRUCTION_ACCESS_DISABLE))

#define IS_MPU_ACCESS_SHAREABLE(STATE)   (((STATE) == MPU_ACCESS_SHAREABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_SHAREABLE))

#define IS_MPU_ACCESS_CACHEABLE(STATE)   (((STATE) == MPU_ACCESS_CACHEABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_CACHEABLE))

#define IS_MPU_ACCESS_BUFFERABLE(STATE)   (((STATE) == MPU_ACCESS_BUFFERABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_BUFFERABLE))

#define IS_MPU_TEX_LEVEL(TYPE) (((TYPE) == MPU_TEX_LEVEL0)  || \
                                ((TYPE) == MPU_TEX_LEVEL1)  || \
                                ((TYPE) == MPU_TEX_LEVEL2))

#define IS_MPU_REGION_PERMISSION_ATTRIBUTE(TYPE) (((TYPE) == MPU_REGION_NO_ACCESS)   || \
                                                  ((TYPE) == MPU_REGION_PRIV_RW)     || \
                                                  ((TYPE) == MPU_REGION_PRIV_RW_URO) || \
                                                  ((TYPE) == MPU_REGION_FULL_ACCESS) || \
                                                  ((TYPE) == MPU_REGION_PRIV_RO)     || \
                                                  ((TYPE) == MPU_REGION_PRIV_RO_URO))

#define IS_MPU_REGION_NUMBER(NUMBER)    (((NUMBER) == MPU_REGION_NUMBER0) || \
                                         ((NUMBER) == MPU_REGION_NUMBER1) || \
                                         ((NUMBER) == MPU_REGION_NUMBER2) || \
                                         ((NUMBER) == MPU_REGION_NUMBER3) || \
                                         ((NUMBER) == MPU_REGION_NUMBER4) || \
                                         ((NUMBER) == MPU_REGION_NUMBER5) || \
                                         ((NUMBER) == MPU_REGION_NUMBER6) || \
                                         ((NUMBER) == MPU_REGION_NUMBER7))

#define IS_MPU_REGION_SIZE(SIZE)    (((SIZE) == MPU_REGION_SIZE_32B)   || \
                                     ((SIZE) == MPU_REGION_SIZE_64B)   || \
                                     ((SIZE) == MPU_REGION_SIZE_128B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_256B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_512B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_1KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_8KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_16KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_32KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_64KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_128KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_256KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_512KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_1MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_8MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_16MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_32MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_64MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_128MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_256MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_512MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_1GB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2GB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4GB))

#define IS_MPU_REGION_ADDR_ALIGN(ADDR, SIZE)  ((ADDR) % (SIZE) == 0)

#define LOS_ERRNO_MPU_NULL_PARAM                     LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x00)
#define LOS_ERRNO_MPU_INVALID_ENABLE                 LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x01)
#define LOS_ERRNO_MPU_INVALID_NUMBER                 LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x02)
#define LOS_ERRNO_MPU_INVALID_XN                     LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x03)
#define LOS_ERRNO_MPU_INVALID_SHAREABLE              LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x04)
#define LOS_ERRNO_MPU_INVALID_CACHEABLE              LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x05)
#define LOS_ERRNO_MPU_INVALID_BUFFERABLE             LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x06)
#define LOS_ERRNO_MPU_INVALID_TEX                    LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x07)
#define LOS_ERRNO_MPU_INVALID_AP                     LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x08)
#define LOS_ERRNO_MPU_INVALID_SIZE                   LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x09)
#define LOS_ERRNO_MPU_INVALID_ADDR                   LOS_ERRNO_OS_FATAL(LOS_MOD_MPU, 0x0A)


typedef struct tagMpuRegionInfo
{
    BOOL        bEnable;
    BOOL        bShareable;
    BOOL        bCacheable;
    BOOL        bBufferable;
    BOOL        bSubRegDisable;
    UINT8       ucTEX;
    UINT8       ucNumber;
    UINT8       ucAP;
    UINT8       ucXN;
    UINT32      uwBaseAddr;
    UINT32      ucSize;
} MPU_REGION_INFO_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _CORTEX_MPU_H */
