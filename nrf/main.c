#include <msp430.h>

int main(void) {
WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

// Configure GPIO
P1DIR |= BIT1;           // Set P1.1 to output
P1OUT &= ~BIT1;        // set P1.1 to Off (green LED)          // set P1.1 to Off (green LED)

P6SEL0 &= ~(BIT0 | BIT1);
P6SEL1 |= BIT0 | BIT1; // USCI_A0 UART operation

// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
PM5CTL0 &= ~LOCKLPM5;

// Startup clock system with max DCO setting ~8MHz
CSCTL0_H = CSKEY >> 8; // Unlock clock registers
CSCTL1 = DCOFSEL_3 | DCORSEL; // Set DCO to 8MHz
CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers
CSCTL0_H = 0; // Lock CS registers

// Configure USCI_A0 for UART mode
UCA0CTLW0 = UCSWRST;
UCA0CTLW0 |= UCSSEL__SMCLK; // CLK = SMCLK
// Baud Rate calculation
// 8000000/(16*9600) = 52.083
// Fractional portion = 0.083
// UCBRFx = int ( (52.083-52)*16) = 1
UCA0BR0 = 4; // 8000000/16/9600
UCA0BR1 = 0x00;
UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
UCA0CTLW0 &= ~UCSWRST; // Initialize eUSCI
UCA0IE |= UCRXIE; // Enable USCI_A0 RX interrupt
__no_operation(); // For debugger
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{

while(!(UCA0IFG & UCTXIFG));
UCA0TXBUF = UCA0RXBUF; //echo
__bis_SR_register(LPM0_bits|GIE);
P1OUT|=BIT1;
__no_operation();
}
