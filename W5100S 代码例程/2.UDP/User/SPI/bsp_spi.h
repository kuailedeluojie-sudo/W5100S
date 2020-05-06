/**
******************************************************************************
* @file         bsp_spi.h                                                                                           
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        bsp_spi.c��ͷ�ļ�
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100s�ٷ������̣�ȫ�̼���֧�֣��۸�۸����ƴ�
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
