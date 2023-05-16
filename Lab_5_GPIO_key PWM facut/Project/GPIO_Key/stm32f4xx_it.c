/**
  ******************************************************************************
  * @file    USART/USART_Printf/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "KEY&LED.h"
#include "usart.h"
#include <string.h>

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
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


#define LINEMAX 100 // Lungimea maxima admisa / a?teptata a liniei
volatile char line_buffer[LINEMAX + 1]; //memoreaza caracterele primite prin USART
volatile int line_valid = 0; //datele sunt valide

void USART1_IRQHandler(void)
{
	static char rx_buffer[LINEMAX]; //buffer local pentru memorarea liniei
	static int rx_index = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //s-a recep?ionat un caracter?
	{
		char rx = USART_ReceiveData(USART1);
		if ((rx == '\r') || (rx == '\n')) //este o condi?ie de sfâr?it de linie?
		{
			if (rx_index != 0) //linia are con?inut?
			{
				memcpy((void *)line_buffer, rx_buffer, rx_index); //copie datele primite
				line_buffer[rx_index] = 0; //adauga null
				line_valid = 1; //seteaza indicator
				rx_index = 0; //reseteaza index
			}	
		}else{
			if (rx_index == LINEMAX)
			rx_index = 0;
			rx_buffer[rx_index++] = rx;
		}
	}
}

/*
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if((char)USART_ReceiveData(USART1) == 'W')
		{
			Led_Toggle(4);
			USART_SendData(USART1, 'w');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}
		}
	}
}*/

void TIM3_IRQHandler(void)
{
	static uint32_t count=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
		count++;
		if(count&(20000-1)){
			GPIO_ToggleBits(GPIOG,GPIO_Pin_13);
			GPIO_ToggleBits(GPIOG,GPIO_Pin_14);
			count=0;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
