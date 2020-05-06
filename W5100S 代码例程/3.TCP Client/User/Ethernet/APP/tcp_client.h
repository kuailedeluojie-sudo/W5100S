/**
******************************************************************************
* @file         tcp_client.h                                 
* @version      V1.0										  		
* @date         2018-06-18								 
* @brief        tcp_client.c的头文件	
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114																										 
******************************************************************************
*/
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <stdint.h>

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
#define DATA_BUF_SIZE			2048
#endif

/* TCP client Loopback test example */
void do_tcp_client(void);

#endif
