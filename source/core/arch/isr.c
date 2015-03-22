/* CakeOS */
#include <system.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    u8int int_no = regs.int_no & 0xFF;
    //dprintf(" INT:0x%x ",int_no);
    if (interrupt_handlers[int_no] != 0)
    {
        isr_t handler = interrupt_handlers[int_no];
        handler(&regs);
        return;
    }
    else
    {
        //dprintf("unhandled interrupt: %d\n",int_no);
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{   
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master.
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {   
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
}
