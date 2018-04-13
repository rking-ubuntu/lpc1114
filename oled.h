#ifndef __OLED__H
#define __OLED__H			  	 


#include "lpc11xx.h"


/*--------------------���Ŷ���--------------------------*/
#define 		OLED_SCLK_Set()			LPC_GPIO0->DATA |= (1<<4)
#define			OLED_SCLK_Clr()			LPC_GPIO0->DATA &= ~(1<<4)
#define			OLED_SDIN_Set()			LPC_GPIO0->DATA |= (1<<5)
#define			OLED_SDIN_Clr()			LPC_GPIO0->DATA &= ~(1<<5)

		

/*definition--------------------------------------------*/
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define SIZE 		16		//��ʾ�ַ��Ĵ�С
#define Max_Column	128		//�������
#define Max_Row		64		//�������
#define X_WIDTH 	128		//X��Ŀ��
#define Y_WIDTH 	64	    //Y��Ŀ��					  				   
#define	IIC_ACK		0		//Ӧ��
#define	IIC_NO_ACK	1		//��Ӧ��




/*-----------------��������-----------------------------*/
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);	  //OLEDд�ֽں���
void OLED_Display_On(void);	//����ʾ����
void OLED_Display_Off(void);//����ʾ����
void OLED_Init(void);	//OLED��ʼ������
void OLED_GPIO_Init(void);	
void OLED_Clear(void);  //��������
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr); //��ʾ�ַ�����
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2); //��ָ����λ�ã���ʾһ��ָ�����ĳ��ȴ�С����
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);	 //��ָ��λ�ÿ�ʼ��ʾ�ַ�������
void OLED_Set_Pos(unsigned char x, unsigned char y);		//���㺯��
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no); //������ָ��λ����ʾ���ֺ���
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]); //��ʾͼƬ����
void OLED_Scroll(void);	//��������
//????    
void LocateXY(unsigned char Page,unsigned char Col); 
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data); 
void LcdFill(unsigned char FillByte);
void PutBMP(unsigned char *puts);
void PutDot(unsigned char DotByte); 
void LCM_SP(unsigned char page);

void LCM_SC(unsigned char col);

void LCM_PP(unsigned char *pic);
#endif  
	 



