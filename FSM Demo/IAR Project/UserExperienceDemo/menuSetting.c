/**********************************************************************//**
 *  @file menuSetting.c
 * 
 * Copyright 2008 Texas Instruments, Inc.
 *************************************************************************/

static char settingMenuText[]={
"Settings Menu  \0"
" 1. Set Time \0"
" 2. Contrast \0"
" 3. Backlight\0"
" 4. Temp: F C\0"
" 5. Set Acc. \0"
" 6. Exit     \0"
" 7. Exit     \0"
" PlaceHolder \0"
};

/*-------------------------------------------------------------------------*/
/**********************************************************************//**
 * @brief  Stores calibration and user-config data into INFOB flash
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void saveSettings(void)
{    
  flashEraseSegment((unsigned long)&boardMode);
  FLASH_UNLOCK;
  boardMode = boardModeLOCAL ;
  Acc_X_Calibrated_Offset =  Acc_X_Calibrated_OffsetLOCAL ;
  Acc_Y_Calibrated_Offset =  Acc_Y_Calibrated_OffsetLOCAL ;
  Acc_Z_Calibrated_Offset = Acc_Z_Calibrated_OffsetLOCAL ;
  wakeUpOnAcc = wakeUpOnAccLOCAL;  
  lcdBackLightLevelSetting = lcdBackLightLevelSettingLOCAL ;
  lcdContrastSetting =  lcdContrastSettingLOCAL;
  lastAudioByteFlash = lastAudioByte;
  temperatureConversion = temperatureConversionLOCAL;  
  temperatureCalibrationC = temperatureCalibrationCLOCAL;  
  FLASH_LOCK;  
}

/**********************************************************************//**
 * @brief  Loads calibration and user-config data from INFOB flash. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void loadSettings(void)
{  
  boardModeLOCAL = boardMode;
  Acc_X_Calibrated_OffsetLOCAL = Acc_X_Calibrated_Offset;
  Acc_Y_Calibrated_OffsetLOCAL = Acc_Y_Calibrated_Offset;
  Acc_Z_Calibrated_OffsetLOCAL = Acc_Z_Calibrated_Offset;
  lcdBackLightLevelSettingLOCAL = lcdBackLightLevelSetting;
  lcdContrastSettingLOCAL = lcdContrastSetting;
  lastAudioByte = lastAudioByteFlash;
  temperatureConversionLOCAL = temperatureConversion;
  temperatureCalibrationCLOCAL = temperatureCalibrationC;
  wakeUpOnAccLOCAL = wakeUpOnAcc;
  
  if (Acc_X_Calibrated_OffsetLOCAL > 4096 || Acc_X_Calibrated_OffsetLOCAL <= 0)
  {
    Acc_X_Calibrated_OffsetLOCAL = (ACC_X_LOW_OFFSET + ACC_X_HIGH_OFFSET) / 2;   
    Acc_Y_Calibrated_OffsetLOCAL = (ACC_Y_LOW_OFFSET + ACC_Y_HIGH_OFFSET) / 2; 
    Acc_Z_Calibrated_OffsetLOCAL = (ACC_Z_LOW_OFFSET + ACC_Z_HIGH_OFFSET) / 2;
  }        
  if (temperatureCalibrationCLOCAL > 10000 || temperatureCalibrationCLOCAL <=0)
    temperatureCalibrationCLOCAL = CELSIUS_OFFSET;   
    
  if ( lcdContrastSettingLOCAL < 60 || lcdContrastSettingLOCAL > 128) 
    lcdContrastSettingLOCAL = 100;
  if ( lcdBackLightLevelSettingLOCAL > 15) 
    lcdBackLightLevelSettingLOCAL = 16;
  
  halAdcSetTempConversionType( temperatureConversionLOCAL );
  halAdcSetTempOffset( temperatureCalibrationCLOCAL );
  halAccelerometerSetCalibratedOffset( Acc_X_Calibrated_OffsetLOCAL, \
                                       Acc_Y_Calibrated_OffsetLOCAL, \
                                       Acc_Z_Calibrated_OffsetLOCAL  );
}

/*-------------------------------------------------------------------------*/
/**********************************************************************//**
 * @brief  Interface to adjust the contrast of the LCD.
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void setContrast( void )
{
  unsigned char quit = 0, previousP2IE ;
  halLcdClearScreen();

  halLcdPrintLine(" Adjust Contrast", 0, 0 );

  halLcdPrintLineCol(" - ", 8, 1, 0 );
  halLcdPrintLineCol(" + ", 8, 13, INVERT_TEXT );
  
  halLcdLine(6, 94, 32, 94, PIXEL_ON);
  halLcdLine(6, 94, 6, 109, PIXEL_ON);
  halLcdLine(32, 94, 32, 109, PIXEL_ON);
  halLcdLine(6, 109, 32, 109, PIXEL_ON);

  halLcdLine(102, 94, 128, 94, PIXEL_ON);
  halLcdLine(128, 94, 128, 109, PIXEL_ON);
  halLcdLine(102, 94, 102, 109,  PIXEL_ON);
  halLcdLine(102, 109, 128, 109,  PIXEL_ON);
  
  previousP2IE = P2IE;
  P2IE = BUTTON_SELECT + BUTTON_S1 + BUTTON_S2;
  
  while (!quit)
  {
    TA0CTL &= ~TAIFG; 
    
    __bis_SR_register(LPM3_bits + GIE); 
    __no_operation(); 
    
    if (buttonsPressed & BUTTON_S2)
      halLcdSetContrast( halLcdGetContrast() + 5);
    else if (buttonsPressed & BUTTON_S1)
      halLcdSetContrast( halLcdGetContrast() - 5);
    if (buttonsPressed & BUTTON_SELECT)
      quit = 1;        
      
    buttonsPressed = 0;
  }
  P2IE = previousP2IE; 
  lcdContrastSettingLOCAL = halLcdGetContrast();
}

/*-------------------------------------------------------------------------*/
/**********************************************************************//**
 * @brief  Interface to adjust the brightness of the backlight. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void setBackLight( void )
{
  unsigned char quit = 0, previousP2IE ;
  unsigned char backlight;
  halLcdClearScreen();
  halLcdPrintLine("Adjust Backlight",  0, 0);

  halLcdPrintLineCol(" - ", 8, 1, 0 );
  halLcdPrintLineCol(" + ", 8, 13, INVERT_TEXT );
  
  halLcdLine(6, 94, 32, 94, PIXEL_ON);
  halLcdLine(6, 94, 6, 109, PIXEL_ON);
  halLcdLine(32, 94, 32, 109, PIXEL_ON);
  halLcdLine(6, 109, 32, 109, PIXEL_ON);

  halLcdLine(102, 94, 128, 94, PIXEL_ON);
  halLcdLine(128, 94, 128, 109, PIXEL_ON);
  halLcdLine(102, 94, 102, 109,  PIXEL_ON);
  halLcdLine(102, 109, 128, 109,  PIXEL_ON);
    
  previousP2IE = P2IE;
  P2IE = BUTTON_SELECT + BUTTON_S1 + BUTTON_S2;
  
  while (!quit)
  {
    TA0CTL &= ~TAIFG; 
    
    __bis_SR_register(LPM3_bits + GIE); 
    __no_operation(); 
    
    backlight = halLcdGetBackLight();
    if (buttonsPressed & BUTTON_S2)
      if (backlight < 16)
        halLcdSetBackLight( backlight + 2);
      else
        halLcdSetBackLight( 16 );        
    else if (buttonsPressed & BUTTON_S1)
      if (backlight > 0 )
        halLcdSetBackLight( backlight - 2);
      else
        halLcdSetBackLight( 0 );        

    if (buttonsPressed & BUTTON_SELECT)
      quit = 1;        
    buttonsPressed = 0;
  }
  
  P2IE = previousP2IE; 
  lcdBackLightLevelSettingLOCAL = halLcdGetBackLight();
}

/*----------------------------------------------------------------------------*/
/**********************************************************************//**
 * @brief  Interface to select the F or C format, as well as to calibrate
 *         the temperature sensor to the current ambient temperature. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void setTemperatureConversion( void )
{
  unsigned char quit = 0, position=0, conversionRequired=0;
  long temp, savedTemp, dummy;
  char tempStr[] = "000^C";
  
  halLcdClearScreen();
  halLcdPrintLine(" Set Temperature", 5, 0 );
  halLcdPrintLine("   Conversion", 6, 0 );   
  halLcdLine(6, 94, 32, 94, PIXEL_ON);
  halLcdLine(6, 94, 6, 109, PIXEL_ON);
  halLcdLine(32, 94, 32, 109, PIXEL_ON);
  halLcdLine(6, 109, 32, 109, PIXEL_ON);
  halLcdLine(102, 94, 128, 94, PIXEL_ON);
  halLcdLine(128, 94, 128, 109, PIXEL_ON);
  halLcdLine(102, 94, 102, 109,  PIXEL_ON);
  halLcdLine(102, 109, 128, 109,  PIXEL_ON);  
  
  halLcdPrintLine("Setup Thermostat", 0, 0 );  
  halLcdPrintLine("Calibrate Sensor", 1, 0);
  
  temp = halAdcGetTemp();
  savedTemp = temp;
  if (temperatureConversionLOCAL == FAHRENHEIT)
    savedTemp = (savedTemp - 320) * 5 / 9;
  while (temp>=1000)
  {
    tempStr[0]++;
    temp -= 1000;
  }
  while (temp>=100)
  {
    tempStr[1]++;
    temp -= 100;
  }
  while (temp>=10)
  {
    tempStr[2]++;
    temp -= 10;
  }
  if (temperatureConversion == FAHRENHEIT)
    tempStr[4] = 'F';
    
  halLcdPrintLineCol(tempStr, 3, 5, INVERT_TEXT | OVERWRITE_TEXT);
  halLcdPrintLineCol(&tempStr[1], 3, 6, OVERWRITE_TEXT );
    
  halButtonsInterruptEnable( BUTTON_ALL );
  
  while (!quit)
  {
    if (temperatureConversionLOCAL == CELSIUS)
    {
      halLcdPrintLineCol(" F ", 8, 1,  OVERWRITE_TEXT);
      halLcdPrintLineCol(" C ", 8, 13, INVERT_TEXT | OVERWRITE_TEXT );
      tempStr[4] = 'C';
    }
    else
    {
      halLcdPrintLineCol(" F ", 8, 1, INVERT_TEXT | OVERWRITE_TEXT);
      halLcdPrintLineCol(" C ", 8, 13,  OVERWRITE_TEXT);
      tempStr[4] = 'F';
    } 
    
    halLcdPrintLineCol(tempStr, 3, 5, OVERWRITE_TEXT);
    halLcdPrintLineCol(&tempStr[position], 3, 5+position, \
                                      INVERT_TEXT | OVERWRITE_TEXT );    
      halLcdPrintLineCol(&tempStr[position+1], 3, 6+position, OVERWRITE_TEXT );
         
    __bis_SR_register(LPM3_bits + GIE); 
    __no_operation(); 
    
    if (buttonsPressed & BUTTON_S2 || buttonsPressed & BUTTON_S1)
    {   
      conversionRequired = 0;
      if (tempStr[0]=='-')
          dummy = - ( (tempStr[1] - '0')*10 + (tempStr[2]-'0') );
        else  
        dummy = (tempStr[0]-'0')*100  +                    \
                (tempStr[1] - '0')*10 + (tempStr[2]-'0');
         
      if ((temperatureConversionLOCAL == FAHRENHEIT) &&    \
          (buttonsPressed & BUTTON_S2))                    
      {
        temperatureConversionLOCAL = CELSIUS;
        dummy = (dummy - 32) * 5 / 9 ;
        conversionRequired  = 1;
      }
      else
      if ((temperatureConversionLOCAL == CELSIUS) &&       \
          (buttonsPressed & BUTTON_S1)) 
      {
        temperatureConversionLOCAL = FAHRENHEIT;        
        dummy = dummy * 9 / 5 + 32;
        conversionRequired  = 1;
      }  
      if (conversionRequired)
      {
        if (dummy < 0)
          tempStr[0] ='-';
        else
        {  
          tempStr[0]='0';
          while (dummy>=100)
          {
            tempStr[0]++;
            dummy -=100;              
          }
        }
        tempStr[1]='0';
        while (dummy>=10)
        {
          tempStr[1]++;
          dummy -=10;              
        }
        tempStr[2]=dummy + '0';
      }    
    }
     
    if (buttonsPressed & BUTTON_LEFT)
    {
      if (position == 0)
        position = 2;      
      else
        position--;   
    }
    else if (buttonsPressed & BUTTON_RIGHT)
    {
      if (position == 2)
        position = 0;
      else
        position++;   
    }      
    else if (buttonsPressed & BUTTON_UP)
    {
      if (tempStr[position] >= '9' || tempStr[position]=='-')
        tempStr[position] = '0';
      else
        tempStr[position]++;                 
    }
    else if (buttonsPressed & BUTTON_DOWN)
    { 
      if (tempStr[position] <= '0' || tempStr[position]=='-')
        tempStr[position] = '9';
      else
        tempStr[position]--;                 
    }    
    else if (buttonsPressed & BUTTON_SELECT)
      quit = 1;        
    buttonsPressed = 0;
  }
   
  if (tempStr[0] == '-')
    temp = -( (tempStr[1]-'0')*100 + (tempStr[2]-'0')*10 );
  else
    temp = (tempStr[0]-'0')*1000 + (tempStr[1]-'0')*100 + (tempStr[2]-'0')*10;
  if (temperatureConversionLOCAL == FAHRENHEIT)
    temp = (temp - 320) *5 / 9;  
  temperatureCalibrationCLOCAL -= temp - savedTemp;    
  halAdcSetTempOffset( temperatureCalibrationCLOCAL );                 
  halAdcSetTempConversionType( temperatureConversionLOCAL );
  
  P2IFG = 0;  
}

/**********************************************************************//**
 * @brief  Interface to calibrate the accelerometer reading and to 
 *         specify whether the board will wake up on an accelerometer tilt.  
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void configureAccelerometer()
{
  unsigned char quit = 0 ;  
  halLcdClearScreen();
  halLcdPrintLine("  Accelerometer ", 0,  OVERWRITE_TEXT );
  halLcdPrintLine("Calibrate: Hit UP", 1, OVERWRITE_TEXT);
  
  halLcdPrintLine(" Wake Up on Acc", 4, 0 );
  halLcdPrintLine("Tilting will wake", 5, 0 );
  halLcdPrintLine("up the board", 6, 0 );
    
  halLcdLine(6, 94, 32, 94, PIXEL_ON);
  halLcdLine(6, 94, 6, 109, PIXEL_ON);
  halLcdLine(32, 94, 32, 109, PIXEL_ON);
  halLcdLine(6, 109, 32, 109, PIXEL_ON);

  halLcdLine(102, 94, 128, 94, PIXEL_ON);
  halLcdLine(128, 94, 128, 109, PIXEL_ON);
  halLcdLine(102, 94, 102, 109,  PIXEL_ON);
  halLcdLine(102, 109, 128, 109,  PIXEL_ON);
  
  halButtonsInterruptDisable( BUTTON_ALL );
  halButtonsInterruptEnable( BUTTON_SELECT + BUTTON_S1 + BUTTON_S2 + BUTTON_UP );
  
  while (!quit)
  {
    if (wakeUpOnAccLOCAL)
    {
      halLcdPrintLineCol("No ", 8, 1,  OVERWRITE_TEXT);
      halLcdPrintLineCol("Yes", 8, 13, INVERT_TEXT | OVERWRITE_TEXT );
    }
    else
    {
      halLcdPrintLineCol("No ", 8, 1,  INVERT_TEXT | OVERWRITE_TEXT);
      halLcdPrintLineCol("Yes", 8, 13,  OVERWRITE_TEXT );
    }    
    
    __bis_SR_register(LPM3_bits + GIE); 
    __no_operation(); 
    
    if (buttonsPressed & BUTTON_S2)
      wakeUpOnAccLOCAL = 1;
    else if (buttonsPressed & BUTTON_S1)
      wakeUpOnAccLOCAL = 0;
    if (buttonsPressed & BUTTON_UP)
    {
      halAccelerometerInit();
      halAccelerometerCalibrate();
      halAccelerometerGetCalibratedOffset(&Acc_X_Calibrated_OffsetLOCAL,   \
                                          &Acc_Y_Calibrated_OffsetLOCAL,   \
                                          &Acc_Z_Calibrated_OffsetLOCAL);  
      halAccelerometerShutDown(); 
      halLcdPrintLine("Acc. Calibrated", 2, OVERWRITE_TEXT);
    }  
    if (buttonsPressed & BUTTON_SELECT)
      quit = 1;        
    buttonsPressed = 0;
  }       
} 
