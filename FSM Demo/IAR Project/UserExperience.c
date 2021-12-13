//**********************************************************************//*****
//  MSP-EXP430F5438 Experimenter's Board - User Experience Demo
//
//  Main Project File: UserExperience.c
//
//  D. Dang
//  Texas Instruments Inc.
//  Ver 1.00 - May 2008
//  Built with Code Composer Essentials CCE v3.2
//*****************************************************************************
#include  <msp430.h>
#include "MSP-EXP430F5438 HAL\hal_MSP-EXP430F5438.h"
#include "UserExperienceDemo\UserExperienceGraphics.h"
#include "UserExperienceDemo\flashUtils.h"

//--Calibration constants and user configuration values stored in INFOB Flash--
#pragma dataseg=INFOB
__no_init unsigned char boardMode;
__no_init unsigned char lcdBackLightLevelSetting;
__no_init unsigned long lastAudioByteFlash;
__no_init unsigned char lcdContrastSetting;
__no_init unsigned char temperatureConversion;
__no_init long temperatureCalibrationC;
__no_init unsigned char wakeUpOnAcc;
__no_init int  Acc_X_Calibrated_Offset;
__no_init int  Acc_Y_Calibrated_Offset;
__no_init int  Acc_Z_Calibrated_Offset;
#pragma dataseg=default

#define TIME_OUT          10
#define TIME_OUT2         3
#define MENU_MAX          7
#define SETTING_MENU_MAX  6
#define MENU_ITEM_WIDTH   14

enum{  LPM4_MODE, LPM3_MODE, ACTIVE_MODE, APPLICATION_MODE } ;
enum{  APP_CLOCK, APP_BAL_BALL, APP_USB, APP_AUDIO,  PMM_MCLK, PUZZLE,MENU_SETTING};
enum{  SET_TIME, SET_CONTRAST, SET_BACKLIGHT, SET_TEMPERATURE_CONVERSION, 
	   CONFIGURE_ACCELEROMETER, EXIT_SETTING};

unsigned char boardModeLOCAL;
int  Acc_X_Calibrated_OffsetLOCAL;
int  Acc_Y_Calibrated_OffsetLOCAL;
int  Acc_Z_Calibrated_OffsetLOCAL;
unsigned char lcdBackLightLevelSettingLOCAL;
unsigned char lcdContrastSettingLOCAL;
unsigned char temperatureConversionLOCAL;
long temperatureCalibrationCLOCAL;
unsigned char wakeUpOnAccLOCAL;

static char menuText[]={
  "MSP-EXP430F5438\0"
  " 1. Clock    \0"
  " 2. UniBall  \0"          
  " 3. USB-UART \0"
  " 4. Voice Rec\0"
  " 5. PMM-MCLK \0"
  " 6. Puzzle   \0"
  " 7. Settings \0"  
};

unsigned char menuPos, settingMenuPos;
unsigned char timeOutCounter  = 0; 
unsigned char CpuMode, accWake=0, menuPos, settingMenuPos;

volatile unsigned char buttonsPressed;
volatile unsigned char buttonDebounce; 

char TemperatureStr[]= "\0\0\0\0\0\0";
char VccStr[] = "0.0V"; 
unsigned char RTCAccHalfSec = 0, RTCExit64Hz= 0 , RTCExitSec= 0, RTCAccSec = 0;

#include "UserExperienceDemo/clock.c"
#include "UserExperienceDemo/balanceBall.c"
#include "UserExperienceDemo/usbTest.c"
#include "UserExperienceDemo/audio.c"
#include "UserExperienceDemo/menuSetting.c"
#include "UserExperienceDemo/PMM.c"
#include "UserExperienceDemo/Puzzle.h"

void setupRTC();

/**********************************************************************//**
 * @brief  Checks for the board revision and returns a value < 0 if wrong
 *         revision is specified in main.c 
 * 
 * @param  none 
 *  
 * @return Whether or not the board revision matches the software
 * - 0 - The board revision does not match the software
 * - 1 - The board revision matches the software
 *************************************************************************/
