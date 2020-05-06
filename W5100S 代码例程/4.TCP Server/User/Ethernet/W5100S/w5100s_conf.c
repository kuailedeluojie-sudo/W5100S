/**
******************************************************************************
* @file         w5100s_conf.c                                                                                              
* @version      V1.0                          
* @date         2018-06-18             
* @brief        配置MCU，移植w5100s程序需要修改的文件，配置w5100s的MAC和IP地址   
*        
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100s官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com     
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/

#include <stdio.h> 
#include <string.h>
#include "w5100s_conf.h"
#include "utility.h"
#include "w5100s.h"
#include "socket.h"
#include "bsp_TiMbase.h"
#include "bsp_flash.h"
#include "dhcp.h"

CONFIG_MSG  ConfigMsg;                   // 配置结构体

uint16 rxlen=0;
uint8 rxbuf[1024];

uint8  txsize[MAX_SOCK_NUM] = {2,2,2,2}; // tx buffer set K bytes
uint8  rxsize[MAX_SOCK_NUM] = {2,2,2,2}; // rx buffet set K bytes
/*MAC地址首字节必须为偶数
  如果多块W5100s网络适配板在同一现场工作，请使用不同的MAC地址
*/
uint8 mac[6]={0x00,0x08,0xdc,0x22,0x33,0x33};

/*定义默认IP信息*/
uint8 local_ip[4] = {192,168,1,233};                        // 定义W5100s默认IP地址
uint8 subnet[4] = {255,255,255,0};                          // 定义W5100s默认子网掩码
uint8 gateway[4] = {192,168,1,1};                           // 定义W5100s默认网关
uint8 dns_server[4] = {114,114,114,114};                    // 定义W5100s默认DNS

uint16 local_port = 8080;                                   // 定义本地端口

/*定义远端IP信息*/
uint8  remote_ip[4] = {192,168,1,100};                      // 远端IP地址
uint16 remote_port = 8080;                                  // 远端端口号

/*IP配置方法选择，请自行选择*/
uint8  ip_from=IP_FROM_DEFINE;      

uint8  dhcp_ok=0;                                           // dhcp成功获取IP
uint32 ms=0;                                                // 毫秒计数
uint32 dhcp_time= 0;                                        // DHCP运行计数
vu8    ntptimer = 0;                                        // NPT秒计数
/**
*@brief  初始化W5100s的复位与中断管脚
*@param  无
*@return 无
*/
void reset_break_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  //  PD-08 -> RST 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //  PD-09 -> INT 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
*@brief  配置W5100s的IP地址
*@param  无
*@return 无
*/
void set_w5100s_netinfo(void)
{
  uint8 temp;
  // 复制定义的配置信息到配置结构体
  memcpy(ConfigMsg.mac, mac, 6);
  memcpy(ConfigMsg.lip,local_ip,4);
  memcpy(ConfigMsg.sub,subnet,4);
  memcpy(ConfigMsg.gw,gateway,4);
  memcpy(ConfigMsg.dns,dns_server,4);
  
  switch(ip_from)
  {
    case IP_FROM_DEFINE:
      printf(" 使用定义的IP信息配置W5100S:\r\n");
      break;
      
    case IP_FROM_DHCP:
      // 复制DHCP获取的配置信息到配置结构体
      if(dhcp_ok==1)
      {
        printf(" IP from DHCP\r\n");     
        memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
        memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
        memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
        memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
      }
      else
      {
        printf(" DHCP子程序未运行,或者不成功\r\n");
        printf(" 使用定义的IP信息配置W5100S\r\n");
      }
      break;
      
    case IP_FROM_FLASH:
      ReadFlashNBtye(0,&temp,1);
      if(temp != 255)
        ReadFlashNBtye(0,ConfigMsg.lip,12);
      break;
    }
  setSUBR(ConfigMsg.sub);
  setGAR(ConfigMsg.gw);
  setSIPR(ConfigMsg.lip);

  getSIPR (local_ip);      
  printf(" W5100S IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
  getSUBR(subnet);
  printf(" W5100S 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
  getGAR(gateway);
  printf(" W5100S 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief  配置W5100S的MAC地址
*@param  无
*@return 无
*/
void set_w5100s_mac(void)
{
  // 以下配置信息，根据需要选用  
  ConfigMsg.sw_ver[0]=FW_VER_HIGH;
  ConfigMsg.sw_ver[1]=FW_VER_LOW;  
  
  memcpy(ConfigMsg.mac, mac, 6);
  setSHAR(ConfigMsg.mac);
  memcpy(DHCP_GET.mac, mac, 6);
  getSHAR(mac);
  printf(" W5100S MAC地址  : %02x.%02x.%02x.%02x.%02x.%02x\r\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}
  

/**
*@brief  W5100s复位设置函数
*@param  无
*@return 无
*/
void reset_w5100s(void)
{
  GPIO_ResetBits(GPIOD, GPIO_Pin_8);
  delay_ms(500);  
  GPIO_SetBits(GPIOD, GPIO_Pin_8);
  delay_ms(1500);  

}

/**
*@brief  STM32系统软复位函数
*@param  无
*@return 无
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" 系统重启中……\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

/**
*@brief  dhcp用到的定时器初始化
*@param  无
*@return 无
*/
void dhcp_timer_init(void)
{
  timer2_init();                                  
}

/**
*@brief  ntp用到的定时器初始化
*@param  无
*@return 无
*/
void ntp_timer_init(void)
{
  timer2_init();                                  
}

/**
*@brief  STM32定时器2初始化
*@param  无
*@return 无
*/
void timer2_init(void)
{
  TIM2_Configuration();                                        //  TIM2 定时配置 
  TIM2_NVIC_Configuration();                                   //  定时器的中断优先级 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);        //  TIM2 重新开时钟，开始计时 
}

/**
*@brief  定时器2中断函数
*@param  无
*@return 无
*/
void timer2_isr(void)
{
  ms++;  
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;             // DHCP定时加1S
#ifndef __NTP_H__
    ntptimer++;              // NTP重试时间加1S
#endif
  }
}

/**
*@brief  检测物理层连接
*@param  无
*@return 无
*/
void PHY_check(void)
{
  uint8 PHY_connect=0;
  PHY_connect=0x01&getPHYStatus();
  if(PHY_connect==0)
  {
    printf(" \r\n 请检查网线是否连接?\r\n");
    PHY_connect = 0;
    do
    {
      close(PHY_connect++);
    }while(PHY_connect < 4);
    PHY_connect = 0;
    while(PHY_connect == 0) { 
      PHY_connect=0x01&getPHYStatus();  
      printf(" .");
      delay_ms(500);
    }
    printf(" \r\n");  
  }
}
/**
*@brief  读取PHY寄存器数据
*@param  无
*@return 无
*/
uint8 getPHYStatus( void )
{
  return IINCHIP_READ(PHYSR);
}
