/**
******************************************************************************
* @file         main.c                                              
* @version      V1.0                          
* @date         2018-06-18                               
* @brief        �������ԣ��뱣֤W5100S��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ         
*               ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
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

#include "BD_TG.h"
#include "MqttKit.h"

/** W5100S EVB �ܽŽ�����Ϣ
******************************************************************************
* W5100S_RST -> PD_08 // ��λ
* W5100S_INT -> PD_09 // �ж�
* W5100S_SCS -> PD_07 // Ƭѡ
* W5100S_CLK -> PB_13 // ʱ��
#if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_) // SPIģʽ
* W5100S_MOSI(A0) -> PB_15 // ����������ӻ�����
* W5100S_MISO(A1) -> PB_14 // �������룬�ӻ����
* W5100S_M2  -> Low   // ͨѶ��ʽΪ��SPI
#elif (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_) // 8λ�������ģʽ
#if STM32F103ZET6     // ���ش��е�ַ����
* W5100S_MOSI(A0) -> PF_00 // ������ߵ�ַλ0
* W5100S_MISO(A1) -> PF_01 // ������ߵ�ַλ1
#endif
* W5100S_M2  -> High  // ͨѶ��ʽΪ��8λ�������
* W5100S_RDN -> PD_04 // ������߶�ȡʱ��
* W5100S_WRN -> PD_05 // ������߷���ʱ��
* W5100S_D0  -> PD_14 // �����������λ0
* W5100S_D1  -> PD_15 // �����������λ1
* W5100S_D2  -> PD_00 // �����������λ2
* W5100S_D3  -> PD_01 // �����������λ3
* W5100S_D4  -> PE_07 // �����������λ4
* W5100S_D5  -> PE_08 // �����������λ5
* W5100S_D6  -> PE_09 // �����������λ6
* W5100S_D7  -> PE_10 // �����������λ7
#endif
******************************************************************************
*/

#define User0 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define User1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
}

int main(void)
{
  systick_init(72);                     // ��ʼ���δ�ʱ��
  USART1_Config();                      // ��ʼ��USART1��115200@8-N-1 
	timer2_init();	                      // ��ʼ����ʱ��
	key_init();														// ������ʼ��
  reset_break_gpio_init();              // ��λ���жϹܽų�ʼ��
#if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_)
  spi_gpio_init();                      // SPI�ܽų�ʼ��
  spiinitailize();                      // SPI���ó�ʼ��
  reg_wizchip_spi_cbfunc(spi_read_byte,spi_send_byte);          // SPI��д���ݺ���ӳ��
  reg_wizchip_cs_cbfunc(cs_low,cs_high);                        // SPIƬѡ���ƺ���ӳ��
#elif (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)
  FSMC_gpio_init();                     // FSMC�ܽų�ʼ��c
  FSMCInitialize();                     // FSMC���ó�ʼ��
#endif
  printf("\r\n ����Ƽ�--WIZnet W5100S�ٷ������̡�ȫ�̼���֧�֣��۸����ƴ�\r\n\r\n");
  reset_w5100s();                       // W5100SӲ����λ 
  PHY_check();                          // ���߼�����
  set_w5100s_mac();                     // ����W5100SMAC��ַ
  set_w5100s_netinfo();                 // ����W5100S�������
  wizchip_init(txsize,rxsize);          // ��ʼ��4��Socket�ķ��ͽ��ջ����С

  while(1)                               
  {
    do_tcp_client();                    // TCP �ͻ������ݻػ����� 
		
		delay_ms(100);

		if(User1 == 0)                      //����
		{
			while(User1 == 0);
			MQTT_STATE = MQTT_PKT_SUBSCRIBE;
		}
//		else if(KEY_5 == 0)                //ȡ������
//		{
//			MQTT_STATE = MQTT_PKT_UNSUBSCRIBE;
//		}
		else if(User0 == 0)                 //����
		{
			while(User0 == 0);
			MQTT_STATE =MQTT_PKT_PUBLISH;
		}	
	}
}
