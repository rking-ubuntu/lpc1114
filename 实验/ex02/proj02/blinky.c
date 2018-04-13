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
    LPC_IOCON->PIO1_9 &= ~0x07;              /* 将P1.9初始化为GPIO功能       */
    LPC_GPIO1->DIR    |= BEEP;               /* 将P1.9方向设置为输出         */
    LPC_GPIO1->DATA   |= BEEP;               /* 将P1.9初始化输出高电平       */

    LPC_IOCON->PIO0_1 &= ~0x07;              /* 初始化P0.1为GPIO功能         */
    LPC_GPIO0->DIR    &= ~KEY;               /* 将P0.1初始化为输入功能       */
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
