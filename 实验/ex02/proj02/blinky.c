#include "LPC11XX.h"

#define BEEP            (1ul << 9)
#define KEY             (1ul << 1)

void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}

void GPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
    LPC_IOCON->PIO1_9 &= ~0x07;              /* ��P1.9��ʼ��ΪGPIO����       */
    LPC_GPIO1->DIR    |= BEEP;               /* ��P1.9��������Ϊ���         */
    LPC_GPIO1->DATA   |= BEEP;               /* ��P1.9��ʼ������ߵ�ƽ       */

    LPC_IOCON->PIO0_1 &= ~0x07;              /* ��ʼ��P0.1ΪGPIO����         */
    LPC_GPIO0->DIR    &= ~KEY;               /* ��P0.1��ʼ��Ϊ���빦��       */
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
  GPIOInit();
  while (1) {
        if ((LPC_GPIO0->DATA & KEY) == 0) {
             LPC_GPIO1->DATA &= (~BEEP);
             myDelay(400); 
             LPC_GPIO1->DATA |= BEEP;
             myDelay(400);
        }
    }
}
