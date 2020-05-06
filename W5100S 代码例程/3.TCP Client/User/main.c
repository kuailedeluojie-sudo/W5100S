/**
******************************************************************************
* @file         main.c                                              
* @version      V1.0                          
* @date         2018-06-18                               
* @brief        内网测试，请保证W5100S的IP与测试PC机在同一网段内，且不冲突         
*               如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
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
#include "stm32f10x.h"

#include "bsp_usart1.h"
#include "bsp_fsmc.h"
#include "bsp_spi.h"

#include "w5100s.h"
#include "W5100s_conf.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

#include "utility.h"

#include "tcp_client.h"

/** W5100S EVB 管脚接线信息
******************************************************************************
* W5100S_RST -> PD_08 // 复位
* W5100S_INT -> PD_09 // 中断
* W5100S_SCS -> PD_07 // 片选
* W5100S_CLK -> PB_13 // 时钟
#if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_) // SPI模式
* W5100S_MOSI(A0) -> PB_15 // 主机输出，从机输入
* W5100S_MISO(A1) -> PB_14 // 主机输入，从机输出
* W5100S_M2  -> Low   // 通讯方式为：SPI
#elif (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) // 8位间接总线模式
#if STM32F103ZET6     // 主控带有地址总线
* W5100S_MOSI(A0) -> PF_00 // 间接总线地址位0
* W5100S_MISO(A1) -> PF_01 // 间接总线地址位1
#endif
* W5100S_M2  -> High  // 通讯方式为：8位间接总线
* W5100S_RDN -> PD_04 // 间接总线读取时序
* W5100S_WRN -> PD_05 // 间接总线发送时序
* W5100S_D0  -> PD_14 // 间接总线数据位0
* W5100S_D1  -> PD_15 // 间接总线数据位1
* W5100S_D2  -> PD_00 // 间接总线数据位2
* W5100S_D3  -> PD_01 // 间接总线数据位3
* W5100S_D4  -> PE_07 // 间接总线数据位4
* W5100S_D5  -> PE_08 // 间接总线数据位5
* W5100S_D6  -> PE_09 // 间接总线数据位6
* W5100S_D7  -> PE_10 // 间接总线数据位7
#endif
******************************************************************************
*/

int main(void)
{
  systick_init(72);                     // 初始化滴答定时器
  USART1_Config();                      // 初始化USART1：115200@8-N-1 
  reset_break_gpio_init();              // 复位与中断管脚初始化
#if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_)
  spi_gpio_init();                      // SPI管脚初始化
  spiinitailize();                      // SPI配置初始化
  reg_wizchip_spi_cbfunc(spi_read_byte,spi_send_byte);          // SPI读写数据函数映射
  reg_wizchip_cs_cbfunc(cs_low,cs_high);                        // SPI片选控制函数映射
#elif (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)
  FSMC_gpio_init();                     // FSMC管脚初始化c
  FSMCInitialize();                     // FSMC配置初始化
#endif
  printf("\r\n 炜世科技--WIZnet W5100S官方代理商。全程技术支持，价格优势大！\r\n\r\n");
  reset_w5100s();                       // W5100S硬件复位 
  PHY_check();                          // 网线检测程序
  set_w5100s_mac();                     // 设置W5100SMAC地址
  set_w5100s_netinfo();                 // 设置W5100S网络参数
  wizchip_init(txsize,rxsize);          // 初始化4个Socket的发送接收缓存大小
  printf("\r\n 电脑作为TCP服务器,等待W5100S作为 TCP客户端 的连接。 \r\n");
  printf(" 服务器IP:%d.%d.%d.%d。\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
  printf(" 监听端口:%d。\r\n",remote_port);
  printf(" 正常现象：连接成功，服务器发送数据给W5100S，W5100S将返回对应数据。\r\n");
  while(1)                               
  {
    do_tcp_client();                    // TCP 客户端数据回环测试 
  }
}
