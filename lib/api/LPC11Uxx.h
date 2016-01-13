
/****************************************************************************************************//**
 * @file     LPC11Uxx.h
 *
 *
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           default LPC11Uxx Device Series
 *
 * @version  V0.1
 * @date     21. March 2011
 *
 * @note     Generated with SFDGen V2.6 Build 3j (beta) on Thursday, 17.03.2011 13:19:45
 *
 *           from CMSIS SVD File 'LPC11U1x_svd.xml' Version 0.1,
 *           created on Wednesday, 16.03.2011 20:30:42, last modified on Thursday, 17.03.2011 20:19:40
 *
 *******************************************************************************************************/

// ################################################################################
// Minor fix 8 April 2011 - changed LPC_CT32B1_BASE from 0x40014000 to 0x40018000
// ################################################################################

/** @addtogroup NXP
  * @{
  */

/** @addtogroup LPC11Uxx
  * @{
  */

#ifndef __LPC11UXX_H__
#define __LPC11UXX_H__

#ifdef __cplusplus
extern "C" {
#endif 


#if defined ( __CC_ARM   )
  #pragma anon_unions
#endif

 /* Interrupt Number Definition */

typedef enum {
// -------------------------  Cortex-M0 Processor Exceptions Numbers  -----------------------------
  Reset_IRQn                        = -15,  /*!<   1  Reset Vector, invoked on Power up and warm reset */
  NonMaskableInt_IRQn               = -14,  /*!<   2  Non maskable Interrupt, cannot be stopped or preempted */
  HardFault_IRQn                    = -13,  /*!<   3  Hard Fault, all classes of Fault */
  SVCall_IRQn                       = -5,   /*!<  11  System Service Call via SVC instruction */
  DebugMonitor_IRQn                 = -4,   /*!<  12  Debug Monitor                    */
  PendSV_IRQn                       = -2,   /*!<  14  Pendable request for system service */
  SysTick_IRQn                      = -1,   /*!<  15  System Tick Timer                */
// ---------------------------  LPC11Uxx Specific Interrupt Numbers  ------------------------------
FLEX_INT0_IRQn                = 0,        /*!< All I/O pins can be routed to below 8 interrupts. */
  FLEX_INT1_IRQn                = 1,
  FLEX_INT2_IRQn                = 2,
  FLEX_INT3_IRQn                = 3,
  FLEX_INT4_IRQn                = 4,   
  FLEX_INT5_IRQn                = 5,        
  FLEX_INT6_IRQn                = 6,        
  FLEX_INT7_IRQn                = 7,        
  GINT0_IRQn                    = 8,        /*!< Grouped Interrupt 0                              */
  GINT1_IRQn                    = 9,        /*!< Grouped Interrupt 1                              */
  Reserved0_IRQn                = 10,       /*!< Reserved Interrupt                               */
  Reserved1_IRQn                = 11,       
  Reserved2_IRQn                = 12,       
  Reserved3_IRQn                = 13,       
  SSP1_IRQn                     = 14,       /*!< SSP1 Interrupt                                   */
  I2C_IRQn                      = 15,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 16,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 17,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 18,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 19,       /*!< 32-bit Timer1 Interrupt                          */
  SSP0_IRQn                     = 20,       /*!< SSP0 Interrupt                                   */
  UART_IRQn                     = 21,       /*!< UART Interrupt                                   */
  USB_IRQn                      = 22,       /*!< USB IRQ Interrupt                                */
  USB_FIQn                      = 23,       /*!< USB FIQ Interrupt                                */
  ADC_IRQn                      = 24,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 25,       /*!< Watchdog timer Interrupt                         */  
  BOD_IRQn                      = 26,       /*!< Brown Out Detect(BOD) Interrupt                  */
  FMC_IRQn                      = 27,       /*!< Flash Memory Controller Interrupt                */
  Reserved4_IRQn                = 28,       /*!< Reserved Interrupt                               */
  Reserved5_IRQn                = 29,       /*!< Reserved Interrupt                               */
  USBWakeup_IRQn                = 30,       /*!< USB wakeup Interrupt                             */
  Reserved6_IRQn                = 31,       /*!< Reserved Interrupt                               */
} IRQn_Type;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/* Processor and Core Peripheral Section */ /* Configuration of the Cortex-M0 Processor and Core Peripherals */

#define __MPU_PRESENT             0         /*!< MPU present or not                    */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used */
/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm0.h"                       /*!< Cortex-M0 processor and core peripherals */
#include "system_LPC11Uxx.h"                /*!< LPC11Uxx System                       */

/** @addtogroup Device_Peripheral_Registers
  * @{
  */


// ------------------------------------------------------------------------------------------------
// -----                                          I2C                                         -----
// ------------------------------------------------------------------------------------------------


/**
  * @brief Product name title=UM10462 Chapter title=LPC11U1x I2C-bus controller Modification date=3/16/2011 Major revision=0 Minor revision=3  (I2C)
  */

typedef struct {                            /*!< (@ 0x40000000) I2C Structure          */
  __IO uint32_t CONSET;                     /*!< (@ 0x40000000) I2C Control Set Register */
  __I  uint32_t STAT;                       /*!< (@ 0x40000004) I2C Status Register */
  __IO uint32_t DAT;                        /*!< (@ 0x40000008) I2C Data Register.  */
  __IO uint32_t ADR0;                       /*!< (@ 0x4000000C) I2C Slave Address Register 0 */
  __IO uint32_t SCLH;                       /*!< (@ 0x40000010) SCH Duty Cycle Register High Half Word */
  __IO uint32_t SCLL;                       /*!< (@ 0x40000014) SCL Duty Cycle Register Low Half Word */
  __IO uint32_t CONCLR;                     /*!< (@ 0x40000018) I2C Control Clear Register*/
  __IO uint32_t MMCTRL;                     /*!< (@ 0x4000001C) Monitor mode control register*/
  __IO uint32_t ADR1;                       /*!< (@ 0x40000020) I2C Slave Address Register 1*/
  __IO uint32_t ADR2;                       /*!< (@ 0x40000024) I2C Slave Address Register 2*/
  __IO uint32_t ADR3;                       /*!< (@ 0x40000028) I2C Slave Address Register 3*/
  __I  uint32_t DATA_BUFFER;                /*!< (@ 0x4000002C) Data buffer register */
union{
  __IO uint32_t MASK[4];                    /*!< (@ 0x40000030) I2C Slave address mask register */
  struct{
  __IO uint32_t MASK0;
  __IO uint32_t MASK1;
  __IO uint32_t MASK2;
  __IO uint32_t MASK3;
  };
  };
} LPC_I2C_Type;

// ------------------------------------------------------------------------------------------------
// -----                                        SYSCON                                        -----
// ------------------------------------------------------------------------------------------------


/**
  * @brief Product name title=UM10462 Chapter title=LPC11U1x System control block Modification date=3/16/2011 Major revision=0 Minor revision=3  (SYSCON)
  */

typedef struct {                            /*!< (@ 0x40048000) SYSCON Structure       */
  __IO uint32_t SYSMEMREMAP;                /*!< (@ 0x40048000) System memory remap    */
  __IO uint32_t PRESETCTRL;                 /*!< (@ 0x40048004) Peripheral reset control */
  __IO uint32_t SYSPLLCTRL;                 /*!< (@ 0x40048008) System PLL control     */
  __I  uint32_t SYSPLLSTAT;                 /*!< (@ 0x4004800C) System PLL status      */
  __IO uint32_t USBPLLCTRL;                 /*!< (@ 0x40048010) USB PLL control        */
  __I  uint32_t USBPLLSTAT;                 /*!< (@ 0x40048014) USB PLL status         */
  __I  uint32_t RESERVED0[2];
  __IO uint32_t SYSOSCCTRL;                 /*!< (@ 0x40048020) System oscillator control */
  __IO uint32_t WDTOSCCTRL;                 /*!< (@ 0x40048024) Watchdog oscillator control */
  __I  uint32_t RESERVED1[2];
  __IO uint32_t SYSRSTSTAT;                 /*!< (@ 0x40048030) System reset status register */
  __I  uint32_t RESERVED2[3];
  __IO uint32_t SYSPLLCLKSEL;               /*!< (@ 0x40048040) System PLL clock source select */
  __IO uint32_t SYSPLLCLKUEN;               /*!< (@ 0x40048044) System PLL clock source update enable */
  __IO uint32_t USBPLLCLKSEL;               /*!< (@ 0x40048048) USB PLL clock source select */
  __IO uint32_t USBPLLCLKUEN;               /*!< (@ 0x4004804C) USB PLL clock source update enable */
  __I  uint32_t RESERVED3[8];
  __IO uint32_t MAINCLKSEL;                 /*!< (@ 0x40048070) Main clock source select */
  __IO uint32_t MAINCLKUEN;                 /*!< (@ 0x40048074) Main clock source update enable */
  __IO uint32_t SYSAHBCLKDIV;               /*!< (@ 0x40048078) System clock divider   */
  __I  uint32_t RESERVED4[1];
  __IO uint32_t SYSAHBCLKCTRL;              /*!< (@ 0x40048080) System clock control   */
  __I  uint32_t RESERVED5[4];
  __IO uint32_t SSP0CLKDIV;                 /*!< (@ 0x40048094) SSP0 clock divider     */
  __IO uint32_t UARTCLKDIV;                 /*!< (@ 0x40048098) UART clock divider     */
  __IO uint32_t SSP1CLKDIV;                 /*!< (@ 0x4004809C) SSP1 clock divider     */
  __I  uint32_t RESERVED6[8];
  __IO uint32_t USBCLKSEL;                  /*!< (@ 0x400480C0) USB clock source select */
  __IO uint32_t USBCLKUEN;                  /*!< (@ 0x400480C4) USB clock source update enable */
  __IO uint32_t USBCLKDIV;                  /*!< (@ 0x400480C8) USB clock source divider */
  __I  uint32_t RESERVED7[5];
  __IO uint32_t CLKOUTSEL;                  /*!< (@ 0x400480E0) CLKOUT clock source select */
  __IO uint32_t CLKOUTUEN;                  /*!< (@ 0x400480E4) CLKOUT clock source update enable */
  __IO uint32_t CLKOUTDIV;                  /*!< (@ 0x400480E8) CLKOUT clock divider   */
  __I  uint32_t RESERVED8[5];
  __I  uint32_t PIOPORCAP0;                 /*!< (@ 0x40048100) POR captured PIO status 0 */
  __I  uint32_t PIOPORCAP1;                 /*!< (@ 0x40048104) POR captured PIO status 1 */
  __I  uint32_t RESERVED9[18];
  __IO uint32_t BODCTRL;                    /*!< (@ 0x40048150) Brown-Out Detect       */
  __IO uint32_t SYSTCKCAL;                  /*!< (@ 0x40048154) System tick counter calibration */
  __I  uint32_t RESERVED10[6];
  __IO uint32_t IRQLATENCY;                 /*!< (@ 0x40048170) IQR delay */
  __IO uint32_t NMISRC;                     /*!< (@ 0x40048174) NMI Source Control     */
  __IO uint32_t PINTSEL[8];                 /*!< (@ 0x40048178) GPIO Pin Interrupt Select register 0 */
  __IO uint32_t USBCLKCTRL;                 /*!< (@ 0x40048198) USB clock control      */
  __I  uint32_t USBCLKST;                   /*!< (@ 0x4004819C) USB clock status       */
  __I  uint32_t RESERVED11[25];
  __IO uint32_t STARTERP0;                  /*!< (@ 0x40048204) Start logic 0 interrupt wake-up enable register 0 */
  __I  uint32_t RESERVED12[3];
  __IO uint32_t STARTERP1;                  /*!< (@ 0x40048214) Start logic 1 interrupt wake-up enable register 1 */
  __I  uint32_t RESERVED13[6];
  __IO uint32_t PDSLEEPCFG;                 /*!< (@ 0x40048230) Power-down states in deep-sleep mode */
  __IO uint32_t PDAWAKECFG;                 /*!< (@ 0x40048234) Power-down states for wake-up from deep-sleep */
  __IO uint32_t PDRUNCFG;                   /*!< (@ 0x40048238) Power configuration register */
  __I  uint32_t RESERVED14[110];
  __I  uint32_t DEVICE_ID;                  /*!< (@ 0x400483F4) Device ID              */
} LPC_SYSCON_Type;


#if defined ( __CC_ARM   )
  #pragma no_anon_unions
#endif


// ------------------------------------------------------------------------------------------------
// -----                                 Peripheral memory map                                -----
// ------------------------------------------------------------------------------------------------

#define LPC_I2C_BASE              (0x40000000)
#define LPC_IOCON_BASE            (0x40044000)
#define LPC_SYSCON_BASE           (0x40048000)
#define LPC_IOCON0_BASE           (LPC_IOCON_BASE)
#define LPC_IOCON1_BASE           (LPC_IOCON_BASE + 0x60)


// ------------------------------------------------------------------------------------------------
// -----                                Peripheral declaration                                -----
// ------------------------------------------------------------------------------------------------

#define LPC_I2C                   ((LPC_I2C_Type            *) LPC_I2C_BASE)
#define LPC_SYSCON                ((LPC_SYSCON_Type         *) LPC_SYSCON_BASE)


/** @} */ /* End of group Device_Peripheral_Registers */
/** @} */ /* End of group (null) */
/** @} */ /* End of group LPC11Uxx */

#ifdef __cplusplus
}
#endif 


#endif  // __LPC11UXX_H__
