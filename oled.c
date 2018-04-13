#include "oled.h"
#include "lpc11xx.h"
#include "oledfont.h"
static void delay(unsigned char num)
{
	uint8_t i = 10;
	while(num--)
	{
		while(i--);
	}
}

static void delay_ms(unsigned int ms)//延迟函数，MS级别
{
	unsigned int x,y;
	for(x = ms;x>0;x--)
 {
		for(y = 12000;y>0;y--);
 }
}


static void OLED_GPIO_Init(void)
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


static void OLED_IIC_Start(void)
{

	OLED_SCLK_Set();	//时钟线置高
	OLED_SDIN_Set();	//信号线置高
	delay(1);	//延迟1us
	OLED_SDIN_Clr();	//信号线置低
	delay(1);	//延迟1us
	OLED_SCLK_Clr();	//时钟线置低

}



static void OLED_IIC_Stop(void)
{
	OLED_SCLK_Clr();
	OLED_SDIN_Clr();	//信号线置低
	delay(1);	//延迟1us
	OLED_SCLK_Set();	//时钟线置高
	delay(1);	//延迟1us
	OLED_SDIN_Set();	//信号线置高

}


static unsigned char IIC_Wait_Ack(void)
{
	unsigned char ack;
  LPC_GPIO0->DIR &= ~(1 << 5);
	
	OLED_SCLK_Clr();	//时钟线置低
	delay(1);	//延迟1us
	OLED_SDIN_Set();	//信号线置高
	delay(1);	//延迟1us
	OLED_SCLK_Set();	//时钟线置高
	delay(1);	//延迟1us
  
	if(LPC_GPIO0->DATA&(1<<5))	//读取SDA的电平
		ack = IIC_NO_ACK;	//如果为1，则从机没有应答
	else
		ack = IIC_ACK;		//如果为0，则从机应答
  LPC_GPIO0->DIR |= 1 << 5;
	OLED_SCLK_Clr();//时钟线置低
	delay(1);	//延迟1us

	return ack;	//返回读取到的应答信息
}


static void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;  //定义变量
	for(i=0;i<8;i++) //for循环8次
	{
		OLED_SCLK_Clr();	
		//delay(1);	//延迟1us

		if(IIC_Byte & 0x80)	//读取最高位
		  	OLED_SDIN_Set();//最高位为1
		else
			OLED_SDIN_Clr();	//最高位为0

		IIC_Byte <<= 1;  //数据左移1位
		delay(1);	//延迟1us
		OLED_SCLK_Set(); //时钟线置高，产生上升沿，把数据发送出去
		delay(1);	//延迟1us
	}
	OLED_SCLK_Clr();	//时钟线置低
	delay(1);	//延迟1us

	while(IIC_Wait_Ack());	//从机应答
	
}

void LocateXY(unsigned char Page,unsigned char Col)   
{   
    Write_IIC_Command(0x30); //00110000  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=00 ; MY=0(COM0->COM66);    
                    //MX=1(SEG101->SEG0); PD=0    
    Write_IIC_Command(0x80|Col); //X Address = 0  Col    
    Write_IIC_Command(0x40|Page);//Y Address = 0  Page    
}   
   
//??(??)    
void LcdFill(unsigned char FillByte)   
{   
    unsigned char i,j;   
    LocateXY(0,0);   
    for(i=0;i<8;i++)   
    {   
        for(j=0;j<102;j++)   
        {   
            Write_IIC_Data(FillByte);   
        }   
    }   
}   


void Write_IIC_Command(unsigned char IIC_Command)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);//写入从机地址，SD0 = 0
   //Write_IIC_Byte(0x00);//写入命令
   Write_IIC_Byte(IIC_Command);//数据
   OLED_IIC_Stop();  //发送停止信号
}


void Write_IIC_Data(unsigned char IIC_Data)
{
   OLED_IIC_Start();
   Write_IIC_Byte(0x78);	//写入从机地址，SD0 = 0
  // Write_IIC_Byte(0x40);	//写入数据
   Write_IIC_Byte(IIC_Data);//数据
   OLED_IIC_Stop();		//发送停止信号
}

