//****************************************************************************//
//  LCD Library
//  fsm_transition.h
//
//  Describtion:
//    Transition functions
//
//  Target MCU:   MSP430F5438
//  Target Board: ATC2008 Board
//
//   Source: main.c from
//        Michael Lueders
//        Texas Instruments Freising
//        August 2008
//
//   modified by:
//      Henrik Hostalka   
//      Texas Instruments Freising
//      August 2009
//
//      modifications:
//         WD-timer-interval -> TA1 - interval (16ms)
//****************************************************************************//

#ifndef FSM_TRANSITION_H
#define FSM_TRANSITION_H

#include <stdint.h>
#include <stdio.h>

//****************************************************************************//
// Game position typedef
//****************************************************************************//
typedef struct
{
  uint8_t PosX;
  uint8_t PosY;
} GAME_POS;


//****************************************************************************//
// Some useful macros
//****************************************************************************//
#define FIELD_SIZE    9
#define NR_OFFSET     0x30      


//****************************************************************************//

#endif /* FSM_TRANSITION_H */

//****************************************************************************//
