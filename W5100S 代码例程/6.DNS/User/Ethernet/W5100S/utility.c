/**
******************************************************************************
* @file         utility.c                                            
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        ¹«¹²ÊµÓÃº¯Êı ÑÓÊ±º¯Êı Êı¾İ¸ñÊ½×ª»»                    
*
* @company      ÉîÛÚì¿ÊÀ¿Æ¼¼ÓĞÏŞ¹«Ë¾
* @information  WIZnet W5100S¹Ù·½´úÀíÉÌ£¬È«³Ì¼¼ÊõÖ§³Ö£¬¼Û¸ñÓÅÊÆ´ó£¡
* @website      www.wisioe.com      
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/

#include "w5100s.h"
#include "w5100s_conf.h"
#include "utility.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static u8  fac_us=0;  /*usÑÓÊ±±¶³ËÊı*/
static u16 fac_ms=0;  /*msÑÓÊ±±¶³ËÊı*/

/**
*@brief  ³õÊ¼»¯ÑÓ³Ùº¯Êı
*@param  SYSCLK:ÏµÍ³Ê±ÖÓ
*@return ÎŞ
*/
void systick_init (u8 sysclk)
{
  SysTick->CTRL&=0xfffffffb;          /*bit2Çå¿Õ,Ñ¡ÔñÍâ²¿Ê±ÖÓ  HCLK/8*/
  fac_us=sysclk/8;        
  fac_ms=(u16)fac_us*1000;
}                    

/**
*@brief  ÃëÑÓÊ±º¯Êı
*@param  time_s:ÒªÑÓÊ±ÃëÊ±¼äÊı
*@return ÎŞ
*/
void delay_s( uint32 time_s )
{
  for(;time_s>0;time_s--)
    delay_ms(1000);
}

/**
*@brief  ºÁÃëÑÓÊ±º¯Êı
*@param  time_ms:ÒªÑÓÊ±ºÁÃëÊ±¼äÊı
*@return ÎŞ
*/
void delay_ms( uint32 time_ms )
{             
  u32 temp;       
  SysTick->LOAD=(u32)time_ms*fac_ms;      /*Ê±¼ä¼ÓÔØ(SysTick->LOADÎª24bit)*/
  SysTick->VAL =0x00;                     /*Çå¿Õ¼ÆÊıÆ÷*/
  SysTick->CTRL=0x01 ;                    /*¿ªÊ¼µ¹Êı*/ 
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));      /*µÈ´ıÊ±¼äµ½´ï*/
  SysTick->CTRL=0x00;                     /*¹Ø±Õ¼ÆÊıÆ÷*/
  SysTick->VAL =0X00;                     /*Çå¿Õ¼ÆÊıÆ÷*/          
}   

/**
*@brief  Î¢ÃëÑÓÊ±º¯Êı
*@param  time_ms:ÒªÑÓÊ±Î¢ÃëÊ±¼äÊı
*@return ÎŞ
*/
void delay_us( uint32 time_us )
{    
  u32 temp;         
  SysTick->LOAD=time_us*fac_us;           /*Ê±¼ä¼ÓÔØ*/         
  SysTick->VAL=0x00;                      /*Çå¿Õ¼ÆÊıÆ÷*/
  SysTick->CTRL=0x01 ;                    /*¿ªÊ¼µ¹Êı */   
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));      /*µÈ´ıÊ±¼äµ½´ï*/
  SysTick->CTRL=0x00;                     /*¹Ø±Õ¼ÆÊıÆ÷*/
  SysTick->VAL =0X00;                     /*Çå¿Õ¼ÆÊıÆ÷*/
}

/**
*@brief  ×Ö·û×ª×ª»¯Îª8Î»ÕûĞÍº¯Êı
*@param  str:Òª×ª»¯×Ö·û´®£¬ base:
*@return num:·µ»Ø×ª»¯ºóµÄÕûĞÍÊı
*/
uint16 atoi16(char* str,uint16 base)
{
  unsigned int num = 0;
  while (*str !=0)
  num = num * base + c2d(*str++);
  return num;
}

/**
*@brief  ×Ö·û×ª×ª»¯Îª32Î»ÕûĞÍº¯Êı
*@param  str:Òª×ª»¯×Ö·û´®£¬ base:
*@return num:·µ»Ø×ª»¯ºóµÄÕûĞÍÊı
*/
uint32 atoi32(char* str,uint16 base  )
{
  uint32 num = 0;
  while (*str !=0)
    num = num * base + c2d(*str++);
  return num;
}

