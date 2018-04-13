#ifndef __LCD_H_
#define __LCD_H_


void WriteCommand(unsigned char CommandByte); 
void WriteData(unsigned char DataByte);
void Send8Bit(unsigned char Byte);
void CheckACK(void);  
void LcdInit(void); 
//void LocateXY(unsigned char Page,unsigned char Col);
//void LcdFill(unsigned char FillByte);
void PutDot(unsigned char DotByte);
void PutBMP(unsigned char *puts);
void LCD_Init(void);
void LCD_GPIO_Init(void);
void delay(unsigned char num);
void delay_ms(unsigned int ms);
void BL(unsigned char n);
void SDA(unsigned char n);
void SCL(unsigned char n);
#endif
