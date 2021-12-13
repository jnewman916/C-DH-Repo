/** 
 * @file  hal_usb.c
 * 
 * Copyright 2008 Texas Instruments, Inc.
***************************************************************************/
#include  <msp430.h>
#include "hal_MSP-EXP430F5438.h"

char halUsbReceiveBuffer[255];
unsigned char bufferSize=0;

/**********************************************************************//**
 * @brief  Initializes the serial communications peripheral and GPIO ports 
 *         to communicate with the TUSB3410.
 * 
 * @param  none
 * 
 * @return none
 **************************************************************************/
void halUsbInit(void)
{
  volatile unsigned char i;
  
  for (i = 0;i < 255; i++)
    halUsbReceiveBuffer[i]='\0';
	  
  bufferSize = 0;
  USB_PORT_SEL |= USB_PIN_RXD + USB_PIN_TXD;
  USB_PORT_DIR |= USB_PIN_TXD;
  USB_PORT_DIR &= ~USB_PIN_RXD;
  
  UCA1CTL1 |= UCSWRST;                          //Reset State                      
  UCA1CTL0 = UCMODE_0;
  
  UCA1CTL0 &= ~UC7BIT;                      // 8bit char
  UCA1CTL1 |= UCSSEL_2;
  UCA1BR0 = 16;                             // 8Mhz/57600=138
  UCA1BR1 = 1;
  UCA1MCTL = 0xE;
  UCA1CTL1 &= ~UCSWRST;  
  UCA1IE |= UCRXIE;
  
  __bis_SR_register(GIE);                   // Enable Interrupts
}

/**********************************************************************//**
 * @brief  Disables the serial communications peripheral and clears the GPIO
 *         settings used to communicate with the TUSB3410.
 * 
 * @param  none
 * 
 * @return none
 **************************************************************************/
void halUsbShutDown(void)
{
  UCA1IE &= ~UCRXIE;
  UCA1CTL1 = UCSWRST;                          //Reset State                         
  USB_PORT_SEL &= ~( USB_PIN_RXD + USB_PIN_TXD );
  USB_PORT_DIR |= USB_PIN_TXD;
  USB_PORT_DIR |= USB_PIN_RXD;
  USB_PORT_OUT &= ~(USB_PIN_TXD + USB_PIN_RXD);
}

/**********************************************************************//**
 * @brief  Sends a character over UART to the TUSB3410.
 * 
 * @param  character The character to be sent. 
 * 
 * @return none
 **************************************************************************/
void halUsbSendChar(char character)
{
  while (!(UCA1IFG & UCTXIFG));  
  UCA1TXBUF = character;
}

/**********************************************************************//**
 * @brief  Sends a string of characters to the TUSB3410
 * 
 * @param  string[] The array of characters to be transmit to the TUSB3410.
 * 
 * @param  length   The length of the string.
 * 
 * @return none
 **************************************************************************/
void halUsbSendString(char string[], unsigned char length)
{
  volatile unsigned char i;
  for (i=0; i < length; i++)
    halUsbSendChar(string[i]);  
}

/************************************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR (void)
{
  halUsbReceiveBuffer[bufferSize++] = UCA1RXBUF;
   __bic_SR_register_on_exit(LPM3_bits);   
}
