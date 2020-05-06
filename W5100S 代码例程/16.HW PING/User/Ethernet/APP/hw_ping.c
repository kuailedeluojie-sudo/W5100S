/**
******************************************************************************
* @file         hw_ping.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        W5100SӲ������PING����           
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/

#include "hw_ping.h"
#include "stm32f10x.h"
#include "w5100s.h"
#include "bsp_usart1.h"
#include "types.h"

uint8 hw_ping_ip[4] = {192,168,1,100};     // PING��IP
uint8 exti_flag = 0;                       // �жϷ�����־
void hw_ping_init(void)
{
	setSLRTR(0x03E8);       // �ش���ʱʱ�����ã�1000 * 100us
	setSLRCR(3);            // �ش��������ã�3��
  /* ʹ���ж�ʱ�ɿ��������� */	
	setSLIMR(0x05);         // �жϿ��� PING & TIMEOUT
	
	setPINGSEQR(0x03E8);    // �������ݰ����к����ã�1000
	setPINGIDR(0x0100);     // �������ݰ�ID��100
}

void hw_ping(void)
{
	setSLPIPR(hw_ping_ip);  // Ŀ��IP���ã�PING����IP
  setSLCR(0x01);				  // ����PING����
}

void EXTI_GPIO_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);

	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure);  	 
}

void EXTI9_5_IRQHandler(void)			
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)	
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
		exti_flag = 1;
	}
}
