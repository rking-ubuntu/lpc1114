#include "LPC11XX.h"

void myDelay (uint32_t ulTime)
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

  return;
}

void ADCInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
    LPC_IOCON->R_PIO0_11 &= ~0xBF;                                      /* ??PIO0_11???????    */
    LPC_IOCON->R_PIO0_11 |=  0x02;                                      /* PIO0_11??????0         */

    LPC_SYSCON->PDRUNCFG &= ~(0x01 << 4);                               /* ADC????                  */
    LPC_SYSCON->SYSAHBCLKCTRL |=  (0x01 << 13);                         /* ??ADC????              */

    LPC_ADC->CR = ( 0x01 << 0 ) |                                       /* SEL=1,??ADC0               */
                  ((48000000 / 1000000 - 1) << 8 ) |             /* ????1MHz                 */
                  ( 0 << 16 ) |                                         /* ????????             */
                  ( 0 << 17 ) |                                         /* ??11 clocks??            */
                  ( 0 << 24 ) |                                         /* ADC????                  */
                  ( 0 << 27 );                                          /* ????ADC??,????    */
}

int main(void)
{
	uint32_t  i;
  uint32_t  ulADCData; 
  uint32_t  ulADCBuf;

	ADCInit();
	
  while (1) {
        ulADCData = 0;
        for (i = 0; i < 10; i++) {
            LPC_ADC->CR |= (1 << 24);                                   /* ????                     */
            while((LPC_ADC->DR[0] & 0x80000000) == 0);                  /* ??AD0DR0?Done             */
            LPC_ADC->CR |= (1 <<24);                                    /* ?????????           */
            while((LPC_ADC->DR[0] & 0x80000000) == 0);                  /* ??AD0DR0?Done             */
            ulADCBuf = LPC_ADC->DR[0];                                  /* ???????               */
            ulADCBuf = (ulADCBuf >> 6) & 0x3ff;
            ulADCData += ulADCBuf;
        }
        ulADCData = ulADCData / 10;                                     /* ??10???????         */
        ulADCData = (ulADCData * 3300) / 1024;
        
        myDelay(200);
      }
}
