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
#ifdef WITH_LWIP
#include "eth.h"
#include "gd32f4xx_enet.h"
#include "gd32f4xx_enet_eval.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include <string.h>


#define netifINTERFACE_TASK_STACK_SIZE              ( 4096u )
#define netifINTERFACE_TASK_PRIORITY                ( 6u   )
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                      ( 1000u )
#define LOWLEVEL_OUTPUT_WAITING_TIME              (2500u)

/* ENET RxDMA/TxDMA descriptor */
extern enet_descriptors_struct  rxdesc_tab[ENET_RXBUF_NUM], txdesc_tab[ENET_TXBUF_NUM];

/* ENET receive buffer  */
extern uint8_t rx_buff[ENET_RXBUF_NUM][ENET_RXBUF_SIZE]; 

/* ENET transmit buffer */
extern uint8_t tx_buff[ENET_TXBUF_NUM][ENET_TXBUF_SIZE]; 

/*global transmit and receive descriptors pointers */
extern enet_descriptors_struct  *dma_current_txdesc;
extern enet_descriptors_struct  *dma_current_rxdesc;

/* preserve another ENET RxDMA/TxDMA ptp descriptor for normal mode */
enet_descriptors_struct  ptp_txstructure[ENET_TXBUF_NUM];
enet_descriptors_struct  ptp_rxstructure[ENET_RXBUF_NUM];



/* Semaphore to signal incoming packets */
static sys_sem_t s_rx_semaphore;

static sys_sem_t s_tx_semaphore;


/* Os ethernet interrupt handler */
void ENET_IRQHandler(void)
{
    /* frame received */
    if(SET == enet_interrupt_flag_get(ENET_DMA_INT_FLAG_RS)){ 
        /* give the semaphore to wakeup LwIP task */
        sys_sem_signal(&s_rx_semaphore);
    }

    /* clear the enet DMA Rx interrupt pending bits */
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_RS_CLR);
    enet_interrupt_flag_clear(ENET_DMA_INT_FLAG_NI_CLR);
}



static void eth_thread(void *arg)
{
    while (1)
    {
        if(SYS_ARCH_TIMEOUT != sys_arch_sem_wait(&s_rx_semaphore, TIME_WAITING_FOR_INPUT))
            ethernetif_input(arg);
    }
}


/* MAC address: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   2
#define MAC_ADDR1   0xA
#define MAC_ADDR2   0xF
#define MAC_ADDR3   0xE
#define MAC_ADDR4   0xD
#define MAC_ADDR5   6


static int8_t eth_init(struct netif *netif)
{
    uint32_t i;

    /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
    enet_system_setup();

    /* set netif MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set netif MAC hardware address */
    netif->hwaddr[0] = MAC_ADDR0;
    netif->hwaddr[1] = MAC_ADDR1;
    netif->hwaddr[2] = MAC_ADDR2;
    netif->hwaddr[3] = MAC_ADDR3;
    netif->hwaddr[4] = MAC_ADDR4;
    netif->hwaddr[5] = MAC_ADDR5;

    /* set netif maximum transfer unit */
    netif->mtu = 1500;

    /* accept broadcast address and ARP traffic */
    //netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;


    //s_pxNetIf =netif;

    /* initialize MAC address in ethernet MAC */ 
    enet_mac_address_set(ENET_MAC_ADDRESS0, netif->hwaddr);

    /* initialize descriptors list: chain/ring mode */
#ifdef SELECT_DESCRIPTORS_ENHANCED_MODE
    enet_ptp_enhanced_descriptors_chain_init(ENET_DMA_TX);
    enet_ptp_enhanced_descriptors_chain_init(ENET_DMA_RX);
#else

    enet_descriptors_chain_init(ENET_DMA_TX);
    enet_descriptors_chain_init(ENET_DMA_RX);

//	  enet_descriptors_ring_init(ENET_DMA_TX);
//	  enet_descriptors_ring_init(ENET_DMA_RX);	

#endif /* SELECT_DESCRIPTORS_ENHANCED_MODE */  

    /* enable ethernet Rx interrrupt */
    {    int i;
        for(i=0; i<ENET_RXBUF_NUM; i++){ 
            enet_rx_desc_immediate_receive_complete_interrupt(&rxdesc_tab[i]);
        }
    }


