#include "lcd.h"
#include "lpc11xx.h"
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
void LCD_Init(void)
{
	  WriteCommand(0x33); //00110011  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=11 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0x97); //10010111  1,0,0,1,BE1,BE0,PC1,PC0    
                    //BE1,BE0=10(DEFAULT);PC1,PC0=11-->x5    
       
    WriteCommand(0x31); //00110001  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=01 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0xEA); //11101010  1,VOP6,VOP5,VOP4,VOP3,VOP2,VOP1,VOP0    
                    //?????? Vop = 6.75+0.03*106 = 9.93 V    
    WriteCommand(0x12); //00010010  0,0,0,1,0,BS2,BS1,BS0   bias = 1/9    
    WriteCommand(0x40); //Start Line = com0    
       
    WriteCommand(0x30); //00110000  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=00 ; MY=0(COM0->COM66);    
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0x80); //X Address = 0  (Col=0)    
    WriteCommand(0x40); //Y Address = 0    
    WriteCommand(0x05); //PRS=1,???????    
    WriteCommand(0x0C); //D=1,E=0,Normal Display ON 
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
void WriteCommand(unsigned char CommandByte)   
{   
    //??I2C??    
    SDA(1);  
    SCL(1);
    delay(1);	
		SDA(0); 
		delay(1);	
    SCL(0);   
	  delay(1);
    //????????SLAVE Address    
    Send8Bit(0x78); //01111(SA1)(SA0)(RW)    
                //SA1=0 SA0=0 RW=0?    
    CheckACK();     //??ACK????    
    //??Control Byte    
    Send8Bit(0x00); //(Co)(A0)000000(A) Co=0 A0=0??    
                //Co=0???????????    
                //???Stop????????    
    CheckACK();   
    //????(???????,?????????????)    
    Send8Bit(CommandByte);   
    CheckACK();   
    //??I2C??    
    SCL(1);   
		    delay(1);
    SDA(0);
		    delay(1);
    SDA(1);    
		    delay(1);
}  

//?????    
void WriteData(unsigned char DataByte)   
{   
    //??I2C??    
    SDA(0); 
 delay(1);	
    SDA(1);   
	 delay(1);
    SCL(0); 
 delay(1);	
    //????????SLAVE Address    
    Send8Bit(0x78); //01111(SA1)(SA0)(RW)    
                //SA1=0 SA0=0 RW=0?    
    CheckACK();     //??ACK????    
    //??Control Byte    
    Send8Bit(0x40); //(Co)(A0)000000(A) Co=0 A0=1??    
                //Co=0???????????    
                //???Stop????????    
    CheckACK();   
    //????(???????Byte???????)    
    Send8Bit(DataByte);   
    CheckACK();   
    //??I2C??    
    SCL(1);  
	 delay(1);
    SDA(0); 
 delay(1);	
    SDA(1);
 delay(1);		
}  
//??8Bit??    
void Send8Bit(unsigned char Byte)   
{   
    unsigned char i;  
     	
    for(i=0;i<8;i++)   
    {    
		if(Byte>>(7-i)&0x01)
			SDA(1);
		else
			SDA(0);
		    delay(1);
        SCL(1);
		    delay(1);
        SCL(0);		
		    delay(1);
    }   
} 
void CheckACK(void)   
{   
	SCL(0);   
	    delay(1);
    SDA(1);  
	    delay(1);
    SCL(1); 
	    delay(1);
    LPC_GPIO0->DIR &= ~(1 << 5);  
    while(LPC_GPIO0->DATA&(1<<5));     //??SDA=High,Wait.....  
   LPC_GPIO0->DIR |= (1 << 5);   
    SCL(0);   
	    delay(1);
}   

//???    
void LcdInit(void)   
{   
    WriteCommand(0x33); //00110011  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=11 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0x97); //10010111  1,0,0,1,BE1,BE0,PC1,PC0    
                    //BE1,BE0=10(DEFAULT);PC1,PC0=11-->x5    
       
    WriteCommand(0x31); //00110001  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=01 ; MY=0(COM0->COM66);     
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0xEA); //11101010  1,VOP6,VOP5,VOP4,VOP3,VOP2,VOP1,VOP0    
                    //?????? Vop = 6.75+0.03*106 = 9.93 V    
    WriteCommand(0x12); //00010010  0,0,0,1,0,BS2,BS1,BS0   bias = 1/9    
    WriteCommand(0x40); //Start Line = com0    
       
    WriteCommand(0x30); //00110000  0,0,1,MX,MY,PD,H1,H0    
                    //SET H[1:0]=00 ; MY=0(COM0->COM66);    
                    //MX=1(SEG101->SEG0); PD=0    
    WriteCommand(0x80); //X Address = 0  (Col=0)    
    WriteCommand(0x40); //Y Address = 0    
    WriteCommand(0x05); //PRS=1,???????    
    WriteCommand(0x0C); //D=1,E=0,Normal Display ON    
}   
   
//????    
//void LocateXY(unsigned char Page,unsigned char Col)   
//{   
//    WriteCommand(0x30); //00110000  0,0,1,MX,MY,PD,H1,H0    
//                    //SET H[1:0]=00 ; MY=0(COM0->COM66);    
//                    //MX=1(SEG101->SEG0); PD=0    
//    WriteCommand(0x80|Col); //X Address = 0  Col    
//    WriteCommand(0x40|Page);//Y Address = 0  Page    
//}   
//   
////??(??)    
//void LcdFill(unsigned char FillByte)   
//{   
//    unsigned char i,j;   
//    LocateXY(0,0);   
//    for(i=0;i<8;i++)   
//    {   
//        for(j=0;j<102;j++)   
//        {   
//            WriteData(FillByte);   
//        }   
//    }   
//}   
   
//????    
//void PutDot(unsigned char DotByte)   
//{   
//    unsigned char i,j;   
//    for(i=0;i<8;i++)   
//    {   
//        LocateXY(i,0);   
//        for(j=0;j<64;j++)   
//        {   
//            WriteData(DotByte);   
//            DotByte=~DotByte;   
//        }   
//    }   
//}   
//   
////????    
//void PutBMP(unsigned char *puts)   
//{   
//    unsigned char i,j;   
//    unsigned int  X=0;   
//    for(i=0;i<8;i++)   
//    {   
//        LocateXY(i,0);   
//        for(j=0;j<64;j++)   
//        {   
//            //WriteData(puts[X]);   
//						WriteData(0xff);   
//            X++;   
//        }   
//    }   
//}   
   
