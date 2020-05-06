/**
******************************************************************************
* @file         bsp_flash.h                                                                                           
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        bsp_flash.c��ͷ�ļ�
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100s�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com   
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef _BSP_FLASH_H  
#define _BSP_FLASH_H
  
#include "types.h"  
#include "stm32f10x_flash.h"
  
void WriteFlashData(uint32_t WriteAddress, uint8_t data[], uint32_t num);

int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum);

#endif /* _MCU_FLASH_H_ */  
