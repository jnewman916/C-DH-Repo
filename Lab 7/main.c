#include <msp430.h>
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    P4SEL |= BIT4+BIT5; // P4.4,5 = USCI_A1 TXD/RXD
    UCA1CTL1 |= UCSWRST; // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2; // SMCLK 1048576 Hz
    UCA1BR0 = 109; // baud rate=9600
    UCA1BR1 = 0; //
    UCA1MCTL |= UCBRS_2 + UCBRF_0; // Modulation UCBRSx=2, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    ADC12CTL0 = ADC12SHT02 + ADC12ON;
    ADC12CTL1 = ADC12SHP+ADC12SSEL1;
    ADC12CTL0 |= ADC12ENC;

    int i = 0;
    while (1) {
        P6SEL |= BIT0;
        while(i < 1000) {

            ADC12CTL0 |= ADC12SC;
            while (ADC12CTL1 & ADC12BUSY);
            int thou = ((int)ADC12MEM0)/1000;
            int hund =  ((int)ADC12MEM0)%1000/100;
            int ten =  ((int)ADC12MEM0)%1000%100/10;
            int one =  ((int)ADC12MEM0)%1000%100%10;
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= thou + 0x30;
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= hund + 0x30;
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= ten + 0x30;
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= one + 0x30;
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= (0x0A);  //Enter
            while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
            UCA1TXBUF= (0x0D);  //Carriage return

            i = i + 1;
        }
        i = 0;
        P6SEL |= BIT1;
        while(i < 1000) {
                    ADC12CTL0 |= ADC12SC;
                    while (ADC12CTL1 & ADC12BUSY);
                    int thou = ((int)ADC12MEM0)/1000;
                    int hund =  ((int)ADC12MEM0)%1000/100;
                    int ten =  ((int)ADC12MEM0)%1000%100/10;
                    int one =  ((int)ADC12MEM0)%1000%100%10;
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= thou + 0x30;
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= hund + 0x30;
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= ten + 0x30;
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= one + 0x30;
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= (0x0A);  //Enter
                    while (!(UCA1IFG&UCTXIFG)); // USCI_A1 TX buffer ready?
                    UCA1TXBUF= (0x0D);  //Carriage return

                    i = i + 1;
        }
    }
}
