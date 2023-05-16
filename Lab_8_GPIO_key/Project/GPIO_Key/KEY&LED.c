
#include "KEY&LED.h"
#include "usart.h"

#define USER_KEY_Port					GPIOA
#define USER_KEY_Pin					GPIO_Pin_0
#define USER_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOA

/****************************************************************/
#define JOY_A_KEY_Port					GPIOG
#define JOY_A_KEY_Pin					GPIO_Pin_6
#define JOY_A_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_B_KEY_Port					GPIOG
#define JOY_B_KEY_Pin					GPIO_Pin_9
#define JOY_B_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_C_KEY_Port					GPIOG
#define JOY_C_KEY_Pin					GPIO_Pin_10
#define JOY_C_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_D_KEY_Port					GPIOG
#define JOY_D_KEY_Pin					GPIO_Pin_11
#define JOY_D_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_CTR_KEY_Port				GPIOG
#define JOY_CTR_KEY_Pin					GPIO_Pin_12
#define JOY_CTR_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOG
/****************************************************************/


static void Delay(__IO uint32_t nCount);
void JOYState_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	/*--------------------------------------------------------------------------------
	
	Key GPIO Config
	
	--------------------------------------------------------------------------------*/
	/* Activarea ceasului pentru periferile */
	RCC_AHB1PeriphClockCmd(USER_KEY_RCC_AHBPeriph |  JOY_A_KEY_RCC_AHBPeriph |
							 JOY_B_KEY_RCC_AHBPeriph |  JOY_C_KEY_RCC_AHBPeriph |  JOY_D_KEY_RCC_AHBPeriph |
							  JOY_CTR_KEY_RCC_AHBPeriph, ENABLE);
	
	/* Configurarea intrarilor in pushpull mode */
	GPIO_InitStructure.GPIO_Pin = JOY_A_KEY_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(JOY_A_KEY_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = JOY_B_KEY_Pin;
	GPIO_Init(JOY_B_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_C_KEY_Pin;
	GPIO_Init(JOY_C_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_D_KEY_Pin;
	GPIO_Init(JOY_D_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_CTR_KEY_Pin;
	GPIO_Init(JOY_CTR_KEY_Port, &GPIO_InitStructure);
	//...Initializare tasta USER
	//....
	
}

void write_buffer(u16 BufferOffset,u8 Data);
u8 read_buffer(u16 BufferOffset);
u8 tmp;
u8 num=0;

uint8_t Read_JOYState(void)
{
  uint8_t vulua;
  if(!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		vulua=0;
	else if(!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		vulua=1;
	else if(!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		vulua=2;
	else if(!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		vulua=3;
	else if(!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin))
		vulua=4;
	else
	 vulua=5;
	Delay(0xffff);
	
	while((!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))||
		    (!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))||
     	  (!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))||
	      (!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))||
	      (!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin)));
	Delay(0xfffff);
	
	return vulua;
}

void Led_Toggle(uint8_t keyvulua)
{
	switch(keyvulua)
		{
			case 0:
          printf("KEY A \r\n");
			printf("\r\nCitire din memoria FLASH AT45DBXX: \r\n");
			for(num = 0; num<255;num++)
			{
				tmp=read_buffer((u16)num);
				printf("%3d ",tmp);
			}
				break;
			case 1:
          printf("KEY B \r\n");
					printf("\r\nScriere in memoria FLASH AT45DBXX: \r\n");
					for(num = 0; num<255;num++)
				{
					tmp=read_buffer((u16)num);
					printf("%3d ",tmp);
				}
				break;
			case 2:
			  	printf("KEY C \r\n");
				break;
			case 3:
				  printf("KEY D \r\n");
				break;
			case 4:
				  printf("KEY Center \r\n");
				break;

			case 5:
				//printf("error \r\n");
				break;

		}
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
