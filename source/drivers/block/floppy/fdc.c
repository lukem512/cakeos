/* CakeOS */

/* THIS NEEDS TO BE CONVERTED TO A DRIVER ONCE I GET 
   THE DRIVER INTERFACE WORKING. THAT MEANS I NEED TO 
   CONVERT ALL THE CALLS TO EITHER API OR SYSCALLS
*/

/*
  WE ALSO NEED TO CONVERT KPRINTF's TO EPRINTF - WRITE
  ERROR DATA TO A FILE
  
  AND GET THE TASKING SHIT SORTED, COS WAITING AROUND FOR
  THE FLOPPY TIMER ISN'T GONNA DO OUR PERFORMANCE MUCH GOOD.
  WE NEED TO GET THE WHOLE DRIVER THING SORTED BASICALLY.
*/

#include <system.h> 

static volatile int floppy_base = 0x03f0;
static volatile int floppy_number_drives;
static volatile int floppy_recieved_int = 1;
static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = 0; 

//Floppy Registers
enum floppy_registers {
   FLOPPY_DOR  = 2,  // digital output register
   FLOPPY_MSR  = 4,  // master status register, read only
   FLOPPY_FIFO = 5,  // data FIFO, in DMA operation for commands
   FLOPPY_CCR  = 7   // configuration control register, write only
}; 

//Floppy Commands
enum floppy_commands {
   FLOPPY_CMD_SPECIFY = 3,            // SPECIFY
   FLOPPY_CMD_WRITE_DATA = 5,         // WRITE DATA
   FLOPPY_CMD_READ_DATA = 6,          // READ DATA
   FLOPPY_CMD_RECALIBRATE = 7,        // RECALIBRATE
   FLOPPY_CMD_SENSE_INTERRUPT = 8,    // SENSE INTERRUPT
   FLOPPY_CMD_SEEK = 15,              // SEEK
}; 

//Floppy Motor States
enum { floppy_motor_off = 0, floppy_motor_on, floppy_motor_wait }; 

//DMA Stuff
#define floppy_dmalen 0x4800

typedef enum {
    floppy_dir_read = 1,
    floppy_dir_write = 2
} floppy_dir; 

static volatile char floppy_dmabuf[floppy_dmalen];

void floppy_motor_kill(int base);
void floppy_motor(int base, int onoff);
void floppy_handler (registers_t regs);
void floppy_change_drives(int drive);
int floppy_reset(int base);
int floppy_calibrate(int base);
void floppy_check_interrupt(int base, int *st0, int *cyl);

int initialise_floppy_driver()
{
      outb(0x70, 0x10);
      unsigned char drives = inb(0x71);
      int a,b;
      a = drives >> 4;
      b = drives & 0xf;
      
      //Check how many floppy drives are installed
      if(a != 4)
      //None. dont waste our time
           return 1;
      if(b != 4)
           floppy_number_drives = 1;
      else
           floppy_number_drives = 2;
           
      kprintf("We got %d Floppy(s)!\n",floppy_number_drives);
      
      //Init to primary floppy
      floppy_base = 0x03f0;
      
      //start_task(floppy_timer,0); //THIS NEEDS TO BE A SYSCALL...BUT WE CANT START CHILD-CHILD PROCESSES IN K LAND
      
      register_interrupt_handler(6, &floppy_handler);
      //kprintf("IRQ6 set up\n");

      if(floppy_reset(floppy_base))
           kprintf("FDC reset was not completed successfully...\n");
      
      return 0;
}

void floppy_wait_for_interrupt()
{
     while(!floppy_recieved_int);
     floppy_recieved_int = 0;
     
     kprintf("GOT ONE! (key point?)\n");
     return;
}

void floppy_write_cmd(int base, char cmd) {
    int i; // do timeout, 60 seconds - THIS IS PROBABLY FAR TOO LONG
    for(i = 0; i < 600; i++) {
        timer_wait(10); // sleep 10 ms
        if(0x80 & in8_p((base+FLOPPY_MSR))) {
            return (void) out8_p(base+FLOPPY_FIFO, cmd);
        }
    }
    kprintf("floppy_write_cmd: timeout");   //CHANGE ME TO REINIT THE FUNCTION
}

unsigned char floppy_read_data(int base) {

    int i;
    for(i = 0; i < 600; i++) {
        timer_wait(10);
        if(0x80 & in8_p(base+FLOPPY_MSR)) {
            return in8_p(base+FLOPPY_FIFO);
        }
    }
    kprintf("floppy_read_data: timeout");   //CHANGE ME TO REINIT THE FUNCTION
    return 1;
} 

