/* CakeOS */
/* Ties all header files together for kernel */

#define DEBUG

#ifdef DEBUG
#define dprintf kprintf
#define DBGPRINTF(s) outb(0xe9, s);
#else
#define dprintf
#define DBGPRINTF(s)
#endif

#include <sys/version.h>             // current version of Cake
#include <sys/modeinfo.h>            // graphical state

#include <arch/common.h>             // common functions
#include <arch/regs.h>               // regs struct
#include <arch/asm.h>                // cpu specific asm functions
#include <arch/isr.h>                // ISR/IRQ
#include <arch/descriptor_tables.h>  // IDT/GDT stuff
#include <arch/paging.h>             // paging setup
#include <arch/kheap.h>              // heap setup/MMU
#include <arch/task.h>               // tasks/multitasking
#include <arch/binary.h>             // binary loading functions
#include <arch/pokex.h>              // poke/peek byte inline ASM functions

#include <gui/bmp.h>                 // BMP file stuff
#include <gui/window.h>              // window_t
#include <gui/component.h>           // component_t

#include <math/math.h>               // maths functions

#include <ordered_array.h>           // ordered arrays for linked lists
#include <text.h>                    // text mode functions
#include <fs.h>                      // VFS functions
#include <initrd.h>                  // initial ramdisk
#include <timer.h>                   // PIT timer
#include <multiboot.h>               // multiboot information
#include <datetime.h>                // datetime_t struct
#include <syscalls.h>                // syscall defines/prototypes
#include <vga_modes.h>               // VGA/VESA modes
#include <vga.h>                     // VGA functions
#include <keyb.h>                    // Keyboard initialisation
#include <ps2.h>                     // ps2 mouse
#include <elf.h>                     // ELF structs

//#include <../drivers/block/floppy/fdc.h>
//#include <../drivers/network/eth.h>
#include <../drivers/network/realtek/rtl8139.h>

/* For files without headers */
void init_timer(u32int frequency);
void timer_wait(int ticks);
datetime_t getDatetime();
int mktime(datetime_t time);
void panic(registers_t* r);
void panic_assert(const char *file, u32int line, const char *desc);
void schedule();
void beep();
void itoa (char *buf, int base, int d);
void init_pci();
