/**
******************************************************************************
* @file         udp_multicast.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udp协议组播数据回环测试        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com 
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include <stdio.h>
#include "udp_multicast.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];  

uint16 Multicast_port = 3000;                              // 组播端口
uint8 Multicast_mac[6] = {0x01,0x00,0x5e,0x01,0x01,0x0b};  // 组播MAC
uint8 Multicast_IP[4] = {224,1,1,11};                      // 组播IP

void udp_multicast_init(uint8 s,uint8* addr,uint16 port,uint8* mac)
{
	setSn_DIPR(s,addr);
	setSn_DHAR(s,mac);
	setSn_DPORT(s,port);
	setSn_PORT(s,port);
}

void do_udp_multicast(void)
{
  uint8 len;
  switch(getSn_SR(SOCK_UDPS))                                                  // 获取socket的状态
  {
    case SOCK_CLOSED:                                                          // socket处于关闭状态
			udp_multicast_init(SOCK_UDPS,Multicast_IP,Multicast_port,Multicast_mac); // UDP组播各参数配置
      socket(SOCK_UDPS,Sn_MR_UDP_MULTICAST,Multicast_port,0);                  // 初始化socket组播模式配置(socket底层已修改)
      break;    
    case SOCK_UDP:                                                             // socket初始化完成
      if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
      {
        setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                       // 清接收中断
      }
      if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                      // 接收到数据
      {
        buff[len-8]=0x00;                                                      // 添加字符串结束符
        recvfrom(SOCK_UDPS,buff, len, Multicast_IP,&Multicast_port);           // W5500接收计算机发送来的数据                                                   
        printf("%s\r\n",buff);                                                 // 打印接收缓存 
        sendto(SOCK_UDPS,buff,len-8, Multicast_IP, Multicast_port);            // W5500把接收到的数据发送给Remote
      }
      break;  
  }
}
