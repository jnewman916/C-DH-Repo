//**********************************************************************//******
//  MSP-EXP430F5438 Experimenter's Board - User Experience Demo
//  D. Dang
//  Texas Instruments Inc.
//  Ver 1.00 - May 2008
//  Ver 1.10 - Jan 2009
//  * Review, documentation of code
//  * Removal of code for production test
//  * Additional support for MSP430EXP5438 Rev 1.x
//  Built with Code Composer Essentials CCE v3.2
//**********************************************************************//******
#include  <msp430.h>
#include "MSP-EXP430F5438 HAL\hal_MSP-EXP430F5438.h"
extern void UserExperience( void );

int __low_level_init(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  return(0);
}

void main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  UserExperience();
}
