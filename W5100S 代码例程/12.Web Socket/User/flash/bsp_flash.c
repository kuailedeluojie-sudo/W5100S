/**
******************************************************************************
* @file         bsp_flash.c                                                                                              
* @version      V1.0                          
* @date         2018-06-18                       
* @brief        配置FLASH读写
*        
* @company      深圳炜世科技有限公司
* @information  WIZnet W5100s官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com   
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "bsp_flash.h"  
#include <string.h>  
#include "stm32f10x_flash.h"  
  
#define STARTADDR 0x0807F800

volatile FLASH_Status FLASHStatus = FLASH_BUSY; //Flash操作状态变量

//////////////////////////////////////////////////////////////////////////////////
// Name:        WriteFlashData
//
// Function:    向内部Flash写入数据
//
// Input:       WriteAddress：数据要写入的目标地址（偏移地址）
//              data[]：      写入的数据首地址
//              num：         写入数据的个数
//////////////////////////////////////////////////////////////////////////////////
void WriteFlashData(uint32_t WriteAddress, uint8_t data[], uint32_t num)
{
  uint32_t i = 0;
  uint16_t temp = 0;

  FLASH_UnlockBank1();                     //解锁flash
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 

  FLASHStatus = FLASH_BUSY;                //清空状态指示标志位
  FLASHStatus = FLASH_ErasePage(STARTADDR);//擦除整页
  if(FLASHStatus == FLASH_COMPLETE)        //flash操作完成
  {
    FLASHStatus = FLASH_BUSY;              //清空状态指示标志位
    for(i=0; i<num; i++)
    {
      temp = (uint16_t)data[i];
      FLASHStatus = FLASH_ProgramHalfWord(STARTADDR+WriteAddress+i*2, temp);//写入数据
    }
  }
  FLASHStatus = FLASH_BUSY;                //清空状态指示标志位

  FLASH_LockBank1();                       //锁定flash
} 

//////////////////////////////////////////////////////////////////////////////////
// Name:        ReadFlashNBtye
//
// Function:    从内部Flash读取N字节数据
//
// Input:       ReadAddress：数据地址（偏移地址）
//              ReadBuf：读取到的数据存放位置指针
//              ReadNum：读取字节个数
//
// Output:      读取的字节数
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

