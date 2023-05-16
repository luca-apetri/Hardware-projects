
#include "KEY&LED.h"

#define USER_KEY_Port					GPIOA
#define USER_KEY_Pin					GPIO_Pin_0
#define USER_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOA

#define LED1_Port				GPIOG
#define LED1_Pin				GPIO_Pin_13
#define LED1_RCC_AHBPeriph		RCC_AHB1Periph_GPIOG

#define LED2_Port				GPIOG
#define LED2_Pin				GPIO_Pin_14
#define LED2_RCC_AHBPeriph		RCC_AHB1Periph_GPIOG


static void Delay(__IO uint32_t nCount);

void JOYState_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	/*--------------------------------------------------------------------------------
	
	Key GPIO Config
	
	
	--------------------------------------------------------------------------------*/
	/* Activarea ceasului pentru periferile */
		RCC_AHB1PeriphClockCmd(LED1_RCC_AHBPeriph | LED2_RCC_AHBPeriph , ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED1_Pin;
	//...Initializare tasta USER
	


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED1_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LED2_Pin;
	GPIO_Init(LED2_Port, &GPIO_InitStructure);
	//....
	
}

void Led_Toggle(uint8_t keyvulua)
{
		//if(((float)data.Hum / 10) > 60 || ((float)data.Temp / 10) < 20)

		GPIO_WriteBit(LED2_Port, LED2_Pin, Bit_SET);
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}