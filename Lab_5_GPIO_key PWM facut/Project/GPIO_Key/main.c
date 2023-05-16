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
#include <math.h>

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

#define LINEMAX 100
extern volatile char line_buffer[LINEMAX + 1];
extern volatile int line_valid;

 void TIM3InterruptInit (void) /*Activeaza întreruperea timer-ului */
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init (&NVIC_InitStructure);
}

void TIM3Init (void)
{
RCC_ClocksTypeDef RCC_Clocks;
RCC_GetClocksFreq (&RCC_Clocks);
uint32_t multiplier;
if (RCC_Clocks.PCLK1_Frequency == RCC_Clocks.SYSCLK_Frequency) {
multiplier = 1;
} else {
multiplier = 2;
}
uint32_t TIM3CLK_Frequency = multiplier * RCC_Clocks.PCLK1_Frequency;
uint32_t TIM3COUNTER_Frequency = 1000000;
uint16_t prescaler = (TIM3CLK_Frequency / TIM3COUNTER_Frequency) - 1;
uint16_t reload = (65000) - 1;
RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE); //clk pentru periferic
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_TimeBaseStructInit (&TIM_TimeBaseStructure); //setam valorile implicite
TIM_TimeBaseStructure.TIM_Period = reload; //modificam reload
TIM_TimeBaseStructure.TIM_Prescaler = prescaler; //modificam prescaler
TIM_TimeBaseInit (TIM3, &TIM_TimeBaseStructure); //Ini?ializam timer-ul
}

void TIM3_start (void)
{
TIM_Cmd (TIM3, ENABLE);
}

void TIM3_interrupt_enable (void)
{
TIM_ClearITPendingBit (TIM3, TIM_IT_Update);
TIM_SetCounter (TIM3, 0); //setam counter-ul
TIM_ITConfig (TIM3, TIM_IT_Update, ENABLE);
}

void TM4_TIMER_Init(void)
{
TIM_TimeBaseInitTypeDef TIM_BaseStruct;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Activam clk TIM4
//TIM_BaseStruct.TIM_Prescaler = 0; //10 khz
	TIM_BaseStruct.TIM_Prescaler = 1000;
TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up; //Count up
//TIM_BaseStruct.TIM_Period = 8399; //frecven?a semnalului PWM 10kHz
	TIM_BaseStruct.TIM_Period = 20000;
TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_BaseStruct.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIM4, &TIM_BaseStruct); //ini?ializare TIM4
TIM_Cmd(TIM4, ENABLE); //porne?te timer-ul
}

void TM4_PWM_Init(void)
{
TIM_OCInitTypeDef TIM_OCStruct;
TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2; // Clear on compare match
TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
TIM_OCStruct.TIM_Pulse = 2099; /* 25% duty cycle */
TIM_OC1Init(TIM4, &TIM_OCStruct); //ini?ializeaza canalul 1 pentru TIM4
TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_OCStruct.TIM_Pulse = 419; /* 50% duty cycle */
TIM_OC2Init(TIM4, &TIM_OCStruct);
TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_OCStruct.TIM_Pulse = 6299; /* 75% duty cycle */
TIM_OC3Init(TIM4, &TIM_OCStruct);
TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_OCStruct.TIM_Pulse = 8399; /* 100% duty cycle */
TIM_OC4Init(TIM4, &TIM_OCStruct); //ini?ializeaza canalul 4 pentru TIM4
TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void TM4_LEDS_Init(void) {
GPIO_InitTypeDef GPIO_InitStruct;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
	while(1);
}

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
	TIM3Init(); 
	TIM3InterruptInit(); 
	TIM3_start();
	TIM3_interrupt_enable();
	
	TM4_LEDS_Init();
	TM4_TIMER_Init();
	TM4_PWM_Init();
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
		
  while (1)
  {
		Led_Toggle(Read_JOYState());
		if(line_valid == 1)
		{
			printf("%s\n", line_buffer);
			line_valid = 0;
		}
		
/*	
		for(int i=0;i<=8399;i+=10)
		{
			TIM4->CCR1=i;
			Delay(0x5ffff);
		}
		TIM4->CCR1=0;
		Delay(0x2ffffff);
  }
		
		float min_duty = 0.1f;
float max_duty = 0.9f;

for (int i = 0; i < 360; i++) {
    float sine_val = sin(i * (3.14159 / 180));
    float duty = min_duty + ((max_duty - min_duty) * sine_val);
    TIM4->CCR1 = duty * TIM4->ARR;
    Delay(20);*/
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
