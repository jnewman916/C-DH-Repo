//****************************************************************************//
//  MSP430 state machine
//  fsm_transition.c
//
//  Describtion:
//    A simple state machine for the MSP430
//    You can add your own code in here!!!
//
//  Generated with Excel Table
//  Date:  08/26/2008        Time:  15:00:57
//
//****************************************************************************//

#include <MSP430.h>

#include "..\MSP-EXP430F5438 HAL\hal_MSP-EXP430F5438.h"
#include "fsm_transition.h"
#include "fsm.h"
#include <stdlib.h>

//****************************************************************************//
// Global variables
//****************************************************************************//
const GAME_POS GamePos [FIELD_SIZE] =
{
  28, 11, 64, 11, 100, 11,
  28, 47, 64, 47, 100, 47,
  28, 83, 64, 83, 100, 83
};

uint8_t  GameData [FIELD_SIZE];


//****************************************************************************//
//  UpdateField
//
//  DESCRIPTION:
//    Update field and check for winning the game
//
//  PROTOTYPE:
//    void UpdateField (void)
//
//  PARAMETERS:
//    None
//
//  RETURN VALUE:
//    None
//
//****************************************************************************//
void UpdateField (void)
{
  uint8_t   i;
  char   String [] = " ";

  // Update field
  for (i = 0; i < FIELD_SIZE; i++)
  {
    *String = GameData [i] + NR_OFFSET;
    if (*String == '0') *String = ' ';

    halLcdPrintXY(String, GamePos[i].PosX, GamePos[i].PosY, 0x04); 	// Textstyle: Overwrite = 0x04
  }

}


//****************************************************************************//
// Transition function "InitGame"
//****************************************************************************//
void FSM_InitGame (void)
{
  // Write text on LCD
  halLcdClearScreen();
  halLcdPrintXY("Press button S2", 10, 40, 0);
  halLcdPrintXY( "to start", 10, 57, 0);

  // Init and start TimerA
  TB0CTL = TBSSEL_2 + MC_2;
}


//****************************************************************************//
// Transition function "StartGame"
//****************************************************************************//
void FSM_StartGame (void)
{
  uint8_t   i;
  uint8_t   RandValue;

  //uint8_t   DefVal [FIELD_SIZE] = {1,2,3,4,6,8,7,5};  // test

  // Initialize random generator and stop TimerA
  srand (TB0R);
  TB0CTL = 0x00;

  // Clear game data
  for (i = 0; i < FIELD_SIZE; i++) GameData [i] = 0;

  // Generate random game data
  for (i = 1; i < FIELD_SIZE; i++)
  {
    do
      RandValue = rand () & 0x07;
    while (GameData [RandValue]);

    GameData [RandValue] = i;
  }
  // for test
  //for (i = 0; i <= 7; i++) GameData [i] = DefVal [i];


  // Draw field
  halLcdClearScreen();
  halLcdLine (14, 36, 124,  36, PIXEL_DARK);	// horizontal lines
  halLcdLine (14, 72, 124,  72, PIXEL_DARK);
  halLcdLine (50,  0,  50, 110, PIXEL_DARK);	// vertical lines
  halLcdLine (86,  0,  86, 110, PIXEL_DARK);

  // Update field
  UpdateField ();
}


//****************************************************************************//
// Transition function "PushLeft"
//****************************************************************************//
void FSM_PushLeft (void)
{
  GameData [ActState - 1] = GameData [ActState - 0 ];
  GameData [ActState - 0] = 0;

  // Update field
  UpdateField ();
}


//****************************************************************************//
// Transition function "PushRight"
//****************************************************************************//
void FSM_PushRight (void)
{
  GameData [ActState - 1] = GameData [ActState - 2];
  GameData [ActState - 2] = 0;

  // Update field
  UpdateField ();
}


//****************************************************************************//
// Transition function "PushUp"
//****************************************************************************//
void FSM_PushUp (void)
{
  GameData [ActState - 1] = GameData [ActState + 2];
  GameData [ActState + 2] = 0;

  // Update field
  UpdateField ();
}


//****************************************************************************//
// Transition function "PushDown"
//****************************************************************************//
void FSM_PushDown (void)
{
  GameData [ActState - 1] = GameData [ActState - 4];
  GameData [ActState - 4] = 0;

  // Update field
  UpdateField ();
}


//****************************************************************************//
// Transition function "StopGame"
//****************************************************************************//
void FSM_StopGame (void)
{
  // Write text on LCD
  halLcdClearScreen();
  halLcdPrintXY("WIN", 58, 47, 0);

}


//****************************************************************************//
