/**
******************************************************************************
* @file         tcp_server.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        tcp服务器数据回环测试        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com       
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include <stdio.h>
#include "tcp_server.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];  

void do_tcp_server(void)
{  
  uint16 len=0;  
  switch(getSn_SR(SOCK_TCPS))                             // 获取socket的状态
  {
    case SOCK_CLOSED:                                     // socket处于关闭状态
      socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);   // 打开socket
      break;     
    
    case SOCK_INIT:                                       // socket已初始化状态
      listen(SOCK_TCPS);                                  // socket建立监听
      break;
    
    case SOCK_ESTABLISHED:                                // socket处于连接建立状态
    
      if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
      {
        setSn_IR(SOCK_TCPS, Sn_IR_CON);                   // 清除接收中断标志位
      }
      len=getSn_RX_RSR(SOCK_TCPS);                        // 定义len为已接收数据的长度
      if(len>0)
      {
        recv(SOCK_TCPS,buff,len);                         // 接收来自Client的数据
        buff[len]=0x00;                                   // 添加字符串结束符
        printf("%s\r\n",buff);
        send(SOCK_TCPS,buff,len);                         // 向Client发送数据
      }
      break;
    
    case SOCK_CLOSE_WAIT:                                 // socket处于等待关闭状态
      close(SOCK_TCPS);
      break;
  }
}
