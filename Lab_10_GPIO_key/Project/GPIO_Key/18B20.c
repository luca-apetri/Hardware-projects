#include "18B20.h"
void convertDs18b20(void) //lanseaza conversia temperaturii
{
resetOnewire(); //realizeaza secven?a de reset
wOnewire(0xcc); //lanseaza conversia circuitele conectate la 1-Wire
wOnewire(0x44); //ini?iaza un ciclu de citire a temperaturii
}

u8* readID(void)
{
u8 ID[8],i,*p;
resetOnewire(); //realizeaza secven?a de reset
wOnewire(0x33); //Comanda 0x33 (Read Rom)
for(i=0;i<8;i++)
{ID[i]=rOnewire();}
p=ID;
return p;
}

u16 readTemp(void)
{
u8 temp1,temp2;
convertDs18b20();
resetOnewire(); //realizeaza secven?a de reset
wOnewire(0xcc);
wOnewire(0xbe); //cite?te con?inutul memoriei scratchpad din DS18B20
temp1=rOnewire(); //cite?te BYTE0 (LSB)
temp2=rOnewire(); //cite?te BYTE1 (MSB)
return (((temp2<<8)|temp1)*6.25); //0.0625=xx, 0.625=xx.x, 6.25=xx.xx corespunzator rela?iei data/temperatura
}