unsigned char assert_board_version( void )
{
  P8DIR &= ~BIT7;                           // Set P8.7 input
  P8OUT |= BIT7;                            // Set pullup resistor
  P8REN |= BIT7;                            // Enable pull up resistors 
  
  #ifdef REV_02
    if(!(P8IN & BIT7))                      // Board rev = 0_02? 
      return 0;
  #else 
    if((P8IN & BIT7))                       // Board rev = 0_03? 
      return 0;
  #endif    
  
  P8DIR |= BIT7;                            // Set P8.7 output
  P8OUT &= ~BIT7;                           // Set P8.7 = 0 
  P8REN &= ~BIT7;                           // Disable pull up resistors   

  return 1; 
}

/**********************************************************************//**
 * @brief  Enters LPM3 and waits for either accelerometer tilt or a button tilt
 *         to activate the board. 
 * 
 * @param  none
 * 
 * @return none 
 *************************************************************************/
void lowPowerMode3(void)
{
  int accX, accY, accZ;
  
  CpuMode = LPM3_MODE;

  halLcdClearScreen();
  halLcdImage(TI_BUG, 14, 106, 10, 0);  
    
  halLcdSetBackLight(0);
  halLcdStandby();
  
  accWake = 0;
  if (wakeUpOnAcc)
  {
    RTCCTL0 |= RTCRDYIE;                    //Enable interrupt
    RTCAccSec = 1;
    halAccelerometerInit();  
    halAdcSetQuitFromISR( 1 );  
  }
  
  halBoardSetSystemClock( SYSCLK_12MHZ );   
  
  /* Either a button press or an RTC interrupt will wake the CPU 
   * from LPM3 mode. The RTC interrupt will periodically enable and 
   * re-initialize the accelerometer to see if the user has registered 
   * a change in the tilt that is greater than the accelerometer thresholds. 
   * If so, accWake is set and the board is activated
   */
  do
  {  	
    halAccelerometerShutDown();	            
    
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3 
    __no_operation();                       // For debugger only
    
    if (!buttonsPressed)
    {
      halAccelerometerRead( &accX, &accY, &accZ );
      accWake = ( accX > ACC_X_THRESHOLD || accX < -ACC_X_THRESHOLD || 
                   accY > ACC_Y_THRESHOLD || accY < -ACC_Y_THRESHOLD) ;
    }
  }
  while (accWake == 0 && buttonsPressed == 0);
  
  halBoardSetSystemClock( SYSCLK_16MHZ );    
    
  RTCCTL0 |= RTCRDYIE;                       //Enable interrupt
  RTCAccSec = 0;
  halAccelerometerShutDown();
  
  halLcdInit();
  halLcdInit();
  halLcdClearScreen();
  halLcdSetBackLight(lcdBackLightLevelSettingLOCAL );
  CpuMode = ACTIVE_MODE;
}

// -------------- Active Mode with Menu---------------------------------------

/**********************************************************************//**
 * @brief  Enters LPM3 and waits for either accelerometer tilt or a button tilt
 *         to activate the board. 
 * 
 * @param  menuText The text that constitues the application menu.
 * 
 * @param  menuPos  The line of the current menu option. 
 * 
 * @param  change   
 * 
 * - 0 - Move menu selection up
 * - 1 - Move menu selection down
 * 
 * @param menuNum   The enumerated value that represents the current 
 *                  menu selection. 
 * 
 * @return none 
 *************************************************************************/

void menuUpdate(char *menuText, unsigned char *menuPos, int change, 
                unsigned char menuNum)
{
  halLcdPrintLine(&menuText[*menuPos*MENU_ITEM_WIDTH+16], 
                  (*menuPos)+1,  OVERWRITE_TEXT );
  
  if (change == 0)                          //Subtract
  {
    if ( *menuPos > 0 )
      (*menuPos)--;
    else
      *menuPos = (menuNum - 1);                
  }
  else
  {
    if ( (*menuPos) < menuNum - 1 )
      (*menuPos)++;
    else
      *menuPos = 0;      
  }
  
  halLcdPrintLine(&menuText[*menuPos*MENU_ITEM_WIDTH+16], 
                  (*menuPos)+1, INVERT_TEXT | OVERWRITE_TEXT );  
}

/**********************************************************************//**
 * @brief  Draws and manages the selection of the menu options.
 * 
 * @param  menuText The text that constitues the application menu. 
 * 
 * @param  menuNum  The enumerated value that represents the current 
 *                  menu selection. 
 * 
 * @return The updated, or latest, menu selection. 
 *************************************************************************/
