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

//#define Open_SPI1
#define Open_SPI4
#ifdef Open_SPI1
#define Open_RCC_APB2Periph_SPIx RCC_APB2Periph_SPI1
#define Open_SPIx SPI1
#define Open_SPIx_CLK RCC_APB2Periph_SPI1
#define Open_SPIx_CLK_INIT RCC_APB1PeriphClockCmd
#define Open_SPIx_IRQn SPI1_IRQn
#define Open_SPIx_IRQHANDLER SPI1_IRQHandler
#define Open_SPIx_SCK_PIN GPIO_Pin_5
#define Open_SPIx_SCK_GPIO_PORT GPIOA
#define Open_SPIx_SCK_GPIO_CLK RCC_AHB1Periph_GPIOA
#define Open_SPIx_SCK_SOURCE GPIO_PinSource5
#define Open_SPIx_SCK_AF GPIO_AF_SPI1
#define Open_SPIx_MISO_PIN GPIO_Pin_6
#define Open_SPIx_MISO_GPIO_PORT GPIOA
#define Open_SPIx_MISO_GPIO_CLK RCC_AHB1Periph_GPIOA
#define Open_SPIx_MISO_SOURCE GPIO_PinSource6
#define Open_SPIx_MISO_AF GPIO_AF_SPI1
#define Open_SPIx_MOSI_PIN GPIO_Pin_7
#define Open_SPIx_MOSI_GPIO_PORT GPIOA
#define Open_SPIx_MOSI_GPIO_CLK RCC_AHB1Periph_GPIOA
#define Open_SPIx_MOSI_SOURCE GPIO_PinSource7
#define Open_SPIx_MOSI_AF GPIO_AF_SPI1
#define RCC_GPIO_CS RCC_AHB1Periph_GPIOA
#define RCC_AHBxPeriphClockCmd RCC_AHB1PeriphClockCmd
#define GPIO_PIN_CS GPIO_Pin_4
#define GPIO_CS_PORT GPIOA
#else
#define Open_RCC_APB2Periph_SPIx RCC_APB2Periph_SPI4
#define Open_SPIx SPI4
#define Open_SPIx_CLK RCC_APB2Periph_SPI4
#define Open_SPIx_CLK_INIT RCC_APB1PeriphClockCmd
#define Open_SPIx_IRQn SPI4_IRQn
#define Open_SPIx_IRQHANDLER SPI4_IRQHandler
#define Open_SPIx_SCK_PIN GPIO_Pin_12
#define Open_SPIx_SCK_GPIO_PORT GPIOE
#define Open_SPIx_SCK_GPIO_CLK RCC_AHB1Periph_GPIOE
#define Open_SPIx_SCK_SOURCE GPIO_PinSource12
#define Open_SPIx_SCK_AF GPIO_AF_SPI4
#define Open_SPIx_MISO_PIN GPIO_Pin_13
#define Open_SPIx_MISO_GPIO_PORT GPIOE
#define Open_SPIx_MISO_GPIO_CLK RCC_AHB1Periph_GPIOE
#define Open_SPIx_MISO_SOURCE GPIO_PinSource13
#define Open_SPIx_MISO_AF GPIO_AF_SPI4
#define Open_SPIx_MOSI_PIN GPIO_Pin_14
#define Open_SPIx_MOSI_GPIO_PORT GPIOE
#define Open_SPIx_MOSI_GPIO_CLK RCC_AHB1Periph_GPIOE
#define Open_SPIx_MOSI_SOURCE GPIO_PinSource14
#define Open_SPIx_MOSI_AF GPIO_AF_SPI4
#define RCC_GPIO_CS RCC_AHB1Periph_GPIOE
#define RCC_AHBxPeriphClockCmd RCC_AHB1PeriphClockCmd
#define GPIO_PIN_CS GPIO_Pin_11
#define GPIO_CS_PORT GPIOE
#endif
#define BUF1_WRITE 0x84 //SRAM Buffer 1 Write
#define BUF2_WRITE 0x87 //SRAM Buffer 2 Write
#define BUF1_READ 0xD4 //Buffer 1 Read (High Frequency)
#define BUF2_READ 0xD6 //Buffer 2 Read (High Frequency)
#define BBUF1_TO_MM_PAGE_PROG_WITH_ERASE 0x83 //Buffer 1 to Main Memory Page Program with Built-In Erase
#define BBUF2_TO_MM_PAGE_PROG_WITH_ERASE 0x86 //Buffer 2 to Main Memory Page Program with Built-In Erase
#define MM_PAGE_TO_B1_XFER 0x53 //Main Memory Page to Buffer 1 Transfer
#define MM_PAGE_TO_B2_XFER 0x55 //Main Memory Page to Buffer 2 Transfer
#define PAGE_ERASE 0x81 //512/528 bytes per page
#define SECTOR_ERASE 0x7C //128k bytes per sector
#define READ_STATE_REGISTER 0xD7 //Status Register Read
#define Read_ID 0x9F //Manufacturer and Device ID Read
#define AT45DBXX_Enable GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
#define AT45DBXX_Disable GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void SPI_Configuration(void)
{
SPI_InitTypeDef SPI_InitStruct;
GPIO_InitTypeDef GPIO_InitStructure;
//Activeaza ceasul pentru periferice
RCC_AHB1PeriphClockCmd(Open_SPIx_SCK_GPIO_CLK | Open_SPIx_MISO_GPIO_CLK |
Open_SPIx_MOSI_GPIO_CLK,ENABLE);
RCC_APB2PeriphClockCmd(Open_RCC_APB2Periph_SPIx,ENABLE);
//Modifica maparea pinilor specifica?i pentru ceas, MOSI si MISO
GPIO_PinAFConfig(Open_SPIx_SCK_GPIO_PORT, Open_SPIx_SCK_SOURCE,
Open_SPIx_MOSI_AF);
GPIO_PinAFConfig(Open_SPIx_MISO_GPIO_PORT, Open_SPIx_MISO_SOURCE,
Open_SPIx_MOSI_AF);
GPIO_PinAFConfig(Open_SPIx_MOSI_GPIO_PORT, Open_SPIx_MOSI_SOURCE,
Open_SPIx_MOSI_AF);
//Configureaza pinul pentru semnalul de ceas
GPIO_InitStructure.GPIO_Pin = Open_SPIx_SCK_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
GPIO_Init(Open_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
//Configureaza pinul MISO
GPIO_InitStructure.GPIO_Pin = Open_SPIx_MISO_PIN;
GPIO_Init(Open_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
//Configureaza pinul MOSI
GPIO_InitStructure.GPIO_Pin = Open_SPIx_MOSI_PIN;
GPIO_Init(Open_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
//Configureaza SPI prin structura SPI_InitStruct
SPI_I2S_DeInit(Open_SPIx); //Reseteaza perifericul
SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex; //Specifica modul de date unidirec?ional sau bidirec?ional
SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; //Specifica dimensiunea datei SPI
SPI_InitStruct.SPI_Mode = SPI_Mode_Master; //Specifica modul de operare SPI
SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; //Specifica starea permanenta a ceasului
SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; //Specifica frontul activ al ceasului pentru captarea de bi?i.
SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; //Semnalul NSS este gestionat de software
SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //Seteaza valoarea ((uint16_t)0x0030)
SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; //Specifica faptul ca transferurile de date pornesc de la bitul MSB
SPI_InitStruct.SPI_CRCPolynomial = 7; //Specifica polinomul utilizat pentru calculul CRC.
SPI_Init(Open_SPIx, &SPI_InitStruct);
SPI_Cmd(Open_SPIx, ENABLE);
}
void SPI_Send_byte(u16 data)
{
while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET); //Verifica starea SPI
SPI_I2S_SendData(Open_SPIx,data); //Transmite data prin comunica?ia SPI
while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
SPI_I2S_ReceiveData(Open_SPIx); //Recep?ioneaza data
}
u16 SPI_Receive_byte(void)
{
while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
SPI_I2S_SendData(Open_SPIx,0x00);
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
return SPI_I2S_ReceiveData(Open_SPIx);
}
void AT45DBXX_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
SPI_Configuration(); //Ini?ializeaza pinii si SPI
RCC_AHBxPeriphClockCmd(RCC_GPIO_CS,ENABLE);
//Ini?ializeaza Chip Select
GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CS;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIO_CS_PORT, &GPIO_InitStructure);
AT45DBXX_Disable; //GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);
}
static void AT45DBXX_BUSY(void)
{
u8 AT45DBXXStruct;
AT45DBXX_Enable; //GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
SPI_Send_byte(READ_STATE_REGISTER); //Emite 0xD7
do
{AT45DBXXStruct = SPI_Receive_byte();} //Cite?te starea memoriei
while(!(AT45DBXXStruct & 0x80)); //A?teapta pana când bitul cel mai semnificativ este 1
AT45DBXX_Disable;
}
void AT45DBXX_Read_ID(u8 *IData)
{
u8 i;
AT45DBXX_BUSY();
AT45DBXX_Enable; //GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
SPI_Send_byte(Read_ID); //0x9F pentru a citi ID-ul
for(i=0;i<4;i++)
{
IData[i] = SPI_Receive_byte(); //Salveaza ID-ul in IData
}
AT45DBXX_Disable; //GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);
}
void write_buffer(u16 BufferOffset,u8 Data)
{
AT45DBXX_Enable; //GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
SPI_Send_byte(0x84); //BUF1_WRITE
SPI_Send_byte(0xff);
SPI_Send_byte((u8)BufferOffset>>8);
SPI_Send_byte((u8)BufferOffset);
SPI_Send_byte(Data);
AT45DBXX_Disable; //GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);
}
u8 read_buffer(u16 BufferOffset)
{
u8 temp=0;
AT45DBXX_Enable; //GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
SPI_Send_byte(0xd4); //BUF1_READ
SPI_Send_byte(0xff);
SPI_Send_byte((u8)BufferOffset>>8);
SPI_Send_byte((u8)BufferOffset);
SPI_Send_byte(0xff);
temp=SPI_Receive_byte();
AT45DBXX_Disable; //GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);
return temp; //Returneaza data citita prin SPI
}
 

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
  /* Output a message on Hyperterminal using printf function */ 
  RCC_GetClocksFreq(&SYS_Clocks);
	printf("\r\nSYSCLK:%dM\r\n",SYS_Clocks.SYSCLK_Frequency/1000000);
	printf("HCLK:%dM\r\n",SYS_Clocks.HCLK_Frequency/1000000);
	printf("PCLK1:%dM\r\n",SYS_Clocks.PCLK1_Frequency/1000000);
	printf("PCLK2:%dM\r\n",SYS_Clocks.PCLK2_Frequency/1000000);	
	printf("\n\r KEY Example \r\n");
		JOYState_LED_GPIO_Init();
	
	//SPI---------------------------
	u8 num =0;
	u8 ID[4];
	
	printf("\r\nComunicatia SPI: STM32F29 & Open429Z-D!\r\n");
	AT45DBXX_Init();
	printf("AT45DBXX este initializat!\r\n");
	AT45DBXX_Read_ID(ID);
	printf("ID-ul AT45DB041E DataFlash (4-Mbit) este:");
	for(num = 0; num<4;num++)
	{
		printf("0x%x ",ID[num]);
	}
	printf("\r\n");
	
  while (1)
  {
		Led_Toggle(Read_JOYState());
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