#ifdef CHECKSUM_BY_HARDWARE
    /* enable the TCP, UDP and ICMP checksum insertion for the Tx frames */
    for(i=0; i < ENET_TXBUF_NUM; i++){
        enet_transmit_checksum_config(&txdesc_tab[i], ENET_CHECKSUM_TCPUDPICMP_FULL);
    }
#endif /* CHECKSUM_BY_HARDWARE */


    if (ERR_OK != sys_sem_new(&s_rx_semaphore, 1))
    {
        return -1;
    }

    if (ERR_OK != sys_sem_new(&s_tx_semaphore, 1))
    {
        return -1;
    }

    //sys_arch_sem_wait(&s_rx_semaphore, 0);
    /* create the task that handles the ETH_MAC */
    sys_thread_new((char *)"Eth_if", eth_thread, netif, netifINTERFACE_TASK_STACK_SIZE, netifINTERFACE_TASK_PRIORITY);

    /* Enable MAC and DMA transmission and reception */
    enet_enable();


    return 0;
}

static int8_t eth_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    uint8_t *buffer ;
    uint16_t framelength = 0;
    ErrStatus reval = ERROR;

	//printf("eth output: 0x%x 0x%x\r\n", netif, p);

    SYS_ARCH_DECL_PROTECT(sr);


    //if (sys_arch_sem_wait(&s_tx_semaphore, LOWLEVEL_OUTPUT_WAITING_TIME))
    if(SYS_ARCH_TIMEOUT != sys_arch_sem_wait(&s_tx_semaphore, LOWLEVEL_OUTPUT_WAITING_TIME))
    {
        SYS_ARCH_PROTECT(sr);

        while((uint32_t)RESET != (dma_current_txdesc->status & ENET_TDES0_DAV)){}

        buffer = (uint8_t *)(enet_desc_information_get(dma_current_txdesc, TXDESC_BUFFER_1_ADDR));

        for(q = p; q != NULL; q = q->next){ 
            memcpy((uint8_t *)&buffer[framelength], q->payload, q->len);
            framelength = framelength + q->len;
        }

    /* transmit descriptors to give to DMA */ 
#ifdef SELECT_DESCRIPTORS_ENHANCED_MODE
        reval = ENET_NOCOPY_PTPFRAME_TRANSMIT_ENHANCED_MODE(framelength, NULL);
#else
        reval = ENET_NOCOPY_FRAME_TRANSMIT(framelength);
#endif /* SELECT_DESCRIPTORS_ENHANCED_MODE */

        SYS_ARCH_UNPROTECT(sr);

        /* give semaphore and exit */
        sys_sem_signal(&s_tx_semaphore);
	}

    if(SUCCESS == reval){
        return ERR_OK;
    }else{
        while(1){
        }
    }
}


static struct pbuf *eth_input(struct netif *netif)
{
    struct pbuf *p= NULL, *q;
    uint32_t l =0;
    u16_t len;
    uint8_t *buffer;

    /* obtain the size of the packet and put it into the "len" variable. */
    len = enet_desc_information_get(dma_current_rxdesc, RXDESC_FRAME_LENGTH);
    buffer = (uint8_t *)(enet_desc_information_get(dma_current_rxdesc, RXDESC_BUFFER_1_ADDR));

    if (len > 0){
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

    if (p != NULL){
        for(q = p; q != NULL; q = q->next){
            memcpy((uint8_t *)q->payload, (u8_t*)&buffer[l], q->len);
            l = l + q->len;
        }
    }
#ifdef SELECT_DESCRIPTORS_ENHANCED_MODE
    ENET_NOCOPY_PTPFRAME_RECEIVE_ENHANCED_MODE(NULL);
#else
    ENET_NOCOPY_FRAME_RECEIVE();
#endif /* SELECT_DESCRIPTORS_ENHANCED_MODE */

    return p;
}



struct ethernet_api g_eth_api = {
    .init     = eth_init,
    .output   = eth_output,
    .input    = eth_input,
};
#endif