unsigned char activeMenuMode(char *menuText, unsigned char menuNum)
{
  unsigned char menuPosition, quit = 0;
  int i;
    
  halAccelerometerShutDown();  
  halAdcInitTempVcc();
  RTCExitSec = 1;                           // To update digital clock
  halButtonsInterruptEnable( BUTTON_ALL );  

  halLcdClearScreen();    
  halLcdImage(TI_TINY_BUG, 4, 32, 104, 12 );
  menuPosition = 0;
  //Print menu title
  halLcdPrintLine(menuText, 0, 0 );
  
  //Print menu items
  for (i=1;i<menuNum;i++)
    halLcdPrintLine(&menuText[i*MENU_ITEM_WIDTH+16], i+1,  OVERWRITE_TEXT );
  //First line is inverted text, automatic selection
  halLcdPrintLine(&menuText[0*MENU_ITEM_WIDTH+16],  1,     \
                  INVERT_TEXT | OVERWRITE_TEXT );  
  timeOutCounter = 0;
  buttonsPressed = 0;
  
  halAdcSetQuitFromISR( 0 );
  
  while (CpuMode == ACTIVE_MODE && quit == 0)
  {     
    TA0CTL &= ~TAIFG; 
    
    __bis_SR_register(LPM3_bits + GIE);     //Returns if button pressed or clock ticks     
    __no_operation();                       // For debugger only
        
    if (buttonsPressed)
    {
      switch (buttonsPressed) 
      {
        case BUTTON_UP:       menuUpdate(menuText, &menuPosition, 0, menuNum); break;
        case BUTTON_DOWN:     menuUpdate(menuText, &menuPosition, 1, menuNum); break;
        case BUTTON_SELECT:   CpuMode = APPLICATION_MODE; break;  
        case BUTTON_S2:       CpuMode = APPLICATION_MODE; break; 
        case BUTTON_S1:       quit = 1; 
        default: break;          
      }
      timeOutCounter  = 0;
    }
    else                                  //if no button pressed --> clock ticks
    {      
      halAdcStartRead();
      digitalClockDraw();      
      halAdcReadTempVcc(TemperatureStr, VccStr);      
      halLcdPrintLineCol(TemperatureStr, 7, 13,  OVERWRITE_TEXT);      
      halLcdPrintLineCol(VccStr, 8, 13,  OVERWRITE_TEXT);
      if (++timeOutCounter > TIME_OUT)
        CpuMode = LPM3_MODE;          
    }
    buttonsPressed  = 0;      
  }  
  RTCExitSec = 0;
  halAdcShutDownTempVcc();
  
  return menuPosition;
}

/**********************************************************************//**
 * @brief  This is the example code's main function. 
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
void UserExperience( void )
{  
  /* Check for the version of the board */ 
  if(!assert_board_version())
    while(1);  
  
  //Initialize clock and peripherals 
  halBoardInit();  
  halBoardStartXT1();	
  halBoardSetSystemClock(SYSCLK_16MHZ);
       
  loadSettings();
  
  //Initialize LCD and backlight    
  halLcdInit();       
  halLcdBackLightInit();  
  halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
  halLcdSetContrast(lcdContrastSettingLOCAL);
  halLcdClearScreen(); 
  halLcdImage(TI_BUG, 14, 106, 10, 0);
  
  //Initialize buttons
  buttonDebounce = 1;
  halButtonsInit( BUTTON_ALL );
  halButtonsInterruptEnable( BUTTON_ALL );
	
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3
  __no_operation();                         // For debugging only 
  
  CpuMode = ACTIVE_MODE;
  setupRTC();  
  
  while (1)                                 // main loop 
  {
    switch (CpuMode) 
    {
      case LPM3_MODE       : lowPowerMode3();          break;   
      case ACTIVE_MODE     : menuPos = activeMenuMode(menuText, MENU_MAX); 
                                                       break;
      case APPLICATION_MODE:       
        CpuMode = ACTIVE_MODE;  
        halButtonsInterruptEnable( BUTTON_ALL ); 
         
        switch (menuPos)
        {
          case APP_BAL_BALL : balanceBall();           break;
          case APP_CLOCK    : displayAnalogClock();    break;
          case APP_USB      : usbTest();               break;
          case APP_AUDIO    : audio(); saveSettings(); break;
          case PMM_MCLK     : menuPMMMCLK();           break;          
          case PUZZLE		: StartPuzzle();		   break;         
          case MENU_SETTING : 
            CpuMode = ACTIVE_MODE;
            settingMenuPos = activeMenuMode(settingMenuText, SETTING_MENU_MAX); 
            if (CpuMode == APPLICATION_MODE) 
            switch (settingMenuPos)
            {
              case SET_TIME                  : setTime();     break;
              case SET_CONTRAST              : setContrast(); break;
              case SET_BACKLIGHT             : setBackLight();break;
              case SET_TEMPERATURE_CONVERSION: setTemperatureConversion(); 
                                                              break;
              case CONFIGURE_ACCELEROMETER   : configureAccelerometer(); 
                                                              break;                                  
              case EXIT_SETTING              : saveSettings();
                                               menuPos = APP_CLOCK; 
                                               CpuMode = ACTIVE_MODE;
                                                              break;
            }                                                      
            //save settings if timeout in setting menu
            if (CpuMode == LPM3_MODE)
            saveSettings();                                                   
        }       
        break;      
    }
  }
}

