#include "lcd2.h"
#include "lcd.h"
#include "lpc11xx.h"
void sdly()
{
    //unsigned int i;
    
    //for(i = 0; i < 1;i++);
}

//void delay(unsigned long dly)
//{
// 	while(dly--)
//		sdly();
//}
void I2C_START(void)
{
	SDA(1);
	sdly();

	SCL(1);
	sdly();
  	SDA(0);		//START
	sdly();

	SCL(0);
	sdly();
}

void I2C_STOP(void)
{
	SDA(0);
	sdly();

	SCL(1); 		//STOP
	sdly();

	SDA(1);

	sdly();
	sdly();
	sdly();
	sdly();
	sdly();
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
		sdly();
		SCL(1);
		sdly();
		SCL(0);
		sdly();
	}

	SDA(1);
	sdly();

	SCL(1);
	sdly();

/*
	if(SDA)
		UART_WSTR("No ACK!\r\n");
	else
		UART_WSTR("ACK!\r\n");
*/

	SCL(0);
	sdly();
	SDA(1);
	sdly();

	sdly();
	sdly();
	sdly();
	sdly();
	sdly();
}
void LCM_INIT(void)
{
//	unsigned char i;

	I2C_START();
	
	I2C_WRITE(0x78);//????????

    I2C_WRITE(0x26);//????/???? - Set TC    
    I2C_WRITE(0x2e);//Set power control   
 
    I2C_WRITE(0x81);//?????
    I2C_WRITE(0x70);//???? - Set Vbias  Potentiometer    
	 
/**************
	1000ab0c
	a - ??????,1=????????
	b - ??????,0=+1,1=-1
	c - ????
 **************/
    //I2C_WRITE(0x89);//set ram control

/**************
	0000abcd
	abcd - ????
 **************/
    I2C_WRITE(0x06);//????

/**************************
  	1100ab0c
	a - ????????
	b - ????????
	c - ????????
 **************************/
    I2C_WRITE(0xcc);

/**************
	01abcdef
	abcdef - ?????
 **************/
    I2C_WRITE(0x40);//set start line  

/**************
	10101111
	???
 **************/
    I2C_WRITE(0xaf);//set display enable    

/**************
	111010ab
	ab - ?????
 **************/
    I2C_WRITE(0xea);//set LCD bias ratio    

/**********************
  	1010010a
	a - ??????
 **********************/
	//I2C_WRITE(0xa5);//all pixel on   

/**********************
  	1010011a
	a - ??????
 **********************/
    //I2C_WRITE(0xa7);//??

	I2C_STOP();

/*

	I2C_START();

	I2C_WRITE(0x7a);	//24

	for(i=0;i<96;i++)
	{
	 	I2C_WRITE(i);
	}

	I2C_STOP();

	I2C_START();

	I2C_WRITE(0x7a);	//24

	I2C_WRITE(0x1f);
	I2C_WRITE(0x05);
	I2C_WRITE(0x07);
	I2C_WRITE(0x00);
	I2C_WRITE(0x1f);
	I2C_WRITE(0x04);
	I2C_WRITE(0x1f);
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(0));
	UART_WNUM(I2C_READ(1));

	I2C_STOP();
*/

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
		LCM_SC(6);
		I2C_START();
		I2C_WRITE(0x7a);
		for(j=0;j<96;j++)
		{
			I2C_WRITE(~*pic++);
		}
		I2C_STOP();
	}
}


