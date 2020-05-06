/**
******************************************************************************
* @file         http_client.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        http ����OneNET��ƽ̨�ϴ�����          
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com     
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include "http_client.h"
#include "w5100s_conf.h"  
#include "w5100s.h"
#include "socket.h"
#include "utility.h"
#define onenet_id     "24638289"                                 /*�豸ID*/
#define apikey        "8OzzO1JDGE8cNXpGGCcXbDgUNC4="             /*�豸API*/

uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val);   //onenet���ݴ��

char tempStr[7]={'W','5','1','0','0','S'};                       /*�ϴ�����ֵ*/
uint8 onenet_server_ip[4]={183,230,40,33};                       /*api.onenet.net  ��ip��ַ*/
uint8 onenet_port=80;                                            /*api.onenet.net  �Ķ˿ں�*/  
char buffer[1024]={0};
/**
*@brief  ִ��http client������
*@param  ��
*@return ��
*/
void do_http_client(void)
{  
  uint16 anyport=5000;
  uint16 len;
  switch(getSn_SR(SOCK_HTTPC))                                   /*��ȡsocket SOCK_TCPS ��״̬*/
  {
    case SOCK_INIT:                                              /*socket��ʼ�����*/
    connect(SOCK_HTTPC, onenet_server_ip ,onenet_port);          /*��socket�˿�*/
     
      break;  
    case SOCK_ESTABLISHED:                                       /*socket���ӽ���*/
      if(getSn_IR(SOCK_HTTPC) & Sn_IR_CON)  
      {
        setSn_IR(SOCK_HTTPC, Sn_IR_CON);                         /*��������жϱ�־*/  
      } 
                                                                    
      len = HTTP_PostPkt(buffer,apikey,onenet_id, "HTTP_Client", tempStr);       /*temp ��ʶ  ��tempStr ������*/
      send(SOCK_HTTPC, (uint8 * )buffer,len);  
      delay_ms(1000);      
      break;
    case SOCK_CLOSE_WAIT:                                        /*socket�ȴ��ر�״̬*/
      close(SOCK_HTTPC);  
      break;
    case SOCK_CLOSED:                                            /*socket�ر�*/
      socket(SOCK_HTTPC, Sn_MR_TCP,anyport++ , 0x00);  
      break;
    default:
      break;
  }  
}

/**
*@brief  OneNet ���ݴ������
*@param  ��
*@return ��
*/

uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val)
{
  char dataBuf[100] = {0};
  char lenBuf[10] = {0};
  *pkt = 0;

  sprintf( dataBuf, ",;%s,%s", dsid, val);                      /*���÷ָ��ַ�����ʽ:type = 5*/
  sprintf(lenBuf, "%d", strlen(dataBuf));

  strcat(pkt, "POST /devices/");
  strcat(pkt, devid);
  strcat(pkt, "/datapoints?type=5 HTTP/1.1\r\n");

  strcat(pkt, "api-key:");
  strcat(pkt, key);
  strcat(pkt, "\r\n");  

  strcat(pkt, "Host:api.heclouds.com\r\n");

  strcat(pkt, "Content-Length:");
  strcat(pkt, lenBuf);
  strcat(pkt, "\r\n\r\n");

  strcat(pkt, dataBuf);

  return strlen(pkt);
}
