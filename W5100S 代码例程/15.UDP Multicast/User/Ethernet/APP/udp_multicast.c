/**
******************************************************************************
* @file         udp_multicast.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udpЭ���鲥���ݻػ�����        
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
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

uint16 Multicast_port = 3000;                              // �鲥�˿�
uint8 Multicast_mac[6] = {0x01,0x00,0x5e,0x01,0x01,0x0b};  // �鲥MAC
uint8 Multicast_IP[4] = {224,1,1,11};                      // �鲥IP

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
  switch(getSn_SR(SOCK_UDPS))                                                  // ��ȡsocket��״̬
  {
    case SOCK_CLOSED:                                                          // socket���ڹر�״̬
			udp_multicast_init(SOCK_UDPS,Multicast_IP,Multicast_port,Multicast_mac); // UDP�鲥����������
      socket(SOCK_UDPS,Sn_MR_UDP_MULTICAST,Multicast_port,0);                  // ��ʼ��socket�鲥ģʽ����(socket�ײ����޸�)
      break;    
    case SOCK_UDP:                                                             // socket��ʼ�����
      if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
      {
        setSn_IR(SOCK_UDPS, Sn_IR_RECV);                                       // ������ж�
      }
      if((len=getSn_RX_RSR(SOCK_UDPS))>0)                                      // ���յ�����
      {
        buff[len-8]=0x00;                                                      // ����ַ���������
        recvfrom(SOCK_UDPS,buff, len, Multicast_IP,&Multicast_port);           // W5500���ռ����������������                                                   
        printf("%s\r\n",buff);                                                 // ��ӡ���ջ��� 
        sendto(SOCK_UDPS,buff,len-8, Multicast_IP, Multicast_port);            // W5500�ѽ��յ������ݷ��͸�Remote
      }
      break;  
  }
}
