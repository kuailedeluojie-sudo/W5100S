/**
******************************************************************************
* @file         http_client.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        http_client.c的头文件 
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com 
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef __HTTP_CLIENT_H
#define __HTTP_CLIENT_H
#include "stm32f10x.h"
#include "types.h"

extern uint8 onenet_server_ip[];
extern uint8 onenet_port;

void do_http_client(void);

#endif 
