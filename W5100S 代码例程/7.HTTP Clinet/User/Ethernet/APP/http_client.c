/**
******************************************************************************
* @file         http_client.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        http 连接OneNET云平台上传数据          
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
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
#define onenet_id     "24638289"                                 /*设备ID*/
#define apikey        "8OzzO1JDGE8cNXpGGCcXbDgUNC4="             /*设备API*/

uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val);   //onenet数据打包

char tempStr[7]={'W','5','1','0','0','S'};                       /*上传的数值*/
uint8 onenet_server_ip[4]={183,230,40,33};                       /*api.onenet.net  的ip地址*/
uint8 onenet_port=80;                                            /*api.onenet.net  的端口号*/  
char buffer[1024]={0};
/**
*@brief  执行http client主函数
*@param  无
*@return 无
*/
void do_http_client(void)
{  
  uint16 anyport=5000;
  uint16 len;
  switch(getSn_SR(SOCK_HTTPC))                                   /*获取socket SOCK_TCPS 的状态*/
  {
    case SOCK_INIT:                                              /*socket初始化完成*/
    connect(SOCK_HTTPC, onenet_server_ip ,onenet_port);          /*打开socket端口*/
     
      break;  
    case SOCK_ESTABLISHED:                                       /*socket连接建立*/
      if(getSn_IR(SOCK_HTTPC) & Sn_IR_CON)  
      {
        setSn_IR(SOCK_HTTPC, Sn_IR_CON);                         /*清除接收中断标志*/  
      } 
                                                                    
      len = HTTP_PostPkt(buffer,apikey,onenet_id, "HTTP_Client", tempStr);       /*temp 标识  ，tempStr 数据流*/
      send(SOCK_HTTPC, (uint8 * )buffer,len);  
      delay_ms(1000);      
      break;
    case SOCK_CLOSE_WAIT:                                        /*socket等待关闭状态*/
      close(SOCK_HTTPC);  
      break;
    case SOCK_CLOSED:                                            /*socket关闭*/
      socket(SOCK_HTTPC, Sn_MR_TCP,anyport++ , 0x00);  
      break;
    default:
      break;
  }  
}

/**
*@brief  OneNet 数据打包函数
*@param  无
*@return 无
*/

uint32_t HTTP_PostPkt(char *pkt, char *key, char *devid, char *dsid, char *val)
{
  char dataBuf[100] = {0};
  char lenBuf[10] = {0};
  *pkt = 0;

  sprintf( dataBuf, ",;%s,%s", dsid, val);                      /*采用分割字符串格式:type = 5*/
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
