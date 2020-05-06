/**
******************************************************************************
* @file         udp_multicast.h                                 
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udp_multicast.c的头文件  
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com  
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef _UDP_MULTICAST_H_
#define _UDP_MULTICAST_H_

#include <stdint.h>
#include "types.h"

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
#define DATA_BUF_SIZE  2048
#endif

extern uint16 Multicast_port;
extern uint8 Multicast_IP[4];

#define Sn_MR_UDP_MULTICAST 0x92

/* UDP Multicast Initialization Configuration */
void udp_multicast_init(uint8 s,uint8* addr,uint16 port,uint8* mac);

/* UDP Multicast Loopback test example */
void do_udp_multicast(void);

#endif
