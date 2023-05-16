#include "OneWire.h"
#include "Delay.h"

void Onewire_Enable_GPIO_Port(void)
{
DS18B20_RCC_AHB( DS18B20_PortRCC,ENABLE); //activeaza ceasul pentru 1-Wire
TIM_Config(); //Configureaza timer-ul pentru întârzieri - uDelay
}

void Onewire_OUT_PULL_UP(void) //seteaza linia PG2 ca ie?ire Push pull
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(DS18B20_Port, &GPIO_InitStructure);
}
void Onewire_OUT_FLOATING(void) //seteaza linia PG2 ca ie?ire Open drain
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(DS18B20_Port, &GPIO_InitStructure);
}
void Onewire_IN_FLOATING(void) //seteaza linia PG2 ca intrare Open drain
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(DS18B20_Port, &GPIO_InitStructure);
}
void Onewire_IN_PULL_UP(void) //seteaza linia PG2 ca intrare Push pull
{
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(DS18B20_Port, &GPIO_InitStructure);
}
void resetOnewire(void) /* Reset */
{
Onewire_OUT_PULL_UP(); //Configureaza linia PG2 ca ie?ire
CLR_DQ(); //reseteaza pinul PG2
uDelay(500); //pulsul de reset
SET_DQ(); //seteaza pinul PG2
uDelay(260); //perioada înainte de pulsul de prezenta
Onewire_IN_PULL_UP(); //Configureaza linia PG2 ca intrare
uDelay(10);
while(!(GET_DQ())); //cite?te pulsul de prezenta de pe pinul PG2
Onewire_OUT_PULL_UP();
SET_DQ();
}

u8 rOnewire(void) /* Citirea unui octet prin 1-Wire Interface */
{
u8 data=0,i=0;
uDelay(80);
for(i=0;i<8;i++) //cite?te 8 biti
{
Onewire_OUT_PULL_UP(); //Configureaza linia PG2 ca ie?ire
CLR_DQ(); //reseteaza pinul PG2
uDelay(1); //a?teapta timpul de recuperare
data=data>>1; //deplaseaza bi?ii deja citi?i la dreapta
SET_DQ(); //seteaza pinul PG2
Onewire_IN_PULL_UP(); //configureaza linia PG2 ca intrare
uDelay(10);
if(GET_DQ())data|=0x80; //memoreaza bitul citit de pe magistrala
uDelay(40); //a?teapta pana la terminarea slotului de citire
}
return(data);
}
void wOnewire(u8 data) /* Scrierea unui octet prin 1-Wire Interface */
{
u8 i=0;
Onewire_OUT_PULL_UP(); //Configureaza linia PG2 ca ie?ire
SET_DQ(); //seteaza pinul PG2
uDelay(120);
for(i=0;i<8;i++)
{
CLR_DQ(); //reseteaza pinul PG2
if(data&0x01) //testeaza bitul ce urmeaza a fi trimis pe 1-Wire
{
SET_DQ(); //seteaza pinul PG2
}
else
{
CLR_DQ(); //reseteaza pinul PG2
}
data=data>>1; //deplaseaza la dreapta data
uDelay(65); //a?teapta 65us
SET_DQ(); //seteaza pinul PG2
}
}
