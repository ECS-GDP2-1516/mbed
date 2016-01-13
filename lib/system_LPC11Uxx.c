/******************************************************************************
 * @file     system_LPC11Uxx.c
 * @purpose  CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the NXP LPC13xx Device Series
 * @version  V1.10
 * @date     24. November 2010
 *
 * @note
 * Copyright (C) 2009-2010 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#include "LPC11Uxx.h"

__attribute__((always_inline)) static inline void __NOP(void)
{
  __asm volatile ("nop");
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void) {
  volatile uint32_t i;

#if (CLOCK_SETUP)                                 /* Clock Setup              */

#if ((SYSPLLCLKSEL_Val & 0x03) == 1)
  LPC_SYSCON->PDRUNCFG     &= ~(1 << 5);          /* Power-up System Osc      */
  LPC_SYSCON->SYSOSCCTRL    = SYSOSCCTRL_Val;
  for (i = 0; i < 200; i++) __NOP();
#endif

  LPC_SYSCON->SYSPLLCLKSEL  = SYSPLLCLKSEL_Val;   /* Select PLL Input         */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;               /* Update Clock Source      */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x00;               /* Toggle Update Register   */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));     /* Wait Until Updated       */
#if ((MAINCLKSEL_Val & 0x03) == 3)                /* Main Clock is PLL Out    */
  LPC_SYSCON->SYSPLLCTRL    = SYSPLLCTRL_Val;
  LPC_SYSCON->PDRUNCFG     &= ~(1 << 7);          /* Power-up SYSPLL          */
  while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));	      /* Wait Until PLL Locked    */
#endif

#if (((MAINCLKSEL_Val & 0x03) == 2) )
  LPC_SYSCON->WDTOSCCTRL    = WDTOSCCTRL_Val;
  LPC_SYSCON->PDRUNCFG     &= ~(1 << 6);          /* Power-up WDT Clock       */
  for (i = 0; i < 200; i++) __NOP();
#endif

  LPC_SYSCON->MAINCLKSEL    = MAINCLKSEL_Val;     /* Select PLL Clock Output  */
  LPC_SYSCON->MAINCLKUEN    = 0x01;               /* Update MCLK Clock Source */
  LPC_SYSCON->MAINCLKUEN    = 0x00;               /* Toggle Update Register   */
  LPC_SYSCON->MAINCLKUEN    = 0x01;
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01));       /* Wait Until Updated       */

  LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;

#if ((USBCLKDIV_Val & 0x1FF) != 0)                /* USB clock is used        */
  LPC_SYSCON->PDRUNCFG     &= ~(1 << 10);         /* Power-up USB PHY         */

#if ((USBCLKSEL_Val & 0x003) == 0)                /* USB clock is USB PLL out */
  LPC_SYSCON->PDRUNCFG     &= ~(1 <<  8);         /* Power-up USB PLL         */
  LPC_SYSCON->USBPLLCLKSEL  = USBPLLCLKSEL_Val;   /* Select PLL Input         */
  LPC_SYSCON->USBPLLCLKUEN  = 0x01;               /* Update Clock Source      */
  LPC_SYSCON->USBPLLCLKUEN  = 0x00;               /* Toggle Update Register   */
  LPC_SYSCON->USBPLLCLKUEN  = 0x01;
  while (!(LPC_SYSCON->USBPLLCLKUEN & 0x01));     /* Wait Until Updated       */
  LPC_SYSCON->USBPLLCTRL    = USBPLLCTRL_Val;
  while (!(LPC_SYSCON->USBPLLSTAT   & 0x01));     /* Wait Until PLL Locked    */
  LPC_SYSCON->USBCLKSEL     = 0x00;               /* Select USB PLL           */
#endif

  LPC_SYSCON->USBCLKSEL     = USBCLKSEL_Val;      /* Select USB Clock         */
  LPC_SYSCON->USBCLKDIV     = USBCLKDIV_Val;      /* Set USB clock divider    */

#else                                             /* USB clock is not used    */                        
  LPC_SYSCON->PDRUNCFG     |=  (1 << 10);         /* Power-down USB PHY       */
  LPC_SYSCON->PDRUNCFG     |=  (1 <<  8);         /* Power-down USB PLL       */
#endif

#endif

  /* System clock to the IOCON needs to be enabled or
  most of the I/O related peripherals won't work. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
}