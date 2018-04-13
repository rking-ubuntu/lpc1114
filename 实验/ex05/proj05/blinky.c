#include "LPC11XX.h"

#define UART_BPS    115200

void myDelay(uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
      for (i = 0; i < 5000; i++);
    }
}

void SystemInit(void)
{
  LPC_SYSCON->PDRUNCFG = LPC_SYSCON->PDRUNCFG & 0xFFFFFF5F;

  LPC_SYSCON->SYSPLLCLKSEL = 1;

  LPC_SYSCON->SYSPLLCLKUEN = 0;
  LPC_SYSCON->SYSPLLCLKUEN = 1;

  LPC_SYSCON->SYSPLLCTRL = (3 + (1<<5)); // M = 4, P = 2
	
  while(LPC_SYSCON->SYSPLLSTAT == 0);

  LPC_SYSCON->MAINCLKSEL = 3;

  LPC_SYSCON->MAINCLKUEN = 0;
  LPC_SYSCON->MAINCLKUEN = 1;
}

void uartInit (void)
{
    uint16_t usFdiv;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
    LPC_IOCON->PIO1_6 |= 0x01;
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);
    LPC_SYSCON->UARTCLKDIV       = 0x01;
    
    LPC_UART->LCR  = 0x83;
    usFdiv = (48000000/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256; 
    LPC_UART->LCR  = 0x03;
    LPC_UART->FCR  = 0x07;
}

uint8_t uartGetByte (void)
{
    uint8_t ucRcvData;
    
    while ((LPC_UART->LSR & 0x01) == 0);
    ucRcvData = LPC_UART->RBR;
    return (ucRcvData);   
}

void uartSendByte (uint8_t ucDat)
{
    LPC_UART->THR = ucDat;
    while ((LPC_UART->LSR & 0x40) == 0);
}

int main(void)
{
  uint8_t c;
	
  uartInit(); 

  while(1)
	{
		c = uartGetByte();
		myDelay(400);
		uartSendByte(c+1);
  }
}
