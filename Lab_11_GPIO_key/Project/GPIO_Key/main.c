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
#include "KEY&LED.h"
#include "usart.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

#include <stdlib.h>
#define USER_KEY_Port GPIOA
#define USER_KEY_Pin GPIO_Pin_0
#define USER_KEY_RCC_AHBPeriph RCC_AHB1Periph_GPIOA
CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
CanTxMsg TxMessage;
//uint8_t KeyNumber = 0x0;
uint16_t CAN1_ID;
uint8_t CAN1_DATA0, CAN1_DATA1, CAN1_DATA2, CAN1_DATA3, CAN1_DATA4, CAN1_DATA5,
CAN1_DATA6, CAN1_DATA7;
uint16_t CAN2_ID;
uint8_t CAN2_DATA0, CAN2_DATA1, CAN2_DATA2, CAN2_DATA3, CAN2_DATA4, CAN2_DATA5,
CAN2_DATA6, CAN2_DATA7;
__IO uint8_t Can1Flag,Can2Flag;
void CAN1_Config(void);
void CAN2_Config(void);
void NVIC_Config(void);
void Can1WriteData(uint16_t ID);
void Can2WriteData(uint16_t ID);
void Key_Config(void);

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



 void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
RCC_ClocksTypeDef SYS_Clocks;
USART_Config();
RCC_GetClocksFreq(&SYS_Clocks);
//JOYState_LED_GPIO_Init();
//uint8_t error_can=0;
Key_Config();
printf("\n\r Exemplu de test pentru comunica?ia CAN.\r\n");
CAN1_Config();
CAN2_Config();
NVIC_Config();
while (1)
{
//Led_Toggle(Read_JOYState());
Delay(0xffff);
if(GPIO_ReadInputDataBit(USER_KEY_Port,USER_KEY_Pin) == ENABLE)
{
Can1WriteData(0x123);
printf("****************\r\nCAN1 transmite date la CAN2!\r\n");
Delay(0xffff);
if(Can2Flag==ENABLE)
{
Can2Flag = DISABLE;
printf("...\r\nCAN2 a recep?ionat corect urmatoarele date: \r\n");
printf("CAN2 ID %x \r\n",CAN2_ID);
printf("CAN2_DATA0 %x \r\n",CAN2_DATA0);
printf("CAN2_DATA1 %x \r\n",CAN2_DATA1);
printf("CAN2_DATA2 %x \r\n",CAN2_DATA2);
printf("CAN2_DATA3 %x \r\n",CAN2_DATA3);
printf("CAN2_DATA4 %x \r\n",CAN2_DATA4);
printf("CAN2_DATA5 %x \r\n",CAN2_DATA5);
printf("CAN2_DATA6 %x \r\n",CAN2_DATA6);
printf("CAN2_DATA7 %x \r\n",CAN2_DATA7);
if(CAN2_ID!=0x123 || CAN2_DATA0!=CAN1_DATA0 || CAN2_DATA1!=CAN1_DATA1
|| CAN2_DATA2!=CAN1_DATA2 || CAN2_DATA3!=CAN1_DATA3 ||
CAN2_DATA4!=CAN1_DATA4 ||
CAN2_DATA5!=CAN1_DATA5 || CAN2_DATA6!=CAN1_DATA6 ||
CAN2_DATA7!=CAN1_DATA7 )
{
printf("Comunica?ie CAN1 - CAN2 eronata!\r\n");//error_can=1;
}
}
Delay(0xffffff);
Can2WriteData(0x321);
printf("\r\nCAN2 transmite date la CAN1!\r\n");
Delay(0xffff);
if(Can1Flag==ENABLE)
{
Can1Flag = DISABLE;
	printf("...\r\nCAN1 a receptionat corect urmatoarele date:\r\n");
printf("CAN1 ID %x \r\n",CAN1_ID);
printf("CAN1_DATA0 %x \r\n",CAN1_DATA0);
printf("CAN1_DATA1 %x \r\n",CAN1_DATA1);
printf("CAN1_DATA2 %x \r\n",CAN1_DATA2);
printf("CAN1_DATA3 %x \r\n",CAN1_DATA3);
printf("CAN1_DATA4 %x \r\n",CAN1_DATA4);
printf("CAN1_DATA5 %x \r\n",CAN1_DATA5);
printf("CAN1_DATA6 %x \r\n",CAN1_DATA6);
printf("CAN1_DATA7 %x \r\n",CAN1_DATA7);
if(CAN1_ID!=0x321 || CAN1_DATA0!=CAN2_DATA0 || CAN1_DATA1!=CAN2_DATA1
|| CAN1_DATA2!=CAN2_DATA2 || CAN1_DATA3!=CAN2_DATA3 ||
CAN1_DATA4!=CAN2_DATA4 || CAN1_DATA5!=CAN2_DATA5 ||
CAN1_DATA6!=CAN2_DATA6 || CAN1_DATA7!=CAN2_DATA7 )
{
printf("Comunicatie CAN2 - CAN1 eronata!\r\n");//error_can=1;
}
}
}
}
}
void Key_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(USER_KEY_RCC_AHBPeriph, ENABLE);
GPIO_InitStructure.GPIO_Pin = USER_KEY_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
GPIO_Init(USER_KEY_Port, &GPIO_InitStructure);
}
void CAN1_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
// Activeaza ceasul
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB, ENABLE);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); //Configureaza GPIO pentru CAN
GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); //Conecteaza pinii 8 si 9
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //Configureaza pinul CAN1 RX
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //Func?ie auxiliara
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOB, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //Configureaza pinul CAN1 TX
GPIO_Init(GPIOB, &GPIO_InitStructure);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); //Activeaza ceasul pentru CAN1
CAN_DeInit(CAN1);
CAN_StructInit(&CAN_InitStructure); //Ini?ializeaza regi?trii CAN
CAN_InitStructure.CAN_TTCM = DISABLE; //Dezactivam triggered communication mode
	CAN_InitStructure.CAN_ABOM = DISABLE; //Dezactivam automatic wake-up mode
