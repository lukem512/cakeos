/* CakeOS */
#include <system.h>
#include <gui/keymaps/keymap-us.h>

extern unsigned long g_csr_x, g_csr_y;
extern unsigned long g_wd, g_ht;
extern int graphical_mode;

extern window_t current_window;
extern int shell_csr_x;
extern int shell_csr_y;

volatile int shift_flag=0;
volatile int caps_flag=0;

volatile char* buffer; //For storing strings
volatile char* buffer2;
volatile u32int kb_count = 0; //Position in buffer
volatile int gets_flag = 0;

unsigned short ltmp;
int ktmp = 0;

static void do_gets();

/* Handles the keyboard interrupt */
static void keyboard_handler(registers_t* regs)
{
    unsigned char scancode;

    //Read scancode
    scancode = inb(0x60);
    
    switch (scancode)
    {
           case 0x3A:
                /* CAPS_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 4;
                outb(0x60,ltmp);
                
                if(caps_flag)
                caps_flag=0;
                else
                caps_flag=1;
                break;
           case 0x45:
                /* NUM_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 2;
                outb(0x60,ltmp);
                break;
           case 0x46:
                /* SCROLL_LOCK LEDS */
                outb(0x60,0xED);
                ltmp |= 1;
                outb(0x60,ltmp);
                break;
           case 60: /* F12 */
                reboot();
                break;
           default:
                break;
    }

    if (scancode & 0x80)
    {
        //Key release
        
        //Left and right shifts
        if (scancode - 0x80 == 42 || scancode - 0x80 == 54)
			shift_flag = 0;
    } else {   
        //Keypress (normal)
        
        //Shift
        if (scancode == 42 || scancode == 54)
		{
			shift_flag = 1;
			return;
		}
        
        //Gets()
        if(kbdus[scancode] == '\n')
        {
             if(gets_flag == 0) do_gets();
             gets_flag++;
             for(;kb_count; kb_count--)
                  buffer[kb_count] = 0;              
        } else {
             if(kbdus[scancode] == '\b')
             {
                  if(kb_count)
                  buffer[kb_count--] = 0;
             } else {
                  buffer[kb_count++] = kbdus[scancode];
             }
                  
        } 
        
        //Print key
        if(graphical_mode == 2)
        {
             if(current_window.id != 0)
             {
                  if(kbdus[scancode] >= 97 && kbdus[scancode] <= 122)
                       plot_char_abs(shell_csr_x, shell_csr_y, (kbdus[scancode]-32), WINDOW_COLOUR_TOPBAR_TEXT, current_window.width, (u32int*)current_window.data);
                  else
                       plot_char_abs(shell_csr_x, shell_csr_y, kbdus[scancode], WINDOW_COLOUR_TOPBAR_TEXT, current_window.width, (u32int*)current_window.data);
                  put_buffer(current_window.x,current_window.y,current_window.width,current_window.height,(u32int*)current_window.data);
             } else {
                  if(kbdus[scancode] >= 97 && kbdus[scancode] <= 122)
                        vgaPutchar (g_csr_x,g_csr_y,(kbdus[scancode]-32),WINDOW_COLOUR_TOPBAR_TEXT);
                  else
                        vgaPutchar (g_csr_x,g_csr_y,(kbdus[scancode]),WINDOW_COLOUR_TOPBAR_TEXT);
                  refresh_screen();
             }
        } else {
        if(graphical_mode == 1)
        {
             if(kbdus[scancode] >= 97 && kbdus[scancode] <= 122)
                  vgaPutchar (g_csr_x,g_csr_y,(kbdus[scancode]-32),1); //A-Z
             else
                  vgaPutchar (g_csr_x,g_csr_y,(kbdus[scancode]),1); //0-9
             refresh_screen();
        } else
             monitor_put(kbdus[scancode]);
        }
        return;
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, &keyboard_handler);
}

//Gets a key
unsigned char getch()
{
     unsigned char getch_char;
     
     if(kbdus[inb(0x60)] != 0) //Not empty
     outb(0x60,0xf4); //Clear buffer
     
     while(kbdus[inb(0x60)] == 0); //While buffer is empty
     getch_char = kbdus[inb(0x60)];
     outb(0x60,0xf4); //Leave it emptying
     return getch_char;
}

char* gets()
{ 
     gets_flag = 0;
     while(gets_flag == 0);
     return (char*)buffer2;
}

static void do_gets()
{
     buffer[kb_count++] = 0; //Null terminated biatch!
     for(;kb_count; kb_count--)
     {
          buffer2[kb_count] = buffer[kb_count];
     }
     return;
}
