#ifndef __LCD2_H_
#define __LCD2_H_

void I2C_START(void);

void I2C_STOP(void);

void I2C_WRITE(unsigned char dat);

void LCM_INIT(void);

void LCM_SP(unsigned char page);

void LCM_SC(unsigned char col);
void LCM_PP(unsigned char *pic);

void sdly(void);

//void delay(unsigned long dly);
#endif
