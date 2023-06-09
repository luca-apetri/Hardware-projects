#include "Delay.h"

__IO uint32_t BSP_delay = 0;
#define TIM_MSEC_DELAY 0x01
#define TIM_USEC_DELAY 0x02
static void BSP_SetTime(uint8_t unit)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_Cmd(TIM3,DISABLE);
TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
if(unit == TIM_USEC_DELAY)
{
TIM_TimeBaseStructure.TIM_Period = 8;
}
else if(unit == TIM_MSEC_DELAY)
{
TIM_TimeBaseStructure.TIM_Period = 11999;
}
TIM_TimeBaseStructure.TIM_Prescaler = 5;
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
TIM_ARRPreloadConfig(TIM3, ENABLE);
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //Configureaza Óntreruperea TIM
}
static void BSP_Delay(uint32_t nTime, uint8_t unit)
{
BSP_delay = nTime;
BSP_SetTime(unit);
TIM_Cmd(TIM3, ENABLE); //Activeaza TIM3
while(BSP_delay != 0);
TIM_Cmd(TIM3, DISABLE);
}
void uDelay (const uint32_t usec)
{
BSP_Delay(usec,TIM_USEC_DELAY);
}
void mDelay (const uint32_t msec)
	{
BSP_Delay(msec,TIM_MSEC_DELAY);
}
void TIM_Config(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
// Seteaza adresa de baza pentru Vector Table la 0x08000000
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
// Configureaza prioritatea grupului pe 2 biti
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//Configureaza Óntreruperea pentru TIM3
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}
void Delay_TimerIRQ (void)
{
if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
{
TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
if (BSP_delay > 0x00)
{
BSP_delay--;
}
else
{
TIM_Cmd(TIM3,DISABLE);
}
}
}
