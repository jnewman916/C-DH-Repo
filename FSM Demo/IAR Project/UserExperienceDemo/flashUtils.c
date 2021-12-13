/**********************************************************************//**
 *  @file flashUtils.c
 * 
 * Copyright 2008 Texas Instruments, Inc.
 **************************************************************************/

#include <msp430.h>
#include "flashUtils.h"

/**********************************************************************//**
 * @brief  Erases a bank in flash memory.
 * 
 * @param  FarPtr The pointer to the location in memory within the bank to
 *                be erased.
 * 
 * @return none
 **************************************************************************/
void flashEraseBank( unsigned long FarPtr)
{
  FCTL3 = FWKEY;
  while (FCTL3 & BUSY );
  FCTL1 = FWKEY + MERAS;
  
  __data20_write_char(FarPtr, 0x00);        // Dummy write to start erase

  while (FCTL3 & BUSY );
  FCTL1 = FWKEY;                
  FCTL3 = FWKEY +  LOCK;
}

/**********************************************************************//**
 * @brief  Erases a single segment of memory containing the address FarPtr. 
 * 
 * @param  FarPtr The address location within the segment of memory to be 
 *                erased. 
 * 
 * @return none
 **************************************************************************/
void flashEraseSegment(unsigned long FarPtr)
{
  FCTL3 = FWKEY;
  FCTL1 = FWKEY + ERASE;
  
  __data20_write_char(FarPtr, 0x00);        // Dummy write to start erase
  
  while (FCTL3 & BUSY );
  FCTL1 = FWKEY;                
  FCTL3 = FWKEY +  LOCK;
}

/**********************************************************************//**
 * @brief  Erases the (multiple) segment(s) of memory including address 
 *         locations Memstart through Memend.
 * 
 * @param  Memstart The starting address location identifying a segment of
 *                  memory to be erased
 * 
 * @param  Memend   The ending address location identifying a segment of
 *                  memory to be erased
 * 
 * @return none
 **************************************************************************/
void flashErase(unsigned long Memstart, unsigned long Memend)
{
  unsigned long FarPtr = Memstart;          // Start of record memory array
  
  FCTL3 = FWKEY;                            // Unlock Flash memory for write
  do
  {
    if ( (FarPtr & 0xFFFF) == 0x0000)       // Use bit 12 to toggle LED
      P1OUT ^= 0x01;

    FCTL1 = FWKEY + ERASE;
    
    __data20_write_char(FarPtr, 0x00);      // Dummy write to activate
    
    while (FCTL3 & BUSY );                  // Segment erase
    FarPtr += 0x0200;                       // Point to next segment
  } while (FarPtr < Memend);
  
  FCTL1 = FWKEY;                
  FCTL3 = FWKEY +  LOCK;
}
