//****************************************************************************//
//  Inclination Game
//  Puzzle.c
//
//  Describtion:
//    Simple example to demonstrate the functionality of the Excel 
//    state machine generation tool
//
//  Target MCU:   MSP430F5438
//  Target Board: ATC2008 Board
//
//   Henrik Hostalka   
//   Texas Instruments Freising
//   August 2009
//****************************************************************************//

#include <MSP430x54x.h>
#include <stdint.h>

#include "..\MSP-EXP430F5438 HAL\hal_MSP-EXP430F5438.h"
#include "fsm_transition.h"
#include "fsm.h"
#include "Puzzle.h"

extern volatile unsigned char buttonsPressed;
extern uint8_t  ActState;
extern uint8_t  GameData [FIELD_SIZE];

//****************************************************************************//
// Start Puzzle Game
//****************************************************************************//
void StartPuzzle (void) {
   
   unsigned int quit = 0;
   
   TimerA1Init();
   halAccelerometerInit(); 
   
     FSM_Init ();    // initialize the Game and set the right active State
     FSM_InitGame ();
     
   while (!quit)  
  {
    halAdcStartRead();
    
    if (buttonsPressed & BUTTON_SELECT || buttonsPressed & BUTTON_S1)   // quit the game
    {
      TimerA1Stop ();
      quit = 1;
    }
    else if (buttonsPressed & BUTTON_S2) FSM_Button    ();            // reset the game
    buttonsPressed = 0;      
  }
   
}

//****************************************************************************//
// Leave Puzzle Game
//****************************************************************************//
void TimerA1Stop (void){
   //Turn off TimerA1
   TA1CTL = 0;
   TA1CCTL0 &= ~CCIE;
   TA1CCR0 = 0;   
}

//****************************************************************************//
// TimerA1 initialisation
//****************************************************************************//
void TimerA1Init (void){
   TA1CTL = TASSEL_1 | MC_1;      // ACLK (32.768 kHz)  Up mode 
   TA1CCTL0 = CCIE;            // CCR0 - Interrupt enabled 
   TA1CCR0 = 523;               // 16ms at ACLK 
}

//****************************************************************************//
// TimerA1 ISR
//****************************************************************************//
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1_CCR0_ISR (void){
   
  int   Acc [3];
  unsigned int i;
  
  P1OUT ^= BIT0;
   
  // Disable TimerA1 interrupt
  TA1CCTL0 &= ~CCIE;
  
  // Get the acceleration in x- and y-direction and call event function
  halAccelerometerRead(&Acc [0],&Acc [1],&Acc [2]);
  
  if      (Acc [0] < -ACC_X_THRESHOLD) {
     TA1CCR0 = 0x8000;   // next ISR at the earliest in 1s
     FSM_Right ();}
  else if (Acc [0] >  ACC_X_THRESHOLD) {
     TA1CCR0 = 0x8000;   // next ISR at the earliest in 1s
     FSM_Left  ();}
  else if (Acc [1] < -ACC_Y_THRESHOLD) {
     TA1CCR0 = 0x8000;   // next ISR at the earliest in 1s
     FSM_Up    ();}
  else if (Acc [1] >  ACC_Y_THRESHOLD) {
     TA1CCR0 = 0x8000;   // next ISR at the earliest in 1s
     FSM_Down  ();}
  
  if (ActState == 9){   // Check for winning the game
     for (i = 0; GameData [i] == (i + 1); i++); 
     if (i == 8) FSM_Win ();
  }
  
  // Enable  TimerA1 interrupt
  TA1CCTL0 = CCIE;
}



