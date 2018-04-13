#ifndef __I2C_H_
#define __I2C_H_
#include "lpc11xx.h"
void delay_ms(unsigned int ms);
void delay(unsigned char num);
void BL(unsigned char n);
void SDA(unsigned char n);
void SCL(unsigned char n);
void LCD_GPIO_Init(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_WRITE(unsigned char dat);
void LCM_INIT(void);
void LCM_SP(unsigned char page);
void LCM_SC(unsigned char col);
void LCM_PP(unsigned char *pic);
void CLEAR(void);
void DisplayXY(unsigned char x,unsigned char y);
void UndisplayXY(unsigned char x,unsigned char y);
#endif
