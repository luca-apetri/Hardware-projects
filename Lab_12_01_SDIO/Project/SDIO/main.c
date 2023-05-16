/**
  ******************************************************************************
  * @file    USART/USART_Printf/main.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sdcard.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

  
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


unsigned char Start_Flag=0;

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks) >> 2)

#define Operate_Block 0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_CardInfo SDCardInfo;
u32 Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
u32 Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;
ErrorStatus HSEStartUpStatus;
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file.
     */     

  RCC_ClocksTypeDef SYS_Clocks;
  /* USART configuration */
  USART_Config();

  RCC_GetClocksFreq(&SYS_Clocks);
	
	
// pag. 169, În int main() introduce?i urmatoarea sec?iune de cod (înainte de bucla while (1)):
u32 i;
NVIC_Configuration();
printf("\n\rSDIO (Secure Digital Input/Output) - Testarea modulului microSD card");
printf("\r\nAtentie: Programul de test va modifica datele memorate in cardul microSD. Pentru a continua tastati 'y'!\r\n");
while(USART_ReceiveData(EVAL_COM1)!= 'y');
Status = SD_Init();
printf("\r\n01. SD_Init Status:%d.",Status);
if (Status == SD_OK)
{
printf(" Initializarea cardului SD a fost realizata cu succes!\r\n");
Status = SD_GetCardInfo(&SDCardInfo); //Cite?te registrii CSD/CID MSD
printf("\r\n02. SD_GetCardInfo Status:%d.",Status);
}
if (Status == SD_OK)
{
printf(" Informatii card SD: Block size:%x, Card type:%x\r\n",SDCardInfo.CardBlockSize,SDCardInfo.CardType);
Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16)); //Selec?ia cardului
printf("\r\n03. SD_SelectDeselect Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Selectie card realizata cu succes!\r\n");
Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
printf("\r\n04. SD_EnableWideBusOperation Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Operatia de activare a magistralei realizata cu succes!\r\n");
//Sterge NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes)
Status = SD_Erase(Operate_Block*BlockSize, (Operate_Block+1)*BlockSize); //Stergere bloc
printf("\r\n05. SD_Erase Status:%d. ",Status);
}	

// pag. 170
if (Status == SD_OK)
{
printf("Stergerea blocului %d s-a realizat cu succes!\r\n",Operate_Block);
Status = SD_SetDeviceMode(SD_DMA_MODE); //Setarea modului de transfer DMA
printf("\r\n06. SD_SetDeviceMode Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Modul de lucru de tip DMA configurat!\r\n");
memset(Buffer_MultiBlock_Rx,0xfe,sizeof(Buffer_MultiBlock_Rx));
Status = SD_ReadMultiBlocks(Operate_Block*BlockSize, Buffer_MultiBlock_Rx, BlockSize, NumberOfBlocks);
printf("\r\n07. SD_ReadMultiBlocks Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf(" Citire cu succes a 2 blocuri din blocul %d! Datele citite sunt:\r\n",Operate_Block);
for(i=0;i<sizeof(Buffer_MultiBlock_Rx)>>2;i++)
{
printf("%02x:0x%08x ",i,Buffer_MultiBlock_Rx[i]);
}
printf("...\r\n");
}	

//Citire/scriere bloc
memset(Buffer_Block_Tx, 0x88,sizeof(Buffer_Block_Tx));//scrie date in buffer
if (Status == SD_OK)
{
//Scrie un bloc de 512 bytes la adresa 0
Status = SD_WriteBlock(Operate_Block*BlockSize, Buffer_Block_Tx, BlockSize);
printf("\r\n08. SD_WriteBlock Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Scrierea blocului %d s-a realizat cu succes!\r\n",Operate_Block);
// Citire bloc de 512 bytes de la adresa 0
Status = SD_ReadBlock(Operate_Block*BlockSize, Buffer_Block_Rx, BlockSize);
printf("\r\n09. SD_ReadBlock Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Citirea blocului %d s-a realizat cu succes! Datele citite din memoria microSD sunt:\r\n",Operate_Block);
for(i=0;i<sizeof(Buffer_Block_Rx)>>2;i++)
{
printf("%02x:0x%08x ",i,Buffer_Block_Rx[i]);
}
printf("...\r\n");
}
memset(Buffer_MultiBlock_Tx, 0x66, sizeof(Buffer_MultiBlock_Tx));//Citire/scriere blocuri multiple
if (Status == SD_OK)
{ // Scrie blocuri multiple la adresa 0

//pag. 171
Status = SD_WriteMultiBlocks((Operate_Block+2)*BlockSize, Buffer_MultiBlock_Tx, BlockSize, NumberOfBlocks);
printf("\r\n10. SD_WriteMultiBlocks Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Scrierea multibloc %d realizata cu succes!\r\n",Operate_Block+2);
//Citirea blocurilor de la adresa 0
Status = SD_ReadMultiBlocks((Operate_Block+2)*BlockSize, Buffer_MultiBlock_Rx, BlockSize, NumberOfBlocks);
printf("\r\n11. SD_ReadMultiBlocks Status:%d. ",Status);
}
if (Status == SD_OK)
{
printf("Citirea multibloc %d realizata cu succes! Datele citite sunt:\r\n",Operate_Block+2);
for(i=0;i<sizeof(Buffer_MultiBlock_Rx)>>2;i++)
{
printf("%02x:0x%08x ",i,Buffer_MultiBlock_Rx[i]);
}
printf("...\r\n\r\n");
printf("Testarea SDIO realizata cu succes!\n");
}
//end of pag. 171, Radu Fechet
	






//
  while (1)
  {
  }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
