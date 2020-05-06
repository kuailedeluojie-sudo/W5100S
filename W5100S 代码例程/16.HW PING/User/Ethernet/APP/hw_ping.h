/**
******************************************************************************
* @file         hw_ping.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        hw_ping.c��ͷ�ļ�        
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#ifndef _HW_PING_H_
#define _HW_PING_H_

#include "stm32f10x.h"
#include "w5100s.h"
#include "bsp_usart1.h"
#include "types.h"

extern uint8 hw_ping_ip[4];
extern uint8 exti_flag;

void hw_ping_init(void);                   // Ӳ��PING��ʼ������
void hw_ping(void);                        // ����PING����

void EXTI_GPIO_Init(void);                 // PING�жϴ��� �ⲿ�ж�����

#endif
