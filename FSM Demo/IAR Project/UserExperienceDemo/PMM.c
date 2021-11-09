/**********************************************************************//**
 * @file PMM.c 
 * 
 * Copyright 2008 Texas Instruments, Inc.
***************************************************************************/

#define VCORE_MENU_MAX 4
#define MCLK_MENU_MAX 7
static char VcoreText[]={
  "Vcore\0"  
  "1.35V\0"
  "1.55V\0"
  "1.75V\0"
  "1.85V\0"
};

static char MCLKText[]={
  "MCLK \0"  
  " 1Mhz\0"
  " 4Mhz\0"
  " 8Mhz\0"
  "12Mhz\0"  
  "16Mhz\0"  
  "20Mhz\0"  
  "25Mhz\0"  
};

static unsigned char MAX_MCLK_ALLOWED[]={4,5,6,7};
long dummyCalculation, wastingTime, wastingCycles;

// -------------- PMM and MCLK Menu---------------------------------
/**********************************************************************//**
 * @brief  Executes the "PMM-MCLK" menu option in the User Experience
 *         example code. This menu option allows one to change the frequency
 *         of operation for the MSP430 and the VCore setting. 
 * 
 * @param  none 
 * 
 * @return none 
 *************************************************************************/
void menuPMMMCLK( void )
{
  unsigned char menuLeftPos = 2, menuRightPos = 0, menuRightMaxAllowed = 5;
  unsigned char ledOn, quit = 0;
  volatile unsigned int i;
  
  halButtonsInterruptDisable( BUTTON_ALL );
  halButtonsInterruptEnable( BUTTON_SELECT + BUTTON_S1 +   \
                             BUTTON_S2 + BUTTON_RIGHT );
  halAccelerometerShutDown();
  halLcdClearScreen();
  
  halBoardOutputSystemClock();
  halLcdPrintLineCol(&VcoreText[0], 0, 1,  OVERWRITE_TEXT );

  for (i=0;i < MCLK_MENU_MAX+1; i++)
    halLcdPrintLineCol(&MCLKText[i*6], i, 12,  OVERWRITE_TEXT );
  
  halLcdPrintLineCol(&VcoreText[menuLeftPos*6 + 6],        \
                     menuLeftPos+1, 1, INVERT_TEXT | OVERWRITE_TEXT);
  halLcdPrintLineCol(&MCLKText[menuRightPos*6 + 6],        \
                     menuRightPos+1, 12, INVERT_TEXT | OVERWRITE_TEXT);    
  buttonsPressed = 0;  
  
  for (i = menuRightMaxAllowed; i < MCLK_MENU_MAX; i++)
  {
    halLcdLine(96, 12*(i+1)+6, 136, 12*(i+1)+6, PIXEL_ON);        
    halLcdLine(96, 12*(i+1)+7, 136, 12*(i+1)+7, PIXEL_ON);     
  }
  ledOn = 0;
  halLcdPrintLine("      LED", 6, 0);  
  halLcdPrintLine("      OFF", 7, 0);
  halLcdImage(IMG_RIGHT_FILLED, 4, 32, 10, 75);
  
  halBoardSetVCore(menuLeftPos);
  halBoardSetSystemClock(menuRightPos);  

  while (!quit)
  {
  	// The LED can be enabled to show the relative difference between
  	// frequencies of operation. 
    while (!buttonsPressed)    
      if (ledOn)
      {
        LED_PORT_OUT ^= LED_1;
        for (i=0; i < 0xFFFF; i++)
          if (buttonsPressed)
            break;
      }      
      else
      {
      	// To emulate a real application instead of continuous jumps, use nops 
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();                           
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();                           
        __no_operation();
      }
    if (buttonsPressed & BUTTON_S1)
    {
      /*
       * Disabled for MSP430F5438 RTM Silicon
      halLcdPrintLineCol(&VcoreText[menuLeftPos*6 + 6], menuLeftPos+1,      \
                         1,  OVERWRITE_TEXT);
      if (++ menuLeftPos >= VCORE_MENU_MAX)
      {
        menuLeftPos = 0;
        menuRightMaxAllowed = MAX_MCLK_ALLOWED[ menuLeftPos ];
        if (menuRightPos >= menuRightMaxAllowed)
        {
          halLcdPrintLineCol(&MCLKText[menuRightPos*6 + 6], menuRightPos+1, \
                             12,  OVERWRITE_TEXT);                
          menuRightPos = menuRightMaxAllowed - 1;
          halLcdPrintLineCol(&MCLKText[menuRightPos*6 + 6], menuRightPos+1, \
                             12, INVERT_TEXT | OVERWRITE_TEXT);  
        }        
        for (i = menuRightMaxAllowed; i < MCLK_MENU_MAX; i++)
        {
          halLcdLine(96, 12*(i+1)+6, 136, 12*(i+1)+6, PIXEL_ON);        
          halLcdLine(96, 12*(i+1)+7, 136, 12*(i+1)+7, PIXEL_ON);        
        }
      } 
         
      else            
      {
        for (i=menuRightMaxAllowed+1; i< MAX_MCLK_ALLOWED[menuLeftPos]+1; i ++)
          halLcdPrintLineCol(&MCLKText[i*6], i, 12,  OVERWRITE_TEXT );          
        menuRightMaxAllowed = MAX_MCLK_ALLOWED[menuLeftPos];
      }      
      halLcdPrintLineCol(&VcoreText[menuLeftPos*6 + 6], menuLeftPos+1,       \
                         1, INVERT_TEXT | OVERWRITE_TEXT);
      */
    }
    if ( buttonsPressed & BUTTON_S2 )
    {
      halLcdPrintLineCol(&MCLKText[menuRightPos*6 + 6],    \
                         menuRightPos+1, 12,  OVERWRITE_TEXT);        
      
      if ( ++menuRightPos >= menuRightMaxAllowed )      
        menuRightPos = 0;
        
      halLcdPrintLineCol(&MCLKText[menuRightPos*6 + 6],    \
                         menuRightPos+1, 12, INVERT_TEXT| OVERWRITE_TEXT);              
    }
    if ( buttonsPressed & BUTTON_RIGHT )
    {
      ledOn = 1 - ledOn;
      if ( ledOn )        
        halLcdPrintLineCol("ON ", 7, 6, OVERWRITE_TEXT);
      else
      {
      	halLcdPrintLineCol("OFF", 7, 6, OVERWRITE_TEXT);
        LED_PORT_OUT &= ~LED_1;
      }         
    }    
    
    if ( buttonsPressed & (BUTTON_S1 | BUTTON_S2) )
    {
      halBoardSetVCore(menuLeftPos);
  		halBoardSetSystemClock(menuRightPos);  
  		//halBoardDisableSVS();
    }
    if ( buttonsPressed & BUTTON_SELECT )    
      quit = 1;
    buttonsPressed = 0;
  }
  
  halBoardSetSystemClock( SYSCLK_16MHZ );
  halBoardStopOutputSystemClock();
  LED_PORT_OUT &= ~LED_1;
}
