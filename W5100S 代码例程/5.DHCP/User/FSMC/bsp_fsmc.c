/**
******************************************************************************
* @file         bsp_fsmc.c                                                                                              
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        ������W5100Sͨ�ŵ����߹ܽ�����������
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100s�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com   
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "bsp_fsmc.h"
#include "stm32f10x_gpio.h"

void FSMC_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  /* PD-14 -> D0 |  PD-15 -> D1 | PD-00 -> D2 | PD-01 -> D3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /* PE-07 -> D4 |  PE-08 -> D5 | PE-09 -> D6 | PE-10 -> D7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  /* PD-04 -> FSMC_NOE | PD-05 -> FSMC_NWE */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /* PD-07 -> FSMC_NE1 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOD,GPIO_Pin_7,Bit_RESET);
#ifdef  STM32F103VET6
  /* PB-07 -> FSMC_NADV */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef  STM32F103ZET6
  /* PF-00 -> A0 |  PF-01 -> A1 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;                              
  GPIO_Init(GPIOF, &GPIO_InitStructure);
#endif
}

void FSMCInitialize()
{
  FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef p;  
#ifdef  STM32F103VET6
  p.FSMC_AddressSetupTime = 0x03;         // ��ַ�Ľ���ʱ��
  p.FSMC_AddressHoldTime = 0x01;          // ��ַ�ı���ʱ��
  p.FSMC_DataSetupTime = 0x08;            // ���ݵı���ʱ��
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0x00;              //CLKʱ������źŵ�����
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_B;  //����ģʽ
  
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                                 // ʹ��NE1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;                // ����/��ַ����
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;                           // �洢������
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;                  // �������߿��
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;           // �ȴ��ź���Ч����
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;                                 //��ʱ������ָ��
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                                     //дʱ������ָ��
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
#endif
#ifdef  STM32F103ZET6
  p.FSMC_AddressSetupTime = 0x00;         // ��ַ�Ľ���ʱ��
  p.FSMC_AddressHoldTime =0;              // ��ַ�ı���ʱ��
  p.FSMC_DataSetupTime = 0x03;            // ���ݵı���ʱ��
  p.FSMC_BusTurnAroundDuration = 0x00; 
  p.FSMC_CLKDivision = 0;                 //CLKʱ������źŵ�����
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;  //����ģʽ
      
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                                  // ʹ��NE1
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;                // ����/��ַ������
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;                           // �洢������
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;                   // �������߿��
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;            // �ȴ��ź���Ч����
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;                
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;                                   //��ʱ������ָ��
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                                       //дʱ������ָ��
#endif
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
