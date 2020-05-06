/**
******************************************************************************
* @file         websocket.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        Web Socket 功能实现的相关函数      
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
#include <stdlib.h>
#include "w5100s.h"
#include "socket.h"
#include "sha1.h"
#include "base64.h"
#include "utility.h"
#include "w5100s_conf.h"



#define WS_SERVER_PORT  1818    //websocket 服务器端口
uint8  wsTxBuf[2048]={0x00,};   //websocket 服务器发送缓存
uint8  wsRxBuf[2048]={0x00,};   //websocket 服务器接收缓存


uint16 Srand      = 0;
uint8  handshaked = 0;          //握手成功标志位

/**
*@brief  将websocket解析到礢ec-WebSocket-Key进行SHA1,base64
*@param  无
*@return 无
*/
void calc_accept_key(char* s, char* r)
{
  int16 i;
  SHA1Context sha;
  const char magicKey[]="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";  //
  char sInput[64]={0x00,};
  char tmpBuf[32]={0x00,};
  strcpy(sInput,s);
  strcpy(sInput+strlen(s),magicKey);
  //printf("input: %s\r\n",sInput);
  SHA1Reset(&sha);
  SHA1Input(&sha, (const unsigned char *)sInput, strlen(sInput));
  if (!SHA1Result(&sha)){
    printf("ERROR-- could not compute message digest\n");
  }else{
    for( i = 0; i < 5 ; i++){
      tmpBuf[i*4+0]=*((char*)&sha.Message_Digest[i]+3);
      tmpBuf[i*4+1]=*((char*)&sha.Message_Digest[i]+2);
      tmpBuf[i*4+2]=*((char*)&sha.Message_Digest[i]+1);
      tmpBuf[i*4+3]=*((char*)&sha.Message_Digest[i]+0);
    }
    for(i=0;i<20;i++)
    //printf("%02X", tmpBuf[i]);
    //printf("\r\n");
    base64encode(tmpBuf,r,20);
  }
}
uint8 temp[1026];             // 发送测试数据数组
/**
*@brief  服务器
*@param  无
*@return 无
*/
void do_websocket_server(void)
{
  uint16 len=0;
  int32 i;
  volatile  uint8 fin;        // 1bit，1表示最后一帧
  volatile  uint8 opcode;     // x0表示是延续frame，x1表示文本，x2表示二进制的frame
  uint8 hasmask;              // x1表示该frame包包含掩码，x0表示无掩码 
  uint8 payloadlength;        // 接收的客户端的信息长度
  uint8 extendlength;         // 负载长度
  uint8 extend2[2];           // 7bit+2byte
  uint8 extend8[8];           // 7bit+8byte 
  uint8 maskcode[4];          // 4byte掩码
  uint8 masklength; 
  uint32 contentlength;       // 长度是上面取出的负载长度，包括扩展数据和应用数据两部分
  uint8* content;
   switch(getSn_SR(SOCK_WEBSOCKET))
  {
    case SOCK_INIT:       
      listen(SOCK_WEBSOCKET);
      break;
    case SOCK_LISTEN:

      break;
    case SOCK_ESTABLISHED:
      if(getSn_IR(SOCK_WEBSOCKET) & Sn_IR_CON)
      {
         setSn_IR(SOCK_WEBSOCKET, Sn_IR_CON);
        handshaked=0;
      }  
      if(handshaked==1){             // 握手成功判断
                                     // 发送数据测试 
              delay_ms(50);                           
            
               temp[0] = 0x81;       // 发送数据包头
              temp[1] = rxlen;       // 发送的字节长度          
              if(rxlen>0){      
               for(i=0;i<rxlen;i++){
                temp[i+2]=rxbuf[i];            
              }                         
              send(SOCK_WEBSOCKET, temp, rxlen+2);
              rxlen = 0;
              memset(temp,0,1026);
              memset(rxbuf,0,1024);
            }       
         
      }
                
      
      if ((len = getSn_RX_RSR(SOCK_WEBSOCKET)) > 0)    
      {
        memset(wsRxBuf,0x00,sizeof(wsRxBuf));                             // 清空websocket服务器接收缓存 
        len = recv(SOCK_WEBSOCKET, wsRxBuf, len); 
        
        if(!handshaked)                                                   // 握手
        {
          char sec_ws_key[32]={0x00,};
          char accept_key[32]={0x00,};         
  //        printf("WS Rx: %s\r\n",wsRxBuf);
          //get Sec-WebSocket-Key:
          if(strstr((char const*)wsRxBuf,"Sec-WebSocket-Key: "))          // 返回websocket客户端握手包Sec-WebSocket-Key首次出现的地址
          {
            mid((char*)wsRxBuf,"Sec-WebSocket-Key: ","\r\n",sec_ws_key);  // 提取websocket客户端握手包Sec-WebSocket-Key
   //         printf("----------Sec-WebSocket-Key: %s\r\n",sec_ws_key);
            calc_accept_key(sec_ws_key,accept_key);                       // 取出的Sec-WebSocket-Key,与一个magic string"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"l=连接成一个新的key串
                                                                          // 将新的key串SH1编码，生成一个有多个组两位16进制数构成的加密串
                                                                          // 把加密串进行base64编码生成最终的key，这个key就是sec-websocket-key
            /*组包websocket服务器发送到websocket客户端*/
            sprintf((char*)wsTxBuf,"HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n",accept_key);
            send(SOCK_WEBSOCKET,wsTxBuf,strlen((char*)wsTxBuf));
          }
          handshaked=1;                                                   // 握手完成
        }else{                                                            // 解析客户端的信息                                      
          if(len<2){                                                      // 无效头 
            printf("Dataframe header invalid!\r\n");
            return;
          }
           //第一个字节          
           fin = (wsRxBuf[0] & 0x80) == 0x80;
           opcode = wsRxBuf[0] & 0x0f;
           //第二个字节
           hasmask = (wsRxBuf[1] & 0x80) == 0x80;
           payloadlength = wsRxBuf[1] & 0x7f;
           extendlength=0;
//           uint8 extend2[2];
//           uint8 extend8[8];
//           uint8 maskcode[4];
            masklength=0;
            contentlength=0;
//           uint8* content;
          
            
          if(payloadlength==126){                      // 有效数据长度0-125
            memcpy(extend2,wsRxBuf+2,2);
            extendlength=2;
          }else if(payloadlength==127){                // 有效数据长度大于125
            memcpy(extend8,wsRxBuf+2,8);
            extendlength=8;
          }
            
          if(hasmask){                                 // 包含掩码处理，不包含掩码不处理
            memcpy(maskcode,wsRxBuf+extendlength+2,4);
            masklength=4;                              // 4个掩码
          }
  
          if(extendlength==0){                         // 7bit取整数值，若在0-125之间，则是负载数据长度
            contentlength=payloadlength;
          }else if(extendlength==2){                   // (7bit+2byte)若是126表示，后两个byte取无符号16位整数值，是负载长度  
            contentlength=extend2[0]*256+extend2[1];
            if(contentlength>1024*100) contentlength=1024*100;
          }else{                                       // (7bit+8byte)127表示后8个byte，取64位无符号整数值，是负载长度
            int32 n=1;
            for ( i = 7; i >= 0; i--)
            {
              contentlength += extend8[i] * n;
              n *= 256;
            }
          }
          
          /**/
          content=wsRxBuf+extendlength+masklength+2;
          if(hasmask){                                 //解码，解码规则为1-4byte掩码循环和数据byte做异或操作
            for(i=0;i<contentlength;i++){
              content[i]=(uint8)(content[i]^maskcode[i%4]);
            }
            printf("Rx: %s\r\n",content);              
          }        
        }       
      }         
      break;
    case SOCK_CLOSE_WAIT:   
      if ((len = getSn_RX_RSR(SOCK_WEBSOCKET)) > 0)
      {        
      }
      disconnect(SOCK_WEBSOCKET);
      break;
    case SOCK_CLOSED:                   
      socket(SOCK_WEBSOCKET, Sn_MR_TCP, WS_SERVER_PORT, 0x20);    // reinitialize the socket 
      break;
    default:
    break;
  }// end of switch
}