void floppy_check_interrupt(int base, int *st0, int *cyl) {
   
    floppy_write_cmd(base, FLOPPY_CMD_SENSE_INTERRUPT);

    *st0 = floppy_read_data(base);
    *cyl = floppy_read_data(base);
}

int floppy_calibrate(int base) {

    int i, st0, cyl = -1; // set to bogus cylinder
    
    //kprintf("Floppy motor...\n");
    floppy_motor(base, floppy_motor_on);

    for(i = 0; i < 10; i++) {
        // Attempt to positions head to cylinder 0
        floppy_write_cmd(base, FLOPPY_CMD_RECALIBRATE);
        //kprintf("calibrate\n");
        floppy_write_cmd(base, 0); // MAKE THIS MULTI-DRIVE
        //kprintf("drive 0\n");

        //floppy_wait_for_interrupt();
        timer_wait(100);
        
        //kprintf("yeyeyey boy\n");
        floppy_check_interrupt(base, &st0, &cyl);
        //kprintf("checked\n");
       
        if(st0 & 0xC0) {
            static const char * status[] =
            { 0, "error", "invalid", "drive" };
            kprintf("floppy_calibrate: status = %s\n", status[st0 >> 6]);
            continue;
        }
        
        if(!cyl) { // found cylinder 0 ?
            floppy_motor(base, floppy_motor_off);
            return 0;
        }
    }

    kprintf("floppy_calibrate: 10 retries exhausted\n");
    floppy_motor(base, floppy_motor_off);
    return -1;
}


int floppy_reset(int base) 
{
    outb((base + FLOPPY_DOR), 0x00); // disable controller
    timer_wait(2);                   // 20 microsecond wait
    outb((base + FLOPPY_DOR), 0x0C); // enable controller
    
    //kprintf("waiting for interrupt..\n");
    //floppy_wait_for_interrupt(); // sleep until interrupt occurs
    timer_wait(100);
    //kprintf("we g0tz in interrupt from FDC.\n");
    
    floppy_wait_for_interrupt();
    int st0, cyl; // These value must be read, but can be ignored
    floppy_check_interrupt(base, &st0, &cyl);
    
    // set transfer speed 500kb/s
    //outb((base + FLOPPY_CCR), 0x00);

    // These value are for 1.44mb floppy
    floppy_write_cmd(base, FLOPPY_CMD_SPECIFY);
    floppy_write_cmd(base, 0xdf); /* steprate = 3ms, unload time = 240ms */
    floppy_write_cmd(base, 0x02); /* load time = 16ms, no-DMA = 0 */

    // This is incase of failure
    //kprintf("lets calibrate the FDC\n");
    if(floppy_calibrate(base)) return 1;
    //kprintf("braaap\n");
    return 0;
}

void floppy_motor(int base, int onoff)
{

    if(onoff) {
        if(!floppy_motor_state) {
            // turn on motor
            out8_p(base + FLOPPY_DOR, 0x1c);
            //kprintf("Floppy motor turning on\n");
            timer_wait(100); // Wait for it...
        }
        floppy_motor_state = floppy_motor_on;
    } else {
        if(floppy_motor_state == floppy_motor_wait) {
            kprintf("floppy_motor: strange, fd motor-state already waiting..\n");
        }
        floppy_motor_ticks = 300; //3 seconds
        floppy_motor_state = floppy_motor_wait;
    }
} 

void floppy_motor_kill(int base)
{
    out8_p(base + FLOPPY_DOR, 0x0c);
    //kprintf("Floppy motor turning off\n");
    floppy_motor_state = floppy_motor_off;
} 

int floppy_seek(int base, unsigned cyli, int head) {

    unsigned i, st0, cyl = -1;

    floppy_motor(base, floppy_motor_on);

    for(i = 0; i < 10; i++) {
        // Attempt to position to given cylinder
        // 1st byte bit[1:0] = drive, bit[2] = head
        // 2nd byte is cylinder number
        floppy_write_cmd(base, FLOPPY_CMD_SEEK);
        floppy_write_cmd(base, head<<2);
        floppy_write_cmd(base, cyli);

        //floppy_wait_for_interrupt();
        timer_wait(100);
        floppy_check_interrupt(base, &st0, &cyl);

        if(st0 & 0xC0) {
            static const char * status[] =
            { "normal", "error", "invalid", "drive" };
            kprintf("floppy_seek: status = %s\n", status[st0 >> 6]);
            continue;
        }

        if(cyl == cyli) {
            floppy_motor(base, floppy_motor_off);
            return 0;
        }

    }

    kprintf("floppy_seek: 10 retries exhausted\n");
    floppy_motor(base, floppy_motor_off);
    return -1;
} 

