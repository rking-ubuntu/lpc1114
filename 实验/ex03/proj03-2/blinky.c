#include "LPC11XX.h"

void SystemInit(void)
{
  LPC_SYSCON->PDRUNCFG = LPC_SYSCON->PDRUNCFG & 0xFFFFFF5F;

  // Select PLL source as crystal oscillator
  LPC_SYSCON->SYSPLLCLKSEL = 1;
  // Update SYSPLL setting (0->1 sequence)
  LPC_SYSCON->SYSPLLCLKUEN = 0;
  LPC_SYSCON->SYSPLLCLKUEN = 1;

  LPC_SYSCON->SYSPLLCTRL = (3 + (1<<5)); // M = 4, P = 2
  // wait until PLL is locked
  while(LPC_SYSCON->SYSPLLSTAT == 0);

  LPC_SYSCON->MAINCLKSEL = 3;
  // Update Main Clock Select setting (0->1 sequence)
  LPC_SYSCON->MAINCLKUEN = 0;
  LPC_SYSCON->MAINCLKUEN = 1;

  return;
}

void timer1Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
    LPC_IOCON->PIO1_9 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);

    LPC_TMR16B1->PR      = 47999;
    LPC_TMR16B1->MCR     = 2;
    LPC_TMR16B1->EMR     = (0x03 << 4);
    LPC_TMR16B1->MR0     = 500;
    LPC_TMR16B1->TCR     = 0x01;
}

int main(void)
{
	timer1Init();
	
  while(1)
	;
}

