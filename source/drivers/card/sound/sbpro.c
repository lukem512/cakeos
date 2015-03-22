/* CakeOS */
#include <system.h>

//Prototypes
static void reset_dsp();
static u8int read_dsp();

volatile int sbc_base = 0;

static int reset_dsp()
{
       int i;
       outb(DSP_RESET,1);
       timer_wait(10); //10ms
       outb(DSP_RESET,0);
       timer_wait(30); //30ms
       for (i = 0; i < 1000 && !(inb(DSP_DATA_AVAIL) & 0x80); i++); //Poll data available register
       if(inb(DSP_READ) != 0xAA)
            return 0; //Poll data read register
       
       return 1;
}

static u8int read_dsp()
{
       int i;
       u8int ret;
       while(i = 0; i < 1000 && !(inb(DSP_DATA_AVAIL) & 0x80); i++);
       ret = inb(DSP_READ);
       return ret;
}

static int write_dsp_command()
{
       int i,timeout;
       datetime_t now = getDatetime();
       timeout = mktime(now);
       for (i = 0; i < 500000 && mktime(getDatetime()) < timeout; i++)
       {
            if ((inb(DSP_STATUS) & 0x80) == 0)
            {    
	             outb(val, DSP_COMMAND);
	             return 1;
            }
       }

       return;
}