//DMA stuff
static void floppy_dma_init(floppy_dir dir) {

    union {
        unsigned char b[4]; // 4 bytes
        unsigned long l;    // 1 long = 32-bit
    } a, c; // address and count

    a.l = (unsigned) &floppy_dmabuf;
    c.l = (unsigned) floppy_dmalen - 1;

    // check that address is at most 24-bits (under 16MB)
    // check that count is at most 16-bits (DMA limit)
    // check that if we add count and address we don't get a carry
    // (DMA can't deal with such a carry, this is the 64k boundary limit)
    if((a.l >> 24) || (c.l >> 16) || (((a.l&0xffff)+c.l)>>16)) {
        kprintf("floppy_dma_init: static buffer problem\n");
    }

    unsigned char mode;
    switch(dir) {
        case floppy_dir_read:
             mode = 0x46; 
        break;
        case floppy_dir_write: 
             mode = 0x4a;
        break;
        default: 
             kprintf("floppy_dma_init: invalid direction");
        return;
    }

    out8_p(0x0a, 0x06);   // mask chan 2

    out8_p(0x0c, 0xff);   // reset flip-flop
    out8_p(0x04, a.b[0]); //  - address low byte
    out8_p(0x04, a.b[1]); //  - address high byte

    out8_p(0x81, a.b[2]); // external page register

    out8_p(0x0c, 0xff);   // reset flip-flop
    out8_p(0x05, c.b[0]); //  - count low byte
    out8_p(0x05, c.b[1]); //  - count high byte

    out8_p(0x0b, mode);   // set mode (see above)

    out8_p(0x0a, 0x02);   // unmask chan 2
} 

