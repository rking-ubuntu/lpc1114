#include "i2c9664.h"
#include "lpc11xx.h"
#include "bmp.h"
void delay_ms(unsigned int ms)//ÑÓ³Ùº¯Êý£¬MS¼¶±ð
{
	unsigned int x,y;
	for(x = ms;x>0;x--)
 {
		for(y = 12000;y>0;y--);
 }
}
void delay(unsigned char num)
{
	uint8_t i = 10;
	while(num--)
	{
		while(i--);
	}
}
void BL(unsigned char n)
{
	if(n)
		LPC_GPIO0->DATA |= (1<<11);
	else 
		LPC_GPIO0->DATA &= ~(1<<11);
}
void SDA(unsigned char n)
{
	LPC_GPIO0->DIR |= (1 << 5);
	if(n)
		LPC_GPIO0->DATA |= (1<<5);
	else 
		LPC_GPIO0->DATA &= ~(1<<5);
}
void SCL(unsigned char n)
{
	if(n)
		LPC_GPIO0->DATA |= (1<<4);
	else 
		LPC_GPIO0->DATA &= ~(1<<4);
}

void LCD_GPIO_Init(void)
{
  LPC_SYSCON->SYSAHBCLKCTRL = LPC_SYSCON->SYSAHBCLKCTRL | (1<<16) | (1<<6)|(1<<5);

	LPC_IOCON->PIO0_5 |= (0<<2+1<<10);
	LPC_GPIO0->DIR |= 1 << 5;
	LPC_GPIO0->DATA |= (1<<5);
	

	LPC_IOCON->PIO0_4 |= (0<<2+1<<10); 
	LPC_GPIO0->DIR |= 1 << 4;
	LPC_GPIO0->DATA |= (1<<4);
	
	LPC_IOCON->R_PIO0_11 &= ~0x07;
	LPC_IOCON->R_PIO0_11 |= 0x01;
	LPC_GPIO0->DIR |= 1 << 11;
	LPC_GPIO0->DATA |= (1<<11);
	
}
void I2C_START()
{
	SDA(1);
	delay(1);

	SCL(1);
	delay(1);
  SDA(0);		//START
	delay(1);

	SCL(0);
	delay(1);
}
void I2C_STOP()
{
	SDA(0);
	delay(1);

	SCL(1); 		//STOP
	delay(1);

	SDA(1);

	delay(1);
	delay(1);
	delay(1);
	delay(1);
	delay(1);
	delay(1);
}

void I2C_WRITE(unsigned char dat)
{
	unsigned char i;

	for(i=0;i<8;i++)
	{
		if(dat&0x80)
			SDA(1);
		else
			SDA(0);

		dat<<=1;
		delay(1);
		SCL(1);
		delay(1);
		SCL(0);
		delay(1);
	}

	SDA(1);
	delay(1);

	SCL(1);
	delay(1);

	SCL(0);
	delay(1);
	SDA(1);
	delay(1);

	delay(1);
	delay(1);
	delay(1);
	delay(1);
	delay(1);
	delay(1);
}
void LCM_INIT()
{

	I2C_START();
	I2C_WRITE(0x78);//????????
	I2C_WRITE(0x26);//????/???? - Set TC    
	I2C_WRITE(0x2e);//Set power control   
	I2C_WRITE(0x81);//?????
	I2C_WRITE(0x70);//???? - Set Vbias  Potentiometer    
	I2C_WRITE(0x06);//????
	I2C_WRITE(0xcc);
	I2C_WRITE(0x40);//set start line  
	I2C_WRITE(0xaf);//set display enable    
	I2C_WRITE(0xea);//set LCD bias ratio    
	I2C_STOP();

}

void LCM_SP(unsigned char page)
{
	I2C_START();
	I2C_WRITE(0x78);
	I2C_WRITE(0xb0|page);
	I2C_STOP();
}

void LCM_SC(unsigned char col)
{
	I2C_START();
	I2C_WRITE(0x78);
	I2C_WRITE(0x00|(col&0x0f));
	col>>=4;
	I2C_WRITE(0x10|(col&0x0f));
	I2C_STOP();
}

void LCM_PP(unsigned char *pic)
{
 	unsigned char i,j;


	for(i=0;i<8;i++)
	{
		LCM_SP(i);
		LCM_SC(0x6);
		I2C_START();
		I2C_WRITE(0x7a);
		for(j=0;j<96;j++)
		{
			//I2C_WRITE(~*pic++);
			I2C_WRITE(*pic++);
		}
		I2C_STOP();
	}
}
void CLEAR(void)
{
 	unsigned char i,j;


	for(i=0;i<8;i++)
	{
		LCM_SP(i);
		LCM_SC(6);
		I2C_START();
		I2C_WRITE(0x7a);
		for(j=0;j<96;j++)
		{
			//I2C_WRITE(~*pic++);
			I2C_WRITE(0x00);
		}
		I2C_STOP();
	}
}
void DisplayXY(unsigned char x,unsigned char y)
{
   int num =0;
	 if(x<96 && y<64)
	 {
		 num = y/8*96+x;
		 bmp2[num] |= (1<<(y%8));
	 }
	
}
void UndisplayXY(unsigned char x,unsigned char y)
{
   int num =0;
	 if(x<96 && y<64)
	 {
			num = y/8*96+x;
			bmp2[num] &= ~(1<<(y%8));
	 }
	 
}
