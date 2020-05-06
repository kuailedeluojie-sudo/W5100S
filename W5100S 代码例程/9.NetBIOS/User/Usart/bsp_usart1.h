/**
******************************************************************************
* @file         bsp_usart1.h                                                                                            
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        bsp_usart1.c的头文件
*        
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100s官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com  
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __USART1_H */
