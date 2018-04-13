#include "LPC11XX.h"

// Switch LED signal to output port with no pull up or pulldown
void LedOutputCfg(void)
{
  LPC_SYSCON->SYSAHBCLKCTRL = LPC_SYSCON->SYSAHBCLKCTRL | (1<<16) | (1<<6);

  LPC_IOCON->PIO1_9 = (0x0) + (0<<3) + (0<<5);

  // Set pin 9 as output
  LPC_GPIO1->DIR = LPC_GPIO1->DIR | (1<<9);

  return;
}

// Switch the CPU clock frequency to 48MHz
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

int main(void)
{
  int i;

  // Initialize LED output
  LedOutputCfg();

  while(1)
	{
    for (i=0;i<2000000;i++) 
		;

    // use P1.9 for LED output
    LPC_GPIO1->DATA = (0<<9); // Clear bit 9 
    
		for (i=0;i<2000000;i++) 
		;

		LPC_GPIO1->DATA = (1<<9); // Set bit 9 
  }
}
