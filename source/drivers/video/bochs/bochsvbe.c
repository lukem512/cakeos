/* CakeOS */
#include <system.h>
#include "bochs.h"

extern unsigned fb_seg;
extern unsigned long* vga_mem;

/*
 * This driver is for Plex86/Bochs VBE graphics card (c) Luke Mitchell
 * It will run on plex86 cards, as well as VirtualBox, Qemu and Bochs
 */

static void SetVideoMode(unsigned short width, unsigned short height, unsigned short depth);
static void ExitVideoMode();
static void WriteCommand(unsigned short index, unsigned short value);

void bochs_vbe_set_mode(unsigned short width, unsigned short height, unsigned short depth)
{   
    fb_seg = 0xE0000000;
    vga_mem = (unsigned long*)0xE0000000;
    SetVideoMode(width,height,depth);
}

void bochs_vbe_exit_mode()
{
     ExitVideoMode();
}

static void SetVideoMode(unsigned short width, unsigned short height, unsigned short depth)
{    
   WriteCommand(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
   WriteCommand(VBE_DISPI_INDEX_XRES, width);
   WriteCommand(VBE_DISPI_INDEX_YRES, height);
   WriteCommand(VBE_DISPI_INDEX_BPP, depth); 
   WriteCommand(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED );
   outb(0x3c0, 0x20);
}

static void ExitVideoMode()
{
   WriteCommand(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
   
}

static void WriteCommand(unsigned short index, unsigned short value)
{
   outw(VBE_DISPI_IOPORT_INDEX, index);
   outw(VBE_DISPI_IOPORT_DATA, value);
} 
