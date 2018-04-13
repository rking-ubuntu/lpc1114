#include "LPC11XX.h"
#include "led.h"
#include "key.h"
#include "system_init.h"
#include "digitSeg.h"
#include "beep.h"
//#include "lcd.h"
//#include "lcd2.h"
//#include "oled.h"
#include "i2c9664.h"
//#include "bmp.h"
/*????*/
#define D0 0x01
#define D1 0x02
#define D2 0x04
#define D3 0x08

#define UP 0x02
#define DOWN 0x08
#define LEFT 0x06
#define RIGHT 0x04

#define key  LPC_GPIO3->DATA 

extern unsigned char bmp2[];
uint8_t cs;
uint8_t digit[4];
uint8_t min;
uint8_t s;
uint8_t val = 1;
int snake[60][2];
int old_snake[60][2];
int len = 6;
int dir = RIGHT;

void myDelay(uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
      for (i = 0; i < 5000; i++);
    }
}



void TIMER16_0_Init(uint16_t uIMsTime)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
	LPC_TMR16B0->IR = 1;
	LPC_TMR16B0->PR = 48000000/1000;	 
	LPC_TMR16B0->MCR = 3;
	LPC_TMR16B0->MR0 = uIMsTime-1;
	LPC_TMR16B0->TCR = 0x01;
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
	NVIC_SetPriority(TIMER_16_0_IRQn,1);
}

void TIMER16_1_Init(uint16_t MsTime)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
	LPC_TMR16B1->IR = 1;
	LPC_TMR16B1->PR = 48000000/1000 - 1;
	LPC_TMR16B1->MCR = 3;
	LPC_TMR16B1->MR0 = MsTime;
	LPC_TMR16B1->TCR = 0x01;
	NVIC_EnableIRQ(TIMER_16_1_IRQn);
	NVIC_SetPriority(TIMER_16_1_IRQn,2);
}

void TIMER16_1_IRQHandler(void)
{	
	int i = 0;							   
	if(cs == 0)	 
		Seg_Display(D0,digit[0]);	
	else if(cs == 1)
		Seg_Display(D1,digit[1]);
	else if(cs == 2)
	  	Seg_Display(D2,digit[2]);
	else if(cs == 3)
	  	Seg_Display(D3,digit[3]);
	cs++;
	cs %= 4;
	//LCM_PP(bmp2);
	for(i=0;i<3;i++)
	{
		LPC_GPIO3->DATA &= ~(0x01<<(i+3));
//		tmp = key;
//		tmp = 	  (key&0x07);
//		tmp = 	   (key&0x07)^0x07;
		switch((key&0x07)^0x07)
		{
			case (0x01):          val=1+i;break;
			case (0x02):          val=4+i;break;
			case (0x04):          val=7+i;break;

		}
		LPC_GPIO3->DATA |= (0x01<<(i+3));	
	}
	LPC_TMR16B1->IR = 1;
}  
void TIMER16_0_IRQHandler(void)
{
	//LCM_PP(bmp2);
	int i;
	int x=0,y=0;

	
	LPC_TMR16B0->IR = 1;
}

  

int main(void)
{

	int i=0;
	int x=0,y=0;
	LedOutputCfg();
	Key3_3_Init();
	DigitSeg_Init();
	Beep_Init();
	
  LCD_GPIO_Init();
	
	LPC_GPIO1->DATA |= (1<<9);
	myDelay(200);	
	LPC_GPIO1->DATA &= ~(1<<9);
	myDelay(200);	
	LPC_GPIO1->DATA |= (1<<9);

	LCM_INIT();
	CLEAR();
	
	for(i=0;i<6;i++)
	{
		snake[i][0]=30+i;
		snake[i][1]=32;
	}
	TIMER16_1_Init(5);
	//TIMER16_0_Init(200);
	BL(0);
	dir = RIGHT;
	val = RIGHT;
	while(1)
	{
		x = 0;
		y = 0;
		if((dir == UP&&val == DOWN) || (dir == DOWN&&val == UP)||(dir == LEFT&&val == RIGHT)||(dir == RIGHT&&val == LEFT))
			dir = dir;
		else if(val == UP||val == DOWN||val == LEFT||val == RIGHT)
			dir = val;
			
		if(dir == UP)
			y=-1;
		else if(dir == DOWN)
			y=1;
		else if(dir == RIGHT)
			x = -1;
		else
			x = 1;
		for(i=len-1;i>0;i--)
		{
			snake[i][0]=snake[i-1][0];
			snake[i][1]=snake[i-1][1];
		}
		snake[0][0] += x;
		snake[0][1] += y;
		
		for(i=0;i<len;i++)
		{
			DisplayXY(snake[i][0],snake[i][1]);
			old_snake[i][0] = snake[i][0];
			old_snake[i][1] = snake[i][1];
		}
		LCM_PP(bmp2);
		myDelay(100);
		for(i=0;i<len;i++)
		{
			UndisplayXY(old_snake[i][0],old_snake[i][1]);
		}
    digit[0] = dir;
	}
  //
	
}

