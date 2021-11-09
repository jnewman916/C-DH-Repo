/**********************************************************************//**
 * @file usbTest.c
 * 
 * Copyright 2008 Texas Instruments, Inc.
***************************************************************************/

extern char halUsbReceiveBuffer[];
extern unsigned char bufferSize;

/**********************************************************************//**
 * @brief  Executes the "USB-UART" menu option in the User Experience
 *         example code. 
 * 
 * Allows communication between the MSP430 UART and a computer serial 
 * COM port via a USB <> serial converter. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void usbTest( void )
{
  unsigned char quit;  
  volatile unsigned char i;

  halButtonsInterruptEnable( BUTTON_ALL );
  halUsbInit();
  halLcdClearScreen();
  halLcdPrintLine("baudrate = 57600", 0, 0);
  halLcdPrintLine("Type on PC      ", 1, 0);
  halLcdPrintLine("or Press Buttons", 2, 0);
  quit = 0; 
  
  while (quit == 0)
  {
    buttonsPressed = 0;
    
    __bis_SR_register(LPM3_bits + GIE);     
    __no_operation();         
        
    if (buttonsPressed & BUTTON_UP)
       halUsbSendChar('U');
    if (buttonsPressed & BUTTON_DOWN)
       halUsbSendChar('D');
    if (buttonsPressed & BUTTON_LEFT)
       halUsbSendChar('L');
    if (buttonsPressed & BUTTON_RIGHT)
       halUsbSendChar('R');
    if (buttonsPressed & BUTTON_SELECT)
    {
      halUsbSendChar('Q');
      halLcdClearScreen();
      quit =1 ;
    }
    if (buttonsPressed & BUTTON_S1)
      halUsbSendChar('1');

    if (buttonsPressed & BUTTON_S2)
      halUsbSendChar('2'); 

    if (bufferSize > 0)
    {       
      halLcdPrint(halUsbReceiveBuffer,  OVERWRITE_TEXT);       
      halLcdCursor();
      for (i = 0; i < bufferSize; i++)
        halUsbReceiveBuffer[i] = '\0';
      bufferSize  = 0;
    }
  }
  halUsbShutDown();
  halLcdCursorOff();
}