int floppy_do_track(int base, unsigned cyl, floppy_dir dir) {
   
    // transfer command, set below
    unsigned char cmd;

    // Read is MT:MF:SK:0:0:1:1:0, write MT:MF:0:0:1:0:1
    // where MT = multitrack, MF = MFM mode, SK = skip deleted
    //
    // Specify multitrack and MFM mode
    static const int flags = 0xC0;
    switch(dir) {
        case floppy_dir_read:
            cmd = FLOPPY_CMD_READ_DATA | flags;
            break;
        case floppy_dir_write:
            cmd = FLOPPY_CMD_WRITE_DATA | flags;
            break;
        default:

            kprintf("FDC: floppy_do_track: invalid direction\n");
            return 0;
    }

    kprintf("FDC: floppy_do_track - seeking heads\n");
    
    // seek both heads
    if(floppy_seek(base, cyl, 0)) return -1;
    
    kprintf("FDC: floppy_do_track - 1/2\n");
    
    if(floppy_seek(base, cyl, 1)) return -1;
    
    kprintf("FDC: floppy_do_track - seeking heads complete\n");

    int i;
    for(i = 0; i < 20; i++) {
        floppy_motor(base, floppy_motor_on);
        
        kprintf("FDC: floppy_do_track - motor turned on\n");

        // init dma..
        floppy_dma_init(dir);
        
        kprintf("FDC: floppy_do_track - DMA initialised\n");
        

        timer_wait(10); // give some time (100ms) to settle after the seeks

        floppy_write_cmd(base, cmd);  // set above for current direction
        floppy_write_cmd(base, 0);    // 0:0:0:0:0:HD:US1:US0 = head and drive
        floppy_write_cmd(base, cyl);  // cylinder
        floppy_write_cmd(base, 0);    // first head (should match with above)
        floppy_write_cmd(base, 1);    // first sector, strangely counts from 1
        floppy_write_cmd(base, 2);    // bytes/sector, 128*2^x (x=2 -> 512)
        floppy_write_cmd(base, 18);   // number of tracks to operate on
        floppy_write_cmd(base, 0x1b); // GAP3 length, 27 is default for 3.5"
        floppy_write_cmd(base, 0xff); // data length (0xff if B/S != 0)
        
        kprintf("FDC: floppy_do_track - commands written\n");
        kprintf("FDC: floppy_do_track - waiting for an interrupt\n");
       
        //floppy_wait_for_interrupt();
        timer_wait(100);
        
        kprintf("FDC: floppy_do_track - interrupt received\n");

        // first read status information
        unsigned char st0, st1, st2, rcy, rhe, rse, bps;
        st0 = floppy_read_data(base);
        st1 = floppy_read_data(base);
        st2 = floppy_read_data(base);
        // CHS values
        rcy = floppy_read_data(base);
        rhe = floppy_read_data(base);
        rse = floppy_read_data(base);
        // bytes per sector, should be what we programmed in
        bps = floppy_read_data(base);
        
        kprintf("FDC: floppy_do_track - status information read\n");

        int error = 0;

        if(st0 & 0xC0) {
            static const char * status[] =
            { 0, "error", "invalid command", "drive not ready" };
            kprintf("FDC: floppy_do_sector: status = %s\n", status[st0 >> 6]);
            error = 1;
        }
        if(st1 & 0x80) {
            kprintf("FDC: floppy_do_sector: end of cylinder\n");
            error = 1;
        }
        if(st0 & 0x08) {
            kprintf("FDC: floppy_do_sector: drive not ready\n");
            error = 1;
        }
        if(st1 & 0x20) {
            kprintf("FDC: floppy_do_sector: CRC error\n");
            error = 1;
        }
        if(st1 & 0x10) {
            kprintf("FDC: floppy_do_sector: controller timeout\n");
            error = 1;
        }
        if(st1 & 0x04) {
            kprintf("FDC: floppy_do_sector: no data found\n");
            error = 1;
        }
        if((st1|st2) & 0x01) {
            kprintf("FDC: floppy_do_sector: no address mark found\n");
            error = 1;
        }
        if(st2 & 0x40) {
            kprintf("FDC: floppy_do_sector: deleted address mark\n");
            error = 1;
        }
        if(st2 & 0x20) {
            kprintf("FDC: floppy_do_sector: CRC error in data\n");
            error = 1;
        }
        if(st2 & 0x10) {
            kprintf("FDC: floppy_do_sector: wrong cylinder\n");
            error = 1;
        }
        if(st2 & 0x04) {
            kprintf("FDC: floppy_do_sector: uPD765 sector not found\n");
            error = 1;
        }
        if(st2 & 0x02) {
            kprintf("FDC: floppy_do_sector: bad cylinder\n");
            error = 1;
        }
        /*if(bps != 0x2) {
            kprintf("FDC: floppy_do_sector: wanted 512B/sector, got %d", (1<<(bps+7)));
            error = 1;
        }*/
        if(st1 & 0x02) {
            kprintf("FDC: floppy_do_sector: not writable\n");
            error = 2;
        }

        if(!error) {
            kprintf("FDC: floppy_do_track - succesful read\n");
            floppy_motor(base, floppy_motor_off);
            kprintf("FDC: floppy_do_track - motor off\n");
            return 0;
        }
        if(error > 1) {
            kprintf("FDC: floppy_do_sector - not retrying..\n");
            floppy_motor(base, floppy_motor_off);
            return -2;
        }
    }

    kprintf("FDC: floppy_do_sector - 20 retries exhausted\n");
    floppy_motor(base, floppy_motor_off);
    return -1;

}

int floppy_read_track(int base, unsigned cyl) {
    return floppy_do_track(base, cyl, floppy_dir_read);
}

int floppy_write_track(int base, unsigned cyl) {
    return floppy_do_track(base, cyl, floppy_dir_write);
} 

int floppy_write(char data, unsigned cylinder)
{
    memset((u8int*)floppy_dmabuf, 0, 0x8000);
    floppy_dmabuf[0] = (char)data;
    return floppy_write_track(floppy_base, cylinder);
}

char floppy_read(unsigned cylinder)
{
    kprintf("FDC: floppy_read - zeroing buffer\n");
    memset((u8int*)floppy_dmabuf, 0, floppy_dmalen);
    kprintf("FDC: floppy_read - calling floppy_read_track\n");
    floppy_read_track(floppy_base, cylinder);
    return (char)floppy_dmabuf;
}

void floppy_change_drives(int drive)
{
     if(drive)
     {
          if(floppy_number_drives < 2)
               floppy_base = 0x03f0;
          
          floppy_base = 370;
     }
     else
     {
          if(drive == 0)
               floppy_base = 0x03f0; //Primary drive
     }
     kprintf("FDC: floppy_change_drives - drive changed\n");
     return;
}

void floppy_timer()
{
    while(1) {
        timer_wait(500);
        if(floppy_motor_state == floppy_motor_wait) {
            floppy_motor_ticks -= 50;
            if(floppy_motor_ticks <= 0) {
                floppy_motor_kill(floppy_base);
                switch_task();
            }
        }
    }
} 

void floppy_handler (registers_t regs)
{
  floppy_recieved_int++;
}