/**
*@brief  ÕûĞÍÊı×ª»¯Îª×Ö·û´®º¯Êı
*@param  n:Òª×ª»¯ÕûÊı£¬ str[5]:´æ·Å×ª»¯ºóµÄ×Ö·û´®  len£ºÕûĞÍÊı³¤¶È
*@return ÎŞ
*/
void itoa(uint16 n,uint8 str[5], uint8 len)
{
  uint8 i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}


/**
*@brief  °Ñ×Ö·û´®×ª»¯ÎªÊ®½øÖÆ»òÊ®Áù½øÖÆÊıº¯Êı
*@param  str:Òª×ª»¯×Ö·û´®£¬ len£ºÕûĞÍÊı³¤¶È
*@return ³É¹¦ - 1, Ê§°Ü - 0
*/
int validatoi(char* str, int base,int* ret)
{
  int c;
  char* tstr = str;
  if(str == 0 || *str == '\0') return 0;
  while(*tstr != '\0')
  {
    c = c2d(*tstr);
    if( c >= 0 && c < base) tstr++;
    else    return 0;
  }
  
  *ret = atoi16(str,base);
  return 1;
}

/**
*@brief  ÓÃĞÂµÄ×Ö·ûÈ¥Ìæ»»×Ö·û´®ÖĞÌØÊâµÄ×Ö·û
*@param  str:Ìæ»»ºó×Ö·û´®£¬oldchar:ÌØÊâµÄ×Ö·û£¬newchar£ºĞÂµÄ×Ö·û  
*@return ÎŞ
*/
void replacetochar(char * str,  char oldchar,char newchar  )
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar)
      str[x] = newchar;  
}

/**
*@brief  °ÑÊ®½øÖÆÊı×ª»¯Îª×Ö·ûĞÍ
*@param  c:Òª×ª»¯Ê®½øÖÆÊı¾İ
*@return ·µ»ØÒ»¸ö×Ö·ûĞÍÊı¾İ
*/
char c2d(uint8 c  )
{
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return 10 + c -'a';
  if (c >= 'A' && c <= 'F')
    return 10 + c -'A';
  return (char)c;
}

/**
*@brief  16Î»×Ö·û¸ß8Î»µÍ8Î»×ª»»
*@param  i:Òª×ª»¯µÄÊı¾İ
*@return ×ª»»ºóµÄÊı¾İ
*/
uint16 swaps(uint16 i)
{
  uint16 ret=0;
  ret = (i & 0xFF) << 8;
  ret |= ((i >> 8)& 0xFF);
  return ret;  
}
/**
*@brief  32Î»×Ö·û¸ßµÍÎ»±ä»»
*@param  i:Òª×ª»¯µÄÊı¾İ
*@return ª»»ºóµÄÊı¾İ
*/
uint32 swapl(uint32 l)
{
  uint32 ret=0;
  ret = (l & 0xFF) << 24;
  ret |= ((l >> 8) & 0xFF) << 16;
  ret |= ((l >> 16) & 0xFF) << 8;
  ret |= ((l >> 24) & 0xFF);
  return ret;
}

/**
*@brief  ×Ö·û´®´¦Àí
*@param  srcÄ¿±ê×Ö·û´® s1 s2²Ù×÷×Ö·û´®
*@return ÎŞ
*/
void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
  int8* sub1;
  int8* sub2;
  uint16 n;

  sub1=strstr(src,s1);
  sub1+=strlen(s1);
  sub2=strstr(sub1,s2);
  n=sub2-sub1;
  strncpy(sub,sub1,n);
  sub[n]=0;
}

/**
*@brief  ipÍøÂçµØÖ·×ª»»
*@param  adr£ºµØÖ· ip£ºip
*@return ÎŞ
*/
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
  int i;
  char taddr[30];
  char * nexttok;
  char num;
  strcpy(taddr,(char *)addr);
  
  nexttok = taddr;
  for(i = 0; i < 4 ; i++)
  {
    nexttok = strtok(nexttok,".");
    if(nexttok[0] == '0' && nexttok[1] == 'x')
      num = atoi16(nexttok+2,0x10);
    else
      num = atoi16(nexttok,10);
  
  ip[i] = num;
  nexttok = NULL;
  }
}  
 
