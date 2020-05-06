/**
******************************************************************************
* @file         websocket.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        Web Socket ����ʵ�ֵ���غ���      
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
#include <stdlib.h>
#include "w5100s.h"
#include "socket.h"
#include "sha1.h"
#include "base64.h"
#include "utility.h"
#include "w5100s_conf.h"



#define WS_SERVER_PORT  1818    //websocket �������˿�
uint8  wsTxBuf[2048]={0x00,};   //websocket ���������ͻ���
uint8  wsRxBuf[2048]={0x00,};   //websocket ���������ջ���


uint16 Srand      = 0;
uint8  handshaked = 0;          //���ֳɹ���־λ

/**
*@brief  ��websocket�������Sec-WebSocket-Key����SHA1,base64
*@param  ��
*@return ��
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
uint8 temp[1026];             // ���Ͳ�����������
/**
*@brief  ������
*@param  ��
*@return ��
*/
void do_websocket_server(void)
{
  uint16 len=0;
  int32 i;
  volatile  uint8 fin;        // 1bit��1��ʾ���һ֡
  volatile  uint8 opcode;     // x0��ʾ������frame��x1��ʾ�ı���x2��ʾ�����Ƶ�frame
  uint8 hasmask;              // x1��ʾ��frame���������룬x0��ʾ������ 
  uint8 payloadlength;        // ���յĿͻ��˵���Ϣ����
  uint8 extendlength;         // ���س���
  uint8 extend2[2];           // 7bit+2byte
  uint8 extend8[8];           // 7bit+8byte 
  uint8 maskcode[4];          // 4byte����
  uint8 masklength; 
  uint32 contentlength;       // ����������ȡ���ĸ��س��ȣ�������չ���ݺ�Ӧ������������
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
      if(handshaked==1){             // ���ֳɹ��ж�
                                     // �������ݲ��� 
              delay_ms(50);                           
            
               temp[0] = 0x81;       // �������ݰ�ͷ
              temp[1] = rxlen;       // ���͵��ֽڳ���          
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
        memset(wsRxBuf,0x00,sizeof(wsRxBuf));                             // ���websocket���������ջ��� 
        len = recv(SOCK_WEBSOCKET, wsRxBuf, len); 
        
        if(!handshaked)                                                   // ����
        {
          char sec_ws_key[32]={0x00,};
          char accept_key[32]={0x00,};         
  //        printf("WS Rx: %s\r\n",wsRxBuf);
          //get Sec-WebSocket-Key:
          if(strstr((char const*)wsRxBuf,"Sec-WebSocket-Key: "))          // ����websocket�ͻ������ְ�Sec-WebSocket-Key�״γ��ֵĵ�ַ
          {
            mid((char*)wsRxBuf,"Sec-WebSocket-Key: ","\r\n",sec_ws_key);  // ��ȡwebsocket�ͻ������ְ�Sec-WebSocket-Key
   //         printf("----------Sec-WebSocket-Key: %s\r\n",sec_ws_key);
            calc_accept_key(sec_ws_key,accept_key);                       // ȡ����Sec-WebSocket-Key,��һ��magic string"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"l=���ӳ�һ���µ�key��
                                                                          // ���µ�key��SH1���룬����һ���ж������λ16���������ɵļ��ܴ�
                                                                          // �Ѽ��ܴ�����base64�����������յ�key�����key����sec-websocket-key
            /*���websocket���������͵�websocket�ͻ���*/
            sprintf((char*)wsTxBuf,"HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n",accept_key);
            send(SOCK_WEBSOCKET,wsTxBuf,strlen((char*)wsTxBuf));
          }
          handshaked=1;                                                   // �������
        }else{                                                            // �����ͻ��˵���Ϣ                                      
          if(len<2){                                                      // ��Чͷ 
            printf("Dataframe header invalid!\r\n");
            return;
          }
           //��һ���ֽ�          
           fin = (wsRxBuf[0] & 0x80) == 0x80;
           opcode = wsRxBuf[0] & 0x0f;
           //�ڶ����ֽ�
           hasmask = (wsRxBuf[1] & 0x80) == 0x80;
           payloadlength = wsRxBuf[1] & 0x7f;
           extendlength=0;
//           uint8 extend2[2];
//           uint8 extend8[8];
//           uint8 maskcode[4];
            masklength=0;
            contentlength=0;
//           uint8* content;
          
            
          if(payloadlength==126){                      // ��Ч���ݳ���0-125
            memcpy(extend2,wsRxBuf+2,2);
            extendlength=2;
          }else if(payloadlength==127){                // ��Ч���ݳ��ȴ���125
            memcpy(extend8,wsRxBuf+2,8);
            extendlength=8;
          }
            
          if(hasmask){                                 // �������봦�����������벻����
            memcpy(maskcode,wsRxBuf+extendlength+2,4);
            masklength=4;                              // 4������
          }
  
          if(extendlength==0){                         // 7bitȡ����ֵ������0-125֮�䣬���Ǹ������ݳ���
            contentlength=payloadlength;
          }else if(extendlength==2){                   // (7bit+2byte)����126��ʾ��������byteȡ�޷���16λ����ֵ���Ǹ��س���  
            contentlength=extend2[0]*256+extend2[1];
            if(contentlength>1024*100) contentlength=1024*100;
          }else{                                       // (7bit+8byte)127��ʾ��8��byte��ȡ64λ�޷�������ֵ���Ǹ��س���
            int32 n=1;
            for ( i = 7; i >= 0; i--)
            {
              contentlength += extend8[i] * n;
              n *= 256;
            }
          }
          
          /**/
          content=wsRxBuf+extendlength+masklength+2;
          if(hasmask){                                 //���룬�������Ϊ1-4byte����ѭ��������byte��������
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