void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd) 
	  {
       Write_IIC_Data(dat); //写入数据
	  }
	else {
       Write_IIC_Command(dat); //写入命令
	}
}






void OLED_Init(void)
{
	OLED_GPIO_Init();	//GPIO口初始化
 
	delay_ms(200);	//延迟，由于单片机上电初始化比OLED快，所以必须加上延迟，等待OLED上复位完成

//	Write_IIC_Command(0x26);
//	Write_IIC_Command(0x2e);
//	Write_IIC_Command(0x81);
//	Write_IIC_Command(0x70);
//	Write_IIC_Command(0x06);
//	Write_IIC_Command(0xcc);
//	Write_IIC_Command(0x40);
//	Write_IIC_Command(0xaf);
//	Write_IIC_Command(0xea);
//	//Write_IIC_Command(0xa5);
//	//Write_IIC_Command(0x0C);
	Write_IIC_Command(0x33); //00110011  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=11 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    Write_IIC_Command(0x97); //10010111  1,0,0,1,BE1,BE0,PC1,PC0    
                    //BE1,BE0=10(DEFAULT);PC1,PC0=11-->x5    
       
    Write_IIC_Command(0x31); //00110001  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=01 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    Write_IIC_Command(0xEA); //11101010  1,VOP6,VOP5,VOP4,VOP3,VOP2,VOP1,VOP0    
                    //?????? Vop = 6.75+0.03*106 = 9.93 V    
    Write_IIC_Command(0x12); //00010010  0,0,0,1,0,BS2,BS1,BS0   bias = 1/9    
    Write_IIC_Command(0x40); //Start Line = com0    
       
    Write_IIC_Command(0x30); //00110000  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=00 ; MY=0(COM0->COM66);    
                    //MX=1(SEG101->SEG0); PD=0    
    Write_IIC_Command(0x80); //X Address = 0  (Col=0)    
    Write_IIC_Command(0x40); //Y Address = 0    
    Write_IIC_Command(0x05); //PRS=1,???????    
    Write_IIC_Command(0x0C); //D=1,E=0,Normal Display ON  

	

}  


/*
	@brief			OLED滚屏函数，范围0~1页，水平向左
	@param			无
	@retval			无
 */	

void PutBMP(unsigned char *puts)   
{   
    unsigned char i,j;   
    unsigned int  X=0;   
    for(i=0;i<8;i++)   
    {   
        LocateXY(i,0);   
        for(j=0;j<64;j++)   
        {   
            Write_IIC_Data(puts[X]);   
            X++;   
        }   
    }   
}
void PutDot(unsigned char DotByte)   
{   
    unsigned char i,j;   
    for(i=0;i<8;i++)   
    {   
        LocateXY(i,0);   
        for(j=0;j<64;j++)   
        {   
            Write_IIC_Data(DotByte);   
            DotByte=~DotByte;   
        }   
    }   
}   
void LCM_SP(unsigned char page)
{
	//OLED_IIC_Start();
	//I2C_WRITE(0x78);
	Write_IIC_Data(0xb0|page);
	//OLED_IIC_Stop();
}

void LCM_SC(unsigned char col)
{
	//OLED_IIC_Start();
	//I2C_WRITE(0x78);
	Write_IIC_Data(0x00|(col&0x0f));
	col>>=4;
	Write_IIC_Data(0x10|(col&0x0f));
	//OLED_IIC_Stop();
}

void LCM_PP(unsigned char *pic)
{
 	unsigned char i,j;


	for(i=0;i<8;i++)
	{
		LCM_SP(i);
		LCM_SC(6);
		//OLED_IIC_Start();
		Write_IIC_Data(0x7a);
		for(j=0;j<96;j++)
		{
			Write_IIC_Data(~*pic++);
		}
		//OLED_IIC_Stop();
	}
}
