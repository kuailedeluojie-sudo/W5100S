/**
******************************************************************************
* @file         bsp_spi.c                                                                                          
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        配置与W5100S通信的SPI管脚与SPI配置
*        
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100s官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com         
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "bsp_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "types.h"
void spi_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);
  /* PB-13 -> SCL | PB-14 -> MISO | PB-15 -> MOSI */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* PD-07 -> CS */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void spiinitailize(void)
{
  SPI_InitTypeDef SPI_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI2, &SPI_InitStructure);

  SPI_Cmd(SPI2,ENABLE);
}

void cs_high(void)
{
  GPIO_SetBits(GPIOD,GPIO_Pin_7);
}

void cs_low(void)
{
  GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

void spi_send_byte(uint8 byte)
{
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI2, byte);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  SPI_I2S_ReceiveData(SPI2);
}

uint8 spi_read_byte(void)
{
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI2, 0);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI2);
}
