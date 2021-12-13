/*******************************************************************************
    Filename: hal_rf.h

    Copyright 2008 Texas Instruments, Inc.
***************************************************************************/
#ifndef HAL_RF_H
#define HAL_RF_H

#define RF_PORT_OUT     P3OUT
#define RF_PORT_DIR     P3DIR
#define RF_PORT_SEL     P3SEL
#define RF_PORT_IN      P3IN
#define RF_SPI_SIMO     BIT1
#define RF_SPI_SOMI     BIT2
#define RF_SPI_CLK      BIT3
#define RF_SPI_CS       BIT0
typedef struct {
    unsigned char  fsctrl1;    // Frequency synthesizer control.
    unsigned char  fsctrl0;    // Frequency synthesizer control.
    unsigned char  freq2;      // Frequency control word, high byte.
    unsigned char  freq1;      // Frequency control word, middle byte.
    unsigned char  freq0;      // Frequency control word, low byte.
    unsigned char  mdmcfg4;    // Modem configuration.
    unsigned char  mdmcfg3;    // Modem configuration.
    unsigned char  mdmcfg2;    // Modem configuration.
    unsigned char  mdmcfg1;    // Modem configuration.
    unsigned char  mdmcfg0;    // Modem configuration.
    unsigned char  channr;     // Channel number.
    unsigned char  deviatn;    // Modem deviation setting (when FSK modulation is enabled).
    unsigned char  frend1;     // Front end RX configuration.
    unsigned char  frend0;     // Front end RX configuration.
    unsigned char  mcsm0;      // Main Radio Control State Machine configuration.
    unsigned char  foccfg;     // Frequency Offset Compensation Configuration.
    unsigned char  bscfg;      // Bit synchronization Configuration.
    unsigned char  agcctrl2;   // AGC control.
    unsigned char  agcctrl1;   // AGC control.
    unsigned char  agcctrl0;   // AGC control.
    unsigned char  fscal3;     // Frequency synthesizer calibration.
    unsigned char  fscal2;     // Frequency synthesizer calibration.
    unsigned char  fscal1;     // Frequency synthesizer calibration.
    unsigned char  fscal0;     // Frequency synthesizer calibration.
    unsigned char  fstest;     // Frequency synthesizer calibration.
    unsigned char  test2;      // Various test settings.
    unsigned char  test1;      // Various test settings.
    unsigned char  test0;      // Various test settings.
    unsigned char  iocfg2;     // GDO2 output pin configuration.
    unsigned char  iocfg0;     // GDO0 output pin configuration.
    unsigned char  pktctrl1;   // Packet automation control.
    unsigned char  pktctrl0;   // Packet automation control.
    unsigned char  addr;       // Device address.
    unsigned char  pktlen;     // Packet length.
} HAL_RF_CONFIG;

/*-------------------------------------------------------------
 *                  Function Prototypes 
 * ------------------------------------------------------------*/ 
void halRfInit(void);
void halRfShutDown(void);
void halRfResetChip(void);
unsigned char halRfWriteReg(unsigned char address, unsigned char data);
unsigned char halRfReadStatus(unsigned char address);
unsigned char halRfGetChipId(void);
unsigned char halRfGetChipVer(void);
void halRfSetupChip(void);

#endif
