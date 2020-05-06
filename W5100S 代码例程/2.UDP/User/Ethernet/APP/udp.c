/**
******************************************************************************
* @file         udp.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udp协议数据回环测试        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格价格优势大！
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include <stdio.h>
#include "udp.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];  
void do_udp(void)
{
  uint8 len;
  switch(getSn_SR(SOCK_UDPS))                                     // 获取socket的状态
  {
    case SOCK_CLOSED:                                             // socket处于关闭状态
      socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                   // 初始化socket
      break;    
    case SOCK_UDP:                                                // socket初始化完成
      if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
      {
        setSn_IR(SOCK_UDPS, Sn_IR_RECV);                          // 清接收中断
      }
      if((len=getSn_RX_RSR(SOCK_UDPS))>0)                         // 接收到数据
      {
        buff[len-8]=0x00;                                         // 添加字符串结束符
        recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);    // W5500接收计算机发送来的数据                                                   
        printf("%s\r\n",buff);                                    // 打印接收缓存 
        sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);     // W5500把接收到的数据发送给Remote
      }
      break;  
  }
}
