/**
******************************************************************************
* @file         base64.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        对数据进行 base64 编码
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com   
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include <string.h>

void base64encode(char *s, char *r,unsigned int len) 
{
  char padstr[4]={0};
  char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  unsigned int i=0;
  unsigned int c=0;
  unsigned long n=0;

  c = len % 3;
  if (c > 0) { 
    for (i=0; c < 3; c++) { 
      padstr[i++] = '='; 
    } 
  }
  padstr[i]=0;

  i = 0;
  for (c=0; c < len; c+=3)
  {
    // these three 8-bit (ASCII) characters become one 24-bit number
    n = s[c]; 
    n <<= 8;
    n += s[c+1]; 
    if (c+2 > len) {
      n &= 0xff00;
    }
    n <<= 8;
    n += s[c+2];
    if (c+1 > len) {
      n &= 0xffff00;
    }

    // this 24-bit number gets separated into four 6-bit numbers
    // those four 6-bit numbers are used as indices into the base64 character list
    r[i++] = base64chars[(n >> 18) & 63];
    r[i++] = base64chars[(n >> 12) & 63];
    r[i++] = base64chars[(n >> 6) & 63];
    r[i++] = base64chars[n & 63];
  }
  i -= strlen(padstr);
  for (c=0; c<strlen(padstr); c++) {
    r[i++] = padstr[c];  
  }
  r[i] = 0;
}
