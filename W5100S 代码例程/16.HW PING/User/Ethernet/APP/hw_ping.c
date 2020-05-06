/**
******************************************************************************
* @file         hw_ping.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        W5100S硬件主动PING功能           
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100S官方代理商，全程技术支持，价格优势大！
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

uint8 hw_ping_ip[4] = {192,168,1,100};     // PING的IP
uint8 exti_flag = 0;                       // 中断发生标志
void hw_ping_init(void)
{
	setSLRTR(0x03E8);       // 重传超时时间设置：1000 * 100us
	setSLRCR(3);            // 重传次数设置：3次
  /* 使用中断时可开启该配置 */	
	setSLIMR(0x05);         // 中断开启 PING & TIMEOUT
	
	setPINGSEQR(0x03E8);    // 请求数据包序列号设置：1000
	setPINGIDR(0x0100);     // 请求数据包ID：100
}

void hw_ping(void)
{
	setSLPIPR(hw_ping_ip);  // 目标IP设置：PING对象IP
  setSLCR(0x01);				  // 发送PING请求
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