CAN_InitStructure.CAN_AWUM = DISABLE; //Dezactivam non-automatic retransmission mode
CAN_InitStructure.CAN_NART = DISABLE; //Dezactivam non-automatic retransmission mode
CAN_InitStructure.CAN_RFLM = DISABLE; //Dezactivam Receive FIFO Locked mode
CAN_InitStructure.CAN_TXFP = DISABLE; //Dezactivam transmit FIFO priority
CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq; //CAN Baudrate = 1MBps (CAN clocked at 30 MHz)
CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
CAN_InitStructure.CAN_Prescaler = 2;
CAN_Init(CAN1, &CAN_InitStructure);
CAN_FilterInitStructure.CAN_FilterNumber = 0; //Ini?ializarea filtrelor CAN
CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
CAN_FilterInit(&CAN_FilterInitStructure);
TxMessage.StdId = 0x321; //Structura de transmisie, specifica identificatorul standard
TxMessage.ExtId = 0x01; //Identificator extins
TxMessage.RTR = CAN_RTR_DATA; //Tipul cadrului pentru mesajul trimis
TxMessage.IDE = CAN_ID_STD; //Identificatorul pentru mesaj
TxMessage.DLC = 1; //Lungimea cadrului transmis
CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //Activeaza întreruperea FIFO 0
}

void CAN2_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);//Configureaza GPIO pentru CAN2
GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //Conecteaza pinii 5 si 6
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //Func?ie auxiliara
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOB, &GPIO_InitStructure);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); //Activeaza ceasul pentru CAN2
CAN_DeInit(CAN2);
CAN_StructInit(&CAN_InitStructure); //Ini?ializarea regi?trilor CAN
CAN_InitStructure.CAN_TTCM = DISABLE; //Dezactivam triggered communication mode
CAN_InitStructure.CAN_ABOM = DISABLE; //Dezactivam automatic bus-off management
CAN_InitStructure.CAN_AWUM = DISABLE; //Dezactivam automatic wake-up mode
	
	CAN_InitStructure.CAN_NART = DISABLE; //Dezactivam non-automatic retransmission mode
