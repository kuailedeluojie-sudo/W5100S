/**
******************************************************************************
* @file         bsp_flash.c                                                                                              
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        ����FLASH��д
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100s�ٷ������̣�ȫ�̼���֧�֣��۸����ƴ�
* @website      www.wisioe.com   
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "bsp_flash.h"  
#include <string.h>  
#include "stm32f10x_flash.h"  
  
#define STARTADDR 0x0807F800

volatile FLASH_Status FLASHStatus = FLASH_BUSY; //Flash����״̬����

//////////////////////////////////////////////////////////////////////////////////
// Name:        WriteFlashData
//
// Function:    ���ڲ�Flashд������
//
// Input:       WriteAddress������Ҫд���Ŀ���ַ��ƫ�Ƶ�ַ��
//              data[]��      д��������׵�ַ
//              num��         д�����ݵĸ���
//////////////////////////////////////////////////////////////////////////////////
void WriteFlashData(uint32_t WriteAddress, uint8_t data[], uint32_t num)
{
  uint32_t i = 0;
  uint16_t temp = 0;

  FLASH_UnlockBank1();                     //����flash
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 

  FLASHStatus = FLASH_BUSY;                //���״ָ̬ʾ��־λ
  FLASHStatus = FLASH_ErasePage(STARTADDR);//������ҳ
  if(FLASHStatus == FLASH_COMPLETE)        //flash�������
  {
    FLASHStatus = FLASH_BUSY;              //���״ָ̬ʾ��־λ
    for(i=0; i<num; i++)
    {
      temp = (uint16_t)data[i];
      FLASHStatus = FLASH_ProgramHalfWord(STARTADDR+WriteAddress+i*2, temp);//д������
    }
  }
  FLASHStatus = FLASH_BUSY;                //���״ָ̬ʾ��־λ

  FLASH_LockBank1();                       //����flash
} 

//////////////////////////////////////////////////////////////////////////////////
// Name:        ReadFlashNBtye
//
// Function:    ���ڲ�Flash��ȡN�ֽ�����
//
// Input:       ReadAddress�����ݵ�ַ��ƫ�Ƶ�ַ��
//              ReadBuf����ȡ�������ݴ��λ��ָ��
//              ReadNum����ȡ�ֽڸ���
//
// Output:      ��ȡ���ֽ���
//////////////////////////////////////////////////////////////////////////////////
int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum)
{   
  int DataNum = 0;

  ReadAddress = (uint32_t)STARTADDR + ReadAddress;  
  while(DataNum < ReadNum)   
  {      
    *(ReadBuf + DataNum) = *(__IO uint16_t*) ReadAddress++;
    DataNum++;
    ReadAddress++;
  }

  return DataNum;    
}

