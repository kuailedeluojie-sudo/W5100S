/**
******************************************************************************
* @file         ping.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        ping.c的头文件        
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com 
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef  _PING_H_
#define  _PING_H_

#include "w5100s_conf.h"
#include "w5100s.h"
#include "socket.h"
#include "Types.h"
#include "utility.h"

#define BUF_LEN            128
#define PING_REQUEST       8
#define PING_REPLY         0
#define CODE_ZERO          0

#define SOCKET_ERROR       1
#define TIMEOUT_ERROR      2
#define SUCCESS            3
#define REPLY_ERROR        4

extern uint8 req;

typedef struct pingmsg
{
  uint8  Type;              // 0 - Ping Reply, 8 - Ping Request
  uint8  Code;              // Always 0
  uint16  CheckSum;         // Check sum
  uint16  ID;               // Identification
  uint16  SeqNum;           // Sequence Number
  int8_t  Data[BUF_LEN];    // Ping Data  : 1452 = IP RAW MTU - sizeof(Type+Code+CheckSum+ID+SeqNum)
} PINGMSGR;

void ping_auto(uint8 s, uint8 *addr);
void ping_count(uint8 s, uint16_t pCount, uint8 *addr);
uint8 ping_request(uint8 s, uint8 *addr);
uint8 ping_reply(uint8 s,  uint8 *addr, uint16_t rlen);
void do_ping(void);

#endif