CAN_InitStructure.CAN_RFLM = DISABLE; //Dezactivam Receive FIFO Locked mode
CAN_InitStructure.CAN_TXFP = DISABLE; //Dezactivam transmit FIFO priority
CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //Modul de operare CAN
CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq; //CAN Baudrate = 1MBps (CAN clocked at 30 MHz)
CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
CAN_InitStructure.CAN_Prescaler = 2;
CAN_Init(CAN2, &CAN_InitStructure);
CAN_FilterInitStructure.CAN_FilterNumber = 14;
CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
CAN_FilterInit(&CAN_FilterInitStructure);

TxMessage.StdId = 0x123; //specifica identificatorul standard
TxMessage.ExtId = 0x01; //Identificatorul extins
TxMessage.RTR = CAN_RTR_DATA; //Tipul cadrului pentru mesajul trimis
TxMessage.IDE = CAN_ID_STD; //Identificatorul pentru mesaj
TxMessage.DLC = 1; //Lungimea cadrului transmis
CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE); //Activam întreruperea FIFO 0 message pending
}

void NVIC_Config(void) //Configureaza întreruperile pentru CAN1 si CAN2
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); //Configureaza NVIC pentru CAN1
NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); //Configureaza NVIC pentru CAN2
}

void Can1WriteData(uint16_t ID)
{
CanTxMsg TxMessage;
CAN1_DATA0=rand()%0xff; CAN1_DATA1=rand()%0xff;
CAN1_DATA2=rand()%0xff; CAN1_DATA3=rand()%0xff;
CAN1_DATA4=rand()%0xff; CAN1_DATA5=rand()%0xff;
CAN1_DATA6=rand()%0xff; CAN1_DATA7=rand()%0xff;
TxMessage.StdId = ID; //Transmisie date pe magistrala CAN
	//TxMessage.ExtId = 0x00;
TxMessage.RTR = CAN_RTR_DATA; //Tipul cadrului pentru mesajul trimis
TxMessage.IDE = CAN_ID_STD; //Identificatorul pentru mesaj
TxMessage.DLC = 8; //Lungimea cadrului transmis
TxMessage.Data[0] = CAN1_DATA0;
TxMessage.Data[1] = CAN1_DATA1;
TxMessage.Data[2] = CAN1_DATA2;
TxMessage.Data[3] = CAN1_DATA3;
TxMessage.Data[4] = CAN1_DATA4;
TxMessage.Data[5] = CAN1_DATA5;
TxMessage.Data[6] = CAN1_DATA6;
TxMessage.Data[7] = CAN1_DATA7;
CAN_Transmit(CAN1,&TxMessage); //Ini?iaza transmisia unui cadru prin CAN1
}

void Can2WriteData(uint16_t ID)
{
CanTxMsg TxMessage;
CAN2_DATA0=rand()%0xff; CAN2_DATA1=rand()%0xff;
CAN2_DATA2=rand()%0xff; CAN2_DATA3=rand()%0xff;
CAN2_DATA4=rand()%0xff; CAN2_DATA5=rand()%0xff;
CAN2_DATA6=rand()%0xff; CAN2_DATA7=rand()%0xff;
TxMessage.StdId = ID; //Transmisie date pe magistrala CAN
//TxMessage.ExtId = 0x00;
TxMessage.RTR = CAN_RTR_DATA; //Tipul cadrului pentru mesajul trimis
TxMessage.IDE = CAN_ID_STD; //Identificatorul pentru mesaj
TxMessage.DLC = 8; //Lungimea cadrului transmis
TxMessage.Data[0] = CAN2_DATA0;
TxMessage.Data[1] = CAN2_DATA1;
TxMessage.Data[2] = CAN2_DATA2;
TxMessage.Data[3] = CAN2_DATA3;
TxMessage.Data[4] = CAN2_DATA4;
TxMessage.Data[5] = CAN2_DATA5;
TxMessage.Data[6] = CAN2_DATA6;
TxMessage.Data[7] = CAN2_DATA7;
CAN_Transmit(CAN2,&TxMessage); //Ini?iaza transmisia unui cadru prin CAN2
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
