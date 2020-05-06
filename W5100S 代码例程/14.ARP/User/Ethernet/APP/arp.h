/**
******************************************************************************
* @file         arp.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        arp.c的头文件        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef _ARP_H_
#define _ARP_H_
#include "w5100s.h"
#include "types.h"

#define ARP_TYPE        0x0806
#define ARP_TYPE_HI     0x08
#define ARP_TYPE_LO     0x06
  
#define ETHER_TYPE      0x0001
#define PRO_TYPE        0x0800
#define HW_SIZE         6
#define PRO_SIZE        4
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002

typedef struct _ARPMSG
{
  uint8_t  dst_mac[6];    // ff.ff.ff.ff.ff.ff
  uint8_t  src_mac[6];  
  uint16_t msg_type;      // ARP (0x0806)
  uint16_t hw_type;       // Ethernet (0x0001)
  uint16_t pro_type;      // IP  (0x0800)
  uint8_t  hw_size;       // 6
  uint8_t  pro_size;      // 4
  uint16_t opcode;        // request (0x0001), reply(0x0002)
  uint8_t  sender_mac[6];  
  uint8_t  sender_ip[4];    
  uint8_t  tgt_mac[6];    // 00.00.00.00.00.00
  uint8_t  tgt_ip[4];
  uint8_t  trailer[22];   // All zeros
}ARPMSG;

void arp_request(uint8 s, uint16 port, uint8 * SrcIP, uint8 *SrcMac, uint8 *TgtIp);
void arp_reply(uint8 s, uint16 rlen);
void do_arp(void);
#endif /* _ARP_H_ */
