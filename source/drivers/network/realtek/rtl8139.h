/* CakeOS */

#ifndef RTL8139_H
#define RTL8139_H

#define RTL8139_TSD0 0x10 //Transmit Status Descriptor 0
#define RTL8139_TSAD0 0x20 //Transmit Start Address Descriptor 0
#define RTL8139_MAC0 0x30 //MAC address
#define RTL8139_RBSTART 0x30 //Receive Buffer START
#define RTL8139_COMMAND 0x37
#define RTL8139_IMR 0x3C //Interrupt Mask Register
#define RTL8139_ISR 0x3E //Interrupt Status Register
#define RTL8139_CONFIG_0 0x51
#define RTL8139_CONFIG_1 0x52

typedef struct rtl8139_tsd0_struct
{
    unsigned int Size             : 13;  // Size of packet
    unsigned int OWN              : 1;   // Set to 1 by RTL8139 when DMA is complete
    unsigned int TUN              : 1;   // Transmit Underrun
    unsigned int TOK              : 1;   // Transmit OK
    unsigned int Threshhold       : 6;   // Early Transmit Threshhold
    unsigned int Reserved         : 2;
    unsigned int CollisionCount   : 4;   // Number of collisions during transfer
    unsigned int CDH              : 1;   // CD Heart Beat - cleared in 100mb/s
    unsigned int OWC              : 1;   // Out of Window Collision
    unsigned int TABT             : 1;   // Transfer Abort
    unsigned int CRS              : 1;   // Carrier Sense lost
} RTL8139_TSD0_t;

typedef struct rtl8139_isr_struct
{
    unsigned short ROK            : 1;   // Received OK
    unsigned short RER            : 1;   // Receive Error
    unsigned short TOK            : 1;   // Transmit OK
    unsigned short TER            : 1;   // Transmit Error
    unsigned short ROF            : 1;   // Receive Overflow (RXOVW)
    unsigned short PUN            : 1;   // Packet Underrun//Link Change
                                         // ^ Set to 1 when CAPR is written but rx buffer is empty
    unsigned short FERR           : 1;   // FIFO Overflow (FOVW)
    unsigned short Reserved       : 6;
    unsigned short LenChg         : 1;   // Cable Length Change
    unsigned short TimeOut        : 1;   // When TCTR reaches TimerInt
    unsigned short SERR           : 1;   // System Error - PCI Bus Error
} RTL8139_ISR_t;

void initialise_rtl8139(u32int base_address, int IRQ);

#endif
