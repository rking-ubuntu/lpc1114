#include "LPC11XX.h"

/* �궨�� */
#define BEEP            (1ul << 9)         /* BEEP����PIOX_7 */
#define KEY             (1ul << 1)         /* ��������PIO0_1 */

#define BEEPOFF()        LPC_GPIO1->DATA |= BEEP                        /* BEEP�� */
#define BEEPON()         LPC_GPIO1->DATA &= (~BEEP)                     /* BEEP�� */

void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}

// Switch LED signal to output port with no pull up or pulldown
void LedOutputCfg(void)
{
  LPC_SYSCON->SYSAHBCLKCTRL = LPC_SYSCON->SYSAHBCLKCTRL | (1<<16) | (1<<6);

  LPC_IOCON->PIO1_9 = (0x0) + (0<<3) + (0<<5);

  // Set pin 1_9 as output
  LPC_GPIO1->DIR = LPC_GPIO1->DIR | (1<<9);

  return;
}

void KEYInit( void )
{
    LPC_IOCON->PIO0_1 &= (~0x07);      /* ��P0.1��ʼ��ΪGPIO����       */
    LPC_GPIO0->DIR    &= (~KEY);       /* ����P0.1Ϊ����               */
    LPC_GPIO0->IS      = 0x00;         /* P0.1Ϊ�����ж�               */
    LPC_GPIO0->IEV    |= KEY;          /* �������ж�                   */
    LPC_GPIO0->IE     |= KEY;          /* P0.1�жϲ�����               */
    NVIC_EnableIRQ(EINT0_IRQn);
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

void PIOINT0_IRQHandler(void)
{
    BEEPON();    
    myDelay(100);
    BEEPOFF();
    myDelay(100);
	  BEEPON();    
    myDelay(100);
    BEEPOFF();
    myDelay(100);
	  LPC_GPIO0->IC |= KEY;          /* �˾�Ҫ�ŵ��жϴ����˳�ǰ */
}

int main(void)
{
  // Initialize LED output
  LedOutputCfg();
	KEYInit();
	
  while(1)
	{
  }
}