/**
*@brief  ½«32µØÖ·×ª»¯ÎªÊ®½øÖÆ¸ñÊ½
*@param  addr:  Òª×ª»¯µØÖ·
*@return ·µ»ØÊ®½øÖÆ¸ñÊ½µØÖ·
*/
char* inet_ntoa(unsigned long addr)
{
  static char addr_str[32];
  memset(addr_str,0,32);
  sprintf(addr_str,"%d.%d.%d.%d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
  return addr_str;
}

/**
*@brief  ½«16µØÖ·×ª»¯ÎªÊ®½øÖÆ¸ñÊ½
*@param  addr:  Òª×ª»¯µØÖ·
*@return ·µ»ØÊ®½øÖÆ¸ñÊ½µØÖ·
*/
char* inet_ntoa_pad(unsigned long addr)
{
  static char addr_str[16];
  memset(addr_str,0,16);
  printf(addr_str,"%03d.%03d.%03d.%03d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
  return addr_str;
}
 
/**
*@brief  ÑéÖ¤IPµØÖ·
*@param  ip addr
*@return ³É¹¦ - 1, Ê§°Ü - 0
*/
char verify_ip_address(char* src, uint8 * ip)
{
  int i;
  int tnum;
  char tsrc[50];
  char* tok = tsrc;
  
  strcpy(tsrc,src);
  
  for(i = 0; i < 4; i++)
  {
    tok = strtok(tok,".");
    if ( !tok )
      return 0;
    if(tok[0] == '0' && tok[1] == 'x')
    {
      if(!validatoi(tok+2,0x10,&tnum))
        return 0;
    }
    else if(!validatoi(tok,10,&tnum)) 
      return 0;

    ip[i] = tnum;
    
    if(tnum < 0 || tnum > 255)
      return 0;
    tok = NULL;
  }
  return 1;  
}

/**
*@brief  ½«Ò»¸ö Ö÷»úÄ£Ê½µÄunsigned shortĞÍÊı¾İ×ª»»µ½´ó¶ËÄ£Ê½µÄTCP/IP ÍøÂç×Ö½Ú¸ñÊ½µÄÊı¾İ.
*@param  Òª×ª»»µÄÊı¾İ
*@return ´ó¶ËÄ£Ê½µÄÊı¾İ
*/ 
uint16 htons( uint16 hostshort)
{
  /**< A 16-bit number in host byte order.  */
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
  return swaps(hostshort);
#else
  return hostshort;
#endif    
}

/**
*@brief  ½«Ò»¸ö Ö÷»úÄ£Ê½µÄunsigned longĞÍÊı¾İ×ª»»µ½´ó¶ËÄ£Ê½µÄTCP/IP ÍøÂç×Ö½Ú¸ñÊ½µÄÊı¾İ.
*@param  Òª×ª»»µÄÊı¾İ
*@return ´ó¶ËÄ£Ê½µÄÊı¾İ
*/ 
unsigned long htonl(unsigned long hostlong)
{
  /**< hostshort  - A 32-bit number in host byte order.  */
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
  return swapl(hostlong);
#else
  return hostlong;
#endif  
}

/**
*@brief  ½«Ò»¸ö´ó¶ËÄ£Ê½µÄTCP/IP ÍøÂç×Ö½Ú¸ñÊ½µÄÊı¾İ×ª»»µ½Ö÷»úÄ£Ê½µÄunsigned shortĞÍÊı¾İ
*@param  Òª×ª»»µÄÊı¾İ
*@return unsigned shortÄ£Ê½µÄÊı¾İ
*/ 
unsigned long ntohs(unsigned short netshort)
{
  /**< netshort - network odering 16bit value */
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )  
  return htons(netshort);
#else
  return netshort;
#endif    
}

/**
*@brief  ½«Ò»¸ö´ó¶ËÄ£Ê½µÄTCP/IP ÍøÂç×Ö½Ú¸ñÊ½µÄÊı¾İ×ª»»µ½Ö÷»úÄ£Ê½µÄunsigned longĞÍÊı¾İ
*@param  Òª×ª»»µÄÊı¾İ
*@return unsigned longÄ£Ê½µÄÊı¾İ
*/ 
unsigned long ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
  return htonl(netlong);
#else
  return netlong;
#endif    
}

/**
*@brief  ¼ÆËã×Ö·û´®Ğ£ÑéÖµ
*@param  Òª×ª»»µÄÊı¾İ
*@return Ğ£ÑéÖµ
*/ 
unsigned short checksum(unsigned char * src,unsigned int len)
{
  u_int sum, tsum, i, j;
  u_long lsum;

  j = len >> 1;

  lsum = 0;

  for (i = 0; i < j; i++) 
  {
    tsum = src[i * 2];
    tsum = tsum << 8;
    tsum += src[i * 2 + 1];
    lsum += tsum;
  }

  if (len % 2) 
  {
    tsum = src[i * 2];
    lsum += (tsum << 8);
  }

  sum = lsum;
  sum = ~(sum + (lsum >> 16));
  return (u_short) sum;  
}
