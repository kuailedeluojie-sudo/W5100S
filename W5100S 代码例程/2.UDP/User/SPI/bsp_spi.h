/**
******************************************************************************
* @file         bsp_spi.h                                                                                           
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        bsp_spi.c的头文件
*        
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100s官方代理商，全程技术支持，价格价格优势大！
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#include "types.h"

void spi_gpio_init(void);
void spiinitailize(void);
void cs_high(void);
void cs_low(void);
void spi_send_byte(uint8 byte);
uint8 spi_read_byte(void);
#endif
