/**********************************************************************//**
    Filename: flashUtils.h

    Copyright 2008 Texas Instruments, Inc.
***************************************************************************/
#ifndef FLASH_UTILS_H
#define FLASH_UTILS_H
#include <msp430.h>

/*-------------------------------------------------------------
 *                       Macros 
 * ------------------------------------------------------------*/ 
#define FLASH_UNLOCK    FCTL3 = FWKEY; FCTL1 = FWKEY + WRT; 
#define FLASH_LOCK      FCTL1 = FWKEY; FCTL3 = FWKEY +  LOCK; 
#define FLASH_WRITE_INFOB(destination, source) st( FLASH_UNLOCK_INFOB;  destination = source; while (FCTL3 & BUSY ); FLASH_LOCK_INFOB; )  

/*-------------------------------------------------------------
 *                  Function Prototypes 
 * ------------------------------------------------------------*/ 
void flashEraseBank(unsigned long FarPtr);
void flashEraseSegment(unsigned long FarPtr);
void flashErase(unsigned long Memstart, unsigned long Memend);

#endif 