/**********************************************************************//**
 * @brief  Initializes the RTC calendar. 
 * 
 * Initial values are January 01, 2009, 12:30:05
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
void setupRTC(void)
{  
  RTCCTL01 = RTCMODE + RTCBCD + RTCHOLD + RTCTEV_1;
  
  SetRTCHOUR(0x12);
  SetRTCMIN(0x30);
  SetRTCSEC(0x05);  
  SetRTCDAY(0x01);
  SetRTCMON(0x01);
  SetRTCYEAR(0x2009);
  
  RTCCTL01 &= ~RTCHOLD;
     
  RTCPS1CTL = RT1IP_5;                      // Interrupt freq: 2Hz  
  RTCPS0CTL = RT0IP_7;                      // Interrupt freq: 128hz
  
  RTCCTL0 |= RTCRDYIE + RTCTEVIE;           // Enable interrupt  
}

/**********************************************************************//**
 * @brief  Sets up the WDT as a button debouncer, only activated once a
 *         button interrupt has occurred. 
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
void startWDT()
{
  //WDT as 250ms interval counter
  SFRIFG1 &= ~WDTIFG;
  WDTCTL = WDTPW + WDTSSEL_1 + WDTTMSEL + WDTCNTCL + WDTIS_5;
  SFRIE1 |= WDTIE;
}

/************************************************************************/
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  static unsigned char counter=0;
  switch (RTCIV)
  {
    case 0x02:  if (RTCExitSec == 1)
                  __bic_SR_register_on_exit(LPM3_bits); 
                
                if (RTCAccSec == 1)
                {
                	halAccelerometerInit();
                	halAdcStartRead();
                }                
                break;
    case 0x04:  hourAdd();break;
    case 0x08:  if (RTCExit64Hz == 1)        
                  if (++counter == 4)
                  {
                    counter = 0;
                    __bic_SR_register_on_exit(LPM3_bits);
                  } 
                break;
    case 0x0A:  if (RTCAccHalfSec == 1)
                {        
                  ADC12IFG = 0;
                  ADC12CTL0 |= ADC12ENC | ADC12SC;
                }       
                break;      
    
  }
  RTCCTL0 &= ~RTCRDYIFG;
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
  if (buttonDebounce == 2)
  {
    buttonDebounce = 1;
    SFRIFG1 &= ~WDTIFG;
    SFRIE1 &= ~WDTIE;
    WDTCTL = WDTPW + WDTHOLD;
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{ 
  if (buttonDebounce == 1)   
  {
    buttonsPressed = P2IFG;
    buttonDebounce = 2;   
    startWDT();     
    __bic_SR_register_on_exit(LPM3_bits);    
  }
  else if (0 == buttonDebounce) 
  {
    buttonsPressed = P2IFG;
    __bic_SR_register_on_exit(LPM4_bits);     
  }
   
  P2IFG = 0;
}
