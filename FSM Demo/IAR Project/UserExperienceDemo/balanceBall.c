/**********************************************************************//**
 * @file balanceBall.c
 * 
 * Copyright 2008 Texas Instruments, Inc.
***************************************************************************/

/**********************************************************************//**
 * @brief  Executes the "UniBall" menu option of the User Experience  
 *         example software. 
 * 
 * Uses the accelerometer to track a ball that erases the TI logo as it 
 * moves around the screen. The application times out and resets the TI 
 * logo periodically. 
 * 
 * @param  none
 * 
 * @return none
 *************************************************************************/
void balanceBall(void)
{
  int x, y, dx, dy, dz, redrawCounter;
  unsigned char quit = 0;
  
  halAccelerometerInit();                     
  
  halButtonsInterruptDisable( BUTTON_ALL );
  halButtonsInterruptEnable( BUTTON_SELECT );
  buttonsPressed  = 0;

  //Starting location of the Ball  
  x = 80;
  y = 16;
                   
  halLcdClearScreen();
  halLcdImage(TI_BUG, 14, 106, 10, 0);  
  
  RTCExit64Hz = 1;
  RTCPS0CTL |= RT0PSIE; 
  redrawCounter = 0;
  halAdcSetQuitFromISR( 0 );

  while (!quit)  
  {
    halAdcStartRead(); 
    
    __bis_SR_register(LPM0_bits + GIE);      
    __no_operation(); 
    
    if (buttonsPressed & BUTTON_SELECT || buttonsPressed & BUTTON_S1)
      quit = 1;
      
    halAccelerometerRead( &dx, &dy, &dz );   
    dx >>= 5;
    dy >>= 5;
    if ( ( ABS(dx) > 2 ) || ( ABS(dy) > 2 ) ) 
    {
      halLcdCircle( x, y, 7, PIXEL_OFF);
      halLcdCircle( x, y, 6, PIXEL_OFF);
      halLcdCircle( x, y, 5, PIXEL_OFF);
      halLcdCircle( x, y, 4, PIXEL_OFF);
      halLcdCircle( x, y, 3, PIXEL_OFF);
      halLcdCircle( x, y, 2, PIXEL_OFF);
      halLcdCircle( x, y, 1, PIXEL_OFF);
      x -= dx;
      y += dy;
      
      if (x<0 || x >= LCD_COL)   
        x += dx;
      if (y <0 || y >= LCD_ROW)
        y -= dy;
     
      halLcdCircle( x, y, 2, PIXEL_ON);
      halLcdCircle( x, y, 3, PIXEL_ON);
      halLcdCircle( x, y, 4, PIXEL_ON);
      halLcdCircle( x, y, 5, PIXEL_ON);
      halLcdCircle( x, y, 6, PIXEL_ON);
    }
	if (++redrawCounter == 320)             //Redraw canvas after ~5s
	{
	  halLcdClearScreen();
  	  halLcdImage(TI_BUG, 14, 106, 10, 0);
  	  redrawCounter = 0;  			
  	  halLcdCircle( x, y, 2, PIXEL_ON);
      halLcdCircle( x, y, 3, PIXEL_ON);
      halLcdCircle( x, y, 4, PIXEL_ON);
      halLcdCircle( x, y, 5, PIXEL_ON);
      halLcdCircle( x, y, 6, PIXEL_ON);
	}
  }
  
  RTCPS0CTL &= ~RT0PSIE; 
  RTCExit64Hz = 0; 
}
