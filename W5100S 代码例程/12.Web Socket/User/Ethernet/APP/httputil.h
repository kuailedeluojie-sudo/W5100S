/**
******************************************************************************
* @file         httputil.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        httputil.c的头文件        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com 
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef  __HTTPUTIL_H__
#define  __HTTPUTIL_H__

#include <stdio.h>
#include <string.h>
#include "w5100s.h"
#include "W5100s_conf.h"
#include "utility.h"
#include "bsp_usart1.h"
#include "http_server.h"
#include "socket.h"


void proc_http(SOCKET s, u_char * buf);
void do_https(void);
void cgi_ipconfig(st_http_request *http_request);
//void trimp(uint8* src, uint8* dst, uint16 len);
uint16 make_msg_response(uint8* buf,int8* msg);

void make_cgi_response(uint16 a,int8* b,int8* c);
void make_pwd_response(int8 isRight,uint16 delay,int8* cgi_response_content, int8 isTimeout);
#endif


