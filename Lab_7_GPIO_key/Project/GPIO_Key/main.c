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
#define ADC1_DR_Address ((u32)0x4001204C)
float AD_value_1, AD_value_2;
vu16 ADC_ConvertedValue[2];
void Delay(__IO uint32_t nCount);
void ADC1_CH6_DMA_Config(void);
void ADC1_CH6_DMA_Config(void)
{
DMA_InitTypeDef DMA_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Ceasul AHB2 - ADC
DMA_InitStructure.DMA_Channel = DMA_Channel_0; //Specifica canalul utilizat pentru fluxul dat
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address; //Specifica adr de baza
DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue; //Adresa de baza de mem.
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //Specifica direc?ia datelor: de la ADC catre mem.
DMA_InitStructure.DMA_BufferSize = 2; //Specifica dimensiunea buffer-ului
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //Nu incrementam registrul de adrese
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //Incrementam adresa de memorie DMA
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Specifica dim. datelor
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //Specifica dim. datelor
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //Este selectat modul DMA circular
DMA_InitStructure.DMA_Priority = DMA_Priority_High; //Specifica prioritatea DMA
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //Este dezactivat modul FIFO
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //Configureaza transferul la memorie
DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //Configureaza transferul la perif.
DMA_Init(DMA2_Stream0, &DMA_InitStructure); //Ini?ializeaza DMA folosind structura DMA_InitStructure
DMA_Cmd(DMA2_Stream0, ENABLE); //Activeaza DMA2_Stream0 (DMA1(Stream0:7), DMA2(Stream0:7))
/* Configureaza pinii PA.6 si PA.7 ca intrari analogice > ADC1 Channel6 */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //Pinii conecta?i la poten?iometre
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //Modul analogic
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //Fara aga?are Up/Down
GPIO_Init(GPIOA, &GPIO_InitStructure); //Ini?ializarea GPIOA.6 si GPIOA.7
/* Ini?ializarea ADC Common */
ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; //Configureaza ADC sa opereze in modul indep.
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //Selecteaza frecven?a ceasului pentru ADC (comun pentru toate ADC-urile)
ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA pentru modul multi ADC
ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //Configureaza întârzierea dintre doua e?antioane
ADC_CommonInit(&ADC_CommonInitStructure); //Ini?ializarea structurii ADC_CommonInitStructure
/* Ini?ializarea ADC1 */
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //Configureaza precizia ADC dual mode
ADC_InitStructure.ADC_ScanConvMode = ENABLE; //Specifica daca conversia se efectueaza în modul Scanare (multicanal) sau Single (un canal).
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //Activam conversia continua
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //Fara trigger
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //Aliniamentul datelor la dreapta
ADC_InitStructure.ADC_NbrOfConversion = 2; //Specifica numarul de conversii ADC care vor fi efectuate pentru grupul de canale obi?nuit.
ADC_Init(ADC1, &ADC_InitStructure); //Ini?ializeaza structura ADC_InitStructure
/* Configurarea ADC1 regulaar channel 6 */
ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1,ADC_SampleTime_3Cycles);//Configureaza canalul regulat ADC selectat, rangul corespunzator in sequencer si timpul de esantionare
ADC_RegularChannelConfig(ADC1, ADC_Channel_7,2,ADC_SampleTime_3Cycles);
/*Activarea cererii DMA dupa ultimul transfer (Single-ADC mode) */
ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
/* Activarea ADC1 DMA*/
ADC_DMACmd(ADC1, ENABLE);
/*Activare ADC1 */
ADC_Cmd(ADC1, ENABLE);
//Activeaza pornirea conversiei ADC pentru canalele respective
ADC_SoftwareStartConv(ADC1);
}

void Delay(__IO uint32_t nCount)
{
while(nCount--)
{
}
}

void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);
void DAC_Configuration(void);
DAC_InitTypeDef DAC_InitStructure;
void GPIO_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
/* Configureaza DAC channel 1 & 2 */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; //(DAC_OUT1 = PA.4), (DAC_OUT2 = PA.5)
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //Selecteaza modul analogic
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //Fara aga?are Up/Down
GPIO_Init(GPIOA, &GPIO_InitStructure); //Ini?ializeaza GPIOA4 si GPIOA5
}	

void RCC_Configuration(void)
{
/* Activeaza ceasul pentru periferice - GPIOA */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
/* Activeaza ceasul pentru DAC */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
/* Activeaza ceasul pentru TIM2 */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
void TIM_Configuration(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
/* Configureaza TIM2 */
TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
TIM_TimeBaseStructure.TIM_Period = 0x1; //Perioada TIM
TIM_TimeBaseStructure.TIM_Prescaler = 0xA; //Prescaller-ul
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; //TIM_ClockDivision
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //Selecteaza modul counter UP
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
/* Selecteaza modul de ie?ire trigger (TRGO) pentru TIM2*/
TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
}
void DAC_Configuration(void)
{ /* Configureaza DAC channel1 */
DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO; //Specifica triggerul extern pentru canalul DAC
DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle; //Specifica forma de unda generata
DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_127; //Amplitudinea semnalului
DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //Dezactiveaza buffer-ul
DAC_Init(DAC_Channel_1, &DAC_InitStructure);
/* Configureaza DAC channel2 */
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_255; //Amplitudinea semnalului
	DAC_Init(DAC_Channel_2,&DAC_InitStructure);
/*Activeaza DAC Channel1: Daca DAC channel1 este activat, pinul PA.04 este conectat automat la DAC.*/
	DAC_Cmd(DAC_Channel_1,ENABLE);
/*Activeaza DAC Channel2: Daca DAC channel2 este activat, pinul PA.05 este conectat automat la DAC.*/	
DAC_Cmd(DAC_Channel_2,ENABLE);
	//Seteaza registrul DAC dual channel DHR12RD, respective data pentru incarcare*/
DAC_SetDualChannelData(DAC_Align_12b_R, 0x100, 0x100);
//Activeaza counterul TIM2
	TIM_Cmd(TIM2,ENABLE);
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
		JOYState_LED_GPIO_Init();
	//ADC-----------------------------------------------
ADC1_CH6_DMA_Config();
	//DAC---------------------------------------------------
RCC_Configuration();
GPIO_Configuration();
  //TIM2
	TIM_Configuration();
	DAC_Configuration();
	
  while (1)
  {
		Led_Toggle(Read_JOYState());
		Delay(0x1ffffff);
		/* Afiseaza un mesaj pe portul serial cu valoarea rezultata de la ADC */
		printf("\r\nADC+DMA : STM32F429**************************\r\n");
		
		AD_value_2 = ADC_ConvertedValue[1];
		printf("Intrarea analogica 1 = %5.0f \r\n", AD_value_2);
		AD_value_2 = (float)(AD_value_2/0xffff)*3.3;
		printf("Intrarea analogica 1 = %4.2fV \r\n", AD_value_2);
		
		AD_value_2 = ADC_ConvertedValue[0];
		printf("Intrarea analogica 2 = %5.0f \r\n", AD_value_2);
		AD_value_2 = (float)(AD_value_2/0xffff)*3.3;
		printf("Intrarea analogica 2 = %4.2fV \r\n", AD_value_2);
	
	if(AD_value_2<1){
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1; //127
		DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	}else{
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_255; //127
		DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	}
	
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
