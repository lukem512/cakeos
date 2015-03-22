/* CakeOS */
/* RTL8139 NIC Driver */
#include <system.h>
#include "rtl8139.h"

//Prototypes
static void reset_rtl8139();
static void rtl8139_isr(registers_t regs);
static void  rtl8139_send(u32int address, int size);
//static void  rtl8139_receive();

//Receive buffer
//This is 8k plus the header size (16)
volatile char rx_buffer[8192+16];

//PCI BAR
//This is the Base Address for the I/O ports
static u32int BAR = 0;

void initialise_rtl8139(u32int base_address, int IRQ)
{
     //Set BAR
     BAR = base_address;
     
     //Install IRQ handler
     register_interrupt_handler(IRQ, &rtl8139_isr);
     
     //Send wake up command
     dprintf("[initialise_rtl8139] Sending WAKE_UP command\n");
     outb(BAR+RTL8139_CONFIG_1,0x00);
     
     //Reset software
     dprintf("[initialise_rtl8139] Resetting software\n");
     reset_rtl8139();
     
     //Set the Receive Buffer address
     dprintf("[initialise_rtl8139] Setting Receive Buffer addres\n");
     outl(BAR+RTL8139_RBSTART, (unsigned long)rx_buffer);
     
     //Enable Transmit OK and Receive OK interrupts
     dprintf("[initialise_rtl8139] Enabling TOK and ROK\n");
     outw(BAR+RTL8139_IMR, 0x0005);
     
     //...And finally enable Receive and Transmitter
     dprintf("[initialise_rtl8139] Enabling Receive and Transmit\n");
     outb(BAR+RTL8139_COMMAND, 0x0C);
     
     rtl8139_send(0x395930,40);
     
     return;
}

static void reset_rtl8139()
{
     //Send reset command
     dprintf("[reset_rtl8139] Sending RESET command\n");
     outb(BAR+RTL8139_COMMAND,0x10);
     
     //We must wait until the RST bit is clear
     dprintf("[reset_rtl8139] Waiting for RST to clear\n");
     while((inb(BAR+RTL8139_COMMAND) & 0x10))
          timer_wait(1); //1 uSecond
}

static void rtl8139_isr(registers_t regs)
{
     //RTL8139_ISR_t* isr;
     //isr = (RTL8139_ISR_t*)inw(BAR+RTL8139_ISR);
     
     //TODO: Error checks here
     
     dprintf("[rtl8139_isr] Interrupt Received!\n");
     
     //Clear TOK
     //isr->TOK = 0;
     //outw(BAR+RTL8139_ISR, (u16int)isr);
}

static void  rtl8139_send(u32int address, int size)
{
     //Location of packet
     dprintf("[rtl8139_send] Sending packet location to TSAD0\n");
     outl(BAR+RTL8139_TSAD0,address);
     
     //Status register
     dprintf("[rtl8139_send] Reading status (TSD0)\n");
     
     RTL8139_TSD0_t* tsd0;
     tsd0 = (RTL8139_TSD0_t*) inl(BAR+RTL8139_TSD0);
     
     //Set size (bits 0-12)
     dprintf("[rtl8139_send] Setting Size\n");
     tsd0->Size = size;
     
     //Set early tx threshhold (bits 16-21)
     dprintf("[rtl8139_send] Setting Early Transmit Threshhold (TODO)\n");
     tsd0->Threshhold = 0; //???<---------TODO: get working
     
     //Clear OWN bit (bit 13) - starts transfer
     dprintf("[rtl8139_send] Clearing OWN bit and starting transfer\n");
     tsd0->OWN = 0;
     
     //Send data
     outl(BAR+RTL8139_TSD0,(u32int)tsd0);
}

/*static void  rtl8139_receive()
{
}*/
