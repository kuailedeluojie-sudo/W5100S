/**
******************************************************************************
* @file         arp.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        arp请求与响应的组包与拆包        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com  
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "arp.h"
#include "utility.h"
#include "socket.h"
#include "w5100s_conf.h"
#include <string.h>


uint8_t tcp_conn_flag;

extern uint8_t req;
unsigned char Rx_Buffer[2048];  //端口接收数据缓冲区 

/*
* target_ip_addr[4]:需要知道远程设备的IP地址
*/
uint8_t target_ip_addr[4]={192,168,1,151};

uint8_t* pMACRAW;

ARPMSG pARPMSG;
ARPMSG *aARPMSG;

uint8_t ret_arp_reply[128];

/*
 *功能：响应arp请求
 *参数：无
 *返回值：无
*/
void do_arp(void)
{
  uint16_t rlen =0;
  uint16_t aPort=5000;

  switch(getSn_SR(SOCK_ARP))
  {
    case SOCK_CLOSED:  
      close(SOCK_ARP);                         // close the SOCKET
      socket(SOCK_ARP,Sn_MR_MACRAW,aPort,0);   // open the SOCKET with MACRAW mode
      while(getSn_SR(SOCK_ARP) != SOCK_MACRAW){
        delay_ms(100);
      }  
    case SOCK_MACRAW:
      arp_request(SOCK_ARP, aPort, ConfigMsg.lip, ConfigMsg.mac, target_ip_addr);    
      rlen=getSn_RX_RSR(SOCK_ARP);             // 定义len为已接收数据的长度
      if((rlen = getSn_RX_RSR(SOCK_ARP)) > 0)
      {
        arp_reply(SOCK_ARP, rlen);
      }
      delay_ms(1000);
      break;
  } 
}

void arp_request(uint8 s, uint16 port, uint8 *SrcIp, uint8 *SrcMac, uint8 *TgtIp)
{
  uint32 tip = 0xFFFFFFFF;
  uint16 i =0;
  
  for( i=0; i<6 ; i++) 
  {
    pARPMSG.dst_mac[i] = 0xFF;                 // BROADCAST_MAC[i];
    pARPMSG.src_mac[i] = SrcMac[i];            // BROADCAST_MAC[i];
    pARPMSG.sender_mac[i] = SrcMac[i];         // BROADCAST_MAC[i];
    pARPMSG.tgt_mac[i] = 0x00;  
  }
  pARPMSG.sender_mac[5]=0x89;  
  pARPMSG.msg_type = htons(ARP_TYPE);
  pARPMSG.hw_type   = htons(ETHER_TYPE);
  pARPMSG.pro_type  = htons(PRO_TYPE);         // IP  (0x0800)
  pARPMSG.hw_size    =  HW_SIZE;               // 6
  pARPMSG.pro_size   = PRO_SIZE;               // 4
  pARPMSG.opcode      =  htons(ARP_REQUEST);   // request (0x0001), reply(0x0002)
  
  for( i=0; i<4 ; i++) 
  {
    pARPMSG.sender_ip[i] = SrcIp[i];           // BROADCAST_MAC[i];
    pARPMSG.tgt_ip[i] = TgtIp[i];
  } 
  if( sendto(s,(uint8*)&pARPMSG,sizeof(pARPMSG),(uint8 *)&tip,port) ==0)
  {
    printf( "\r\n Fail to send ping-reply packet  \r\n") ;        
  }
  else
  {
    if(pARPMSG.opcode==htons(ARP_REQUEST))
    {
      printf("\r\n Who has ");
      printf( "%3d.%3d.%d.%3d ?", pARPMSG.tgt_ip[0], pARPMSG.tgt_ip[1], pARPMSG.tgt_ip[2], pARPMSG.tgt_ip[3]) ;
      printf( " Tell %3d.%3d.%d.%3d\r\n", (pARPMSG.sender_ip[0]) , (pARPMSG.sender_ip[1]), (pARPMSG.sender_ip[2]), (pARPMSG.sender_ip[3]));              
    }
    else
    {
      printf(" opcode has wrong value. check opcode !\r\n");
    }
  }
}

void arp_reply(uint8 s, uint16 rlen)
{
  uint16 mac_destport;  
  uint8 mac_destip[4];

  recvfrom(s,(uint8 *)Rx_Buffer,rlen,mac_destip,&mac_destport);

  if( Rx_Buffer[12]==ARP_TYPE_HI && Rx_Buffer[13]==ARP_TYPE_LO )
  {
    aARPMSG =(ARPMSG*) Rx_Buffer;  
    if((aARPMSG->opcode) == htons(ARP_REPLY)) {

      memset(ret_arp_reply,0x00,128);
      sprintf((int8*)ret_arp_reply," %3d.%3d.%d.%3d is at %x.%x.%x.%x.%x.%x\r\n",
                            (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]),(aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3]),
                            (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]),
                            (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5]));
      
      printf( " %3d.%3d.%d.%3d ", 
               (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]), 
               (aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3])) ;
      printf(" is at  ");
      printf(" %x.%x.%x.%x.%x.%x\r\n", 
               (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]), 
               (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;
      
    
      
    }else if((aARPMSG->opcode) == htons(ARP_REQUEST)) {

      printf(" Who has %3d.%3d.%d.%3d ?", 
               (aARPMSG->tgt_ip[0]),  (aARPMSG->tgt_ip[1]), 
               (aARPMSG->tgt_ip[2]),  (aARPMSG->tgt_ip[3])) ;
      printf(" Tell ");
      printf("%x.%x.%x.%x.%x.%x\r\n",  
               (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]), 
               (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;

    }else{
      printf(" This msg is not ARP reply : opcode is not 0x02 \n");  
    }
  }
}
