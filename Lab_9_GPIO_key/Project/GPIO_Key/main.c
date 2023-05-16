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
#define Open_I2C1
//#define Open_I2C2
#ifdef Open_I2C1
#define Open_I2Cx I2C1
#define Open_I2Cx_CLK RCC_APB1Periph_I2C1
#define Open_I2Cx_SDA_PIN GPIO_Pin_7
#define Open_I2Cx_SDA_GPIO_PORT GPIOB
#define Open_I2Cx_SDA_GPIO_CLK RCC_AHB1Periph_GPIOB
#define Open_I2Cx_SDA_SOURCE GPIO_PinSource7
#define Open_I2Cx_SDA_AF GPIO_AF_I2C1
#define Open_I2Cx_SCL_PIN GPIO_Pin_6
#define Open_I2Cx_SCL_GPIO_PORT GPIOB
#define Open_I2Cx_SCL_GPIO_CLK RCC_AHB1Periph_GPIOB
#define Open_I2Cx_SCL_SOURCE GPIO_PinSource6
#define Open_I2Cx_SCL_AF GPIO_AF_I2C1
#elif defined Open_I2C2
#define Open_I2Cx I2C2
#define Open_I2Cx_CLK RCC_APB1Periph_I2C2
#define Open_I2Cx_SDA_PIN GPIO_Pin_0
#define Open_I2Cx_SDA_GPIO_PORT GPIOF
#define Open_I2Cx_SDA_GPIO_CLK RCC_AHB1Periph_GPIOF
#define Open_I2Cx_SDA_SOURCE GPIO_PinSource0
#define Open_I2Cx_SDA_AF GPIO_AF_I2C2
#define Open_I2Cx_SCL_PIN GPIO_Pin_1
#define Open_I2Cx_SCL_GPIO_PORT GPIOF
#define Open_I2Cx_SCL_GPIO_CLK RCC_AHB1Periph_GPIOF
#define Open_I2Cx_SCL_SOURCE GPIO_PinSource1
#define Open_I2Cx_SCL_AF GPIO_AF_I2C2
#endif
#include <string.h>
#define I2Cx_SPEED 100000
#define I2Cx_SLAVE_ADDRESS7 0x30
#define ADDR_24LC02 0xA0
#define I2C_PAGESIZE 4
#define I2C_Open_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2C_Open_LONG_TIMEOUT ((uint32_t)(10 * I2C_Open_FLAG_TIMEOUT))
__IO uint32_t I2CTimeout = I2C_Open_LONG_TIMEOUT;
void I2C_Configuration(void) //#define AT24C01A
{
I2C_InitTypeDef I2C_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
//Activeaza ceasul pentru magistrala
RCC_AHB1PeriphClockCmd(Open_I2Cx_SDA_GPIO_CLK | Open_I2Cx_SCL_GPIO_CLK,ENABLE);
RCC_APB1PeriphClockCmd(Open_I2Cx_CLK,ENABLE);
//Modifica maparea pinilor SDA si SCL ca func?ii alternative I2C
GPIO_PinAFConfig(Open_I2Cx_SDA_GPIO_PORT, Open_I2Cx_SDA_SOURCE, Open_I2Cx_SDA_AF);
GPIO_PinAFConfig(Open_I2Cx_SCL_GPIO_PORT, Open_I2Cx_SCL_SOURCE, Open_I2Cx_SCL_AF);
//Configureaza si ini?ializeaza pinul SDA
GPIO_InitStructure.GPIO_Pin = Open_I2Cx_SDA_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //func?ie alternativa
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(Open_I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
//Configureaza si ini?ializeaza pinul SCL
GPIO_InitStructure.GPIO_Pin = Open_I2Cx_SCL_PIN;
GPIO_Init(Open_I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
I2C_DeInit(Open_I2Cx);
I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; //Modul I2C 0x0000
I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //I2C fast mode Tlow/Thigh = 2
I2C_InitStructure.I2C_OwnAddress1 = I2Cx_SLAVE_ADDRESS7; //Adresa 0x30 pe 7 biti
I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; //Activeaza confirmarile
I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
I2C_InitStructure.I2C_ClockSpeed = I2Cx_SPEED; //Frecventa ceasului I2C= 100000
I2C_Cmd(Open_I2Cx, ENABLE); //Activeaza perifericul I2C: I2Cx->CR1 |= I2C_CR1_PE;
I2C_Init(Open_I2Cx, &I2C_InitStructure); //Ini?ializeaza I2C
I2C_AcknowledgeConfig(Open_I2Cx, ENABLE); //Activeaza caracteristica de confirmare I2C
}
static void I2C_delay(uint16_t cnt){
while(cnt--);
}

uint32_t I2C_TIMEOUT_UserCallback(void)
{
printf("\r\n Comunica?ia I2C este întrerupta! \r\n");
while (1)
{
}
}
static void I2C_AcknowledgePolling(I2C_TypeDef *I2Cx,uint8_t I2C_Addr)
{
vu16 SR1_Tmp;
do
{
I2C_GenerateSTART(I2Cx, ENABLE); //Genereaza condi?ia de START pentru comunica?ia I2Cx.
SR1_Tmp = I2C_ReadRegister(I2Cx, I2C_Register_SR1); //Cite?te registrul I2C si returneaza valoarea acestuia in SR1_Tmp.
I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter); //Transmite adresa pentru a selecta dispozitivul slave.
}while(!(I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0x0002));//repeta pana când bitul ADDR din registrul SR1 este 1
I2C_ClearFlag(I2Cx, I2C_FLAG_AF); //?terge flag-ul AF I2C_FLAG_AF
I2C_GenerateSTOP(I2Cx, ENABLE); //Opre?te comunica?ia I2Cx.
}
uint8_t I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num)
{ //Cite?te date la I2C1. Parametri: Adresa dispozitivului, adresa de citire, pointerul la buffer, nr. de bytes
if(num==0) return 1;
while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); //verifica flag-ul BUSY din registrul SR2
I2C_AcknowledgeConfig(I2Cx, ENABLE); //Activeaza op?iunea de confirmare pentru I2C
I2C_GenerateSTART(I2Cx, ENABLE); //Genereaza condi?ia de START pentru comunica?ia I2Cx.
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); //Verifica daca ultimul eveniment I2Cx este I2C_EVENT_MASTER_MODE_SELECT
I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter); //Transmite adresa pentru a selecta dispozitivul si direc?ia
while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
I2C_SendData(I2Cx, addr); //Trimite adresa
while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
I2C_GenerateSTART(I2Cx, ENABLE); //Genereaza condi?ia de START pentru comunica?ia I2Cx.
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver); //Transmite adresa pentru a selecta direc?ia dispozitivului
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
while (num) {//repeta pana când nu mai sunt octe?i de citit
if(num==1){
I2C_AcknowledgeConfig(I2Cx, DISABLE);
I2C_GenerateSTOP(I2Cx, ENABLE);
}
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); //A?teapta recep?ia
*buf = I2C_ReceiveData(I2Cx); //memoreaza data primita
buf++; //Incremeteaza pointerul
num--; //Decrementeaza numarul de octe?i de citit din memorie
}
I2C_AcknowledgeConfig(I2Cx, ENABLE); //Activeaza op?iunea de confirmare pentru I2C
return 0; }
uint8_t I2C_WriteOneByte(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t value)
{
I2CTimeout = I2C_Open_LONG_TIMEOUT;
while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) //verifica flag-ul BUSY din registrul SR2
{
if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();//Comunica?ie e?uata
}
I2C_GenerateSTART(I2Cx, ENABLE); //Genereaza condi?ia de START pentru comunica?ia I2Cx.
I2CTimeout = I2C_Open_FLAG_TIMEOUT;
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
{ //Dupa trimiterea condi?iei START (I2C_GenerateSTART ()), masterul trebuie sa a?tepte acest eveniment.
//Înseamna ca condi?ia Start a fost trimisa corect pe magistrala I2C
//Magistrala este libera, nu exista alte dispozitive care comunica.
if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
}
I2CTimeout = I2C_Open_FLAG_TIMEOUT;
I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter); //Transmite adresa pentru a selectadispozitivul si direc?ia
/*Dupa ce a verificat starea de pornire, master-ul trimite adresa circuitului slave cu care va comunica (functia
I2C_Send7bitAddress(), determinând de asemenea direc?ia de comunica?ie (transmi?ator sau receptor). Masterul
trebuie sa a?tepte ca slave-ul sa-si recunoasca adresa. Daca este trimisa o confirmare pe magistrala I2C, se va
seta unul dintre urmatoarele evenimente:
1) În cazul in care MASTER-ul recep?ioneaza: I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
2) În cazul in care MASTER-ul transmite: I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED */
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
{
	if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
}
/*Daca este stabilita o comunica?ie (condi?ia START generata ?i adresa slave recunoscuta), master-ul trebuie sa
verifice unul dintre urmatoarele evenimente pentru procedurile de comunica?ie:
1) Modul Master Receiver: Master-ul trebuie sa a?tepte la evenimentul
I2C_EVENT_MASTER_BYTE_RECEIVED pentru a citi datele primite de la slave (I2C_ReceiveData()).
2) Modul Master Transmitter: Master-ul trebuie sa trimita date (func?ia I2C_SendData()) apoi sa a?tepte la
evenimentul I2C_EVENT_MASTER_BYTE_TRANSMITTING sau
I2C_EVENT_MASTER_BYTE_TRANSMITTED. Aceste doua evenimente sunt similare:
- I2C_EVENT_MASTER_BYTE_TRANSMITTING înseamna ca datele au fost scrise în registrul de date ?i au
fost trimise.
- I2C_EVENT_MASTER_BYTE_TRANSMITTED înseamna ca datele au fost deplasate din registru ?i sunt
pe magistrala I2C.
În majoritatea cazurilor, utilizarea I2C_EVENT_MASTER_BYTE_TRANSMITTING este suficienta pentru
aplica?ie. Utilizarea I2C_EVENT_MASTER_BYTE_TRANSMITTED duce la o comunica?ie mai lenta, dar
asigura un test mai fiabil.
I2C_EVENT_MASTER_BYTE_TRANSMITTED este, de asemenea, mai potrivit decât
I2C_EVENT_MASTER_BYTE_TRANSMITTING pentru testarea ultimei transmisii de date (înainte de
generarea condi?iilor Stop).*/
I2C_SendData(I2Cx, addr); //Trimite adresa
I2CTimeout = I2C_Open_FLAG_TIMEOUT;
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//;
{
if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
}
I2C_SendData(I2Cx, value); //Trimite un octet de date prin perifericul I2C: I2Cx->DR = Data;
I2CTimeout = I2C_Open_FLAG_TIMEOUT;
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//;
{
if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
}
I2C_GenerateSTOP(I2Cx, ENABLE); //Opre?te comunica?ia I2Cx.
I2C_AcknowledgePolling(I2Cx,I2C_Addr); //Ob?ine confirmarea circuitului I2C
I2C_delay(1000);
return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num)
{ //Scrie date la I2C1, Adresa dispozitivului, adresa de scriere, pointerul la buffer, nr. de bytes
uint8_t err=0;
while(num--)
{
if(I2C_WriteOneByte(I2Cx, I2C_Addr,addr++,*buf++))
{
err++;
}
}
if(err)
return 1;
else
return 0; //Datele au fost trimise prin I2C fara erori
}




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
	I2C_Configuration();
	printf("\r\nMemoria EEPROM AT24C01A este configurata!\r\n");
	uint16_t Addr;
	uint8_t WriteBuffer[256], ReadBuffer[256];
	for(Addr=0;Addr<256;Addr++)
		WriteBuffer[Addr]=Addr ;
	printf("\r\n Scrierea in memoria EEPROM 24C02 \r\n");
	I2C_Write(Open_I2Cx,ADDR_24LC02,0,WriteBuffer,sizeof(WriteBuffer));
	printf("\r\n Citire din memoria EEPROM 24C02 \r\n");
	I2C_Read(Open_I2Cx,ADDR_24LC02,0,ReadBuffer,sizeof(WriteBuffer));
	if(memcmp(WriteBuffer,ReadBuffer,sizeof(WriteBuffer)) == 0 )
		printf("\r\n I2C EEPROM 24C02 Test OK\r\n");
	else
		printf("\r\n I2C EEPROM 24C02 Test esuat\r\n");
		JOYState_LED_GPIO_Init();
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
