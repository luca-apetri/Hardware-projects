/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_am2301.h"
#include "tm_stm32f4_ili9341.h"


#include <stdio.h>

//declarari-------------------------------------------
	#define LED1_Port				GPIOG
	#define LED1_Pin				GPIO_Pin_13
	#define LED1_RCC_AHBPeriph		RCC_AHB1Periph_GPIOG

	#define LED2_Port				GPIOG
	#define LED2_Pin				GPIO_Pin_14
	#define LED2_RCC_AHBPeriph		RCC_AHB1Periph_GPIOG
//----------------------------------------------------

void JOYState_LED_GPIO_Init(void);

int main(void) {
	TM_AM2301_Data_t data;
	char str[50];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize LCD on F429-Discovery board */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	TM_ILI9341_Fill(ILI9341_COLOR_ORANGE);
	TM_ILI9341_Puts(10, 10, "Sensor: DHT11\n 20-24 grade/30-60% umiditate", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	TM_ILI9341_Puts(20, 300, "Autor proiect: Apetri Luca &", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	TM_ILI9341_Puts(90, 310, " Opocinschi Robert", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	
	/* Initialize sensor */
	TM_AM2301_Init();
	
	/* Reset time */
	TM_DELAY_SetTime(0);
	JOYState_LED_GPIO_Init();
	while (1) {
		/* Every 1 second */
		if (TM_DELAY_Time() > 1000) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			/* Data valid */
			if (TM_AM2301_Read(&data) == TM_AM2301_OK) {
				/* Show on LCD */
				sprintf(str, "Humidity: %2.1f %%\nTemperature: %2.1f C", (float)data.Hum / 10, (float)data.Temp / 10);
				//verifica 
				if(((float)data.Hum / 10) > 60 || ((float)data.Temp / 10) < 20)
				{
					GPIO_WriteBit(LED2_Port, LED2_Pin, Bit_RESET);
					GPIO_WriteBit(LED1_Port, LED1_Pin, Bit_SET);
				}else if(((float)data.Hum / 10) < 30 || ((float)data.Temp / 10) > 24)
				{
					GPIO_WriteBit(LED1_Port, LED1_Pin, Bit_RESET);
					GPIO_WriteBit(LED2_Port, LED2_Pin, Bit_SET);}
				/* Put to LCD */
				TM_ILI9341_Puts(10, 100, str, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
			}
		}
		
		/* Do other stuff here constantly */
	}
}

void JOYState_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
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
