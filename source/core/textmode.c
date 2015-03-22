/* CakeOS */
/* Text mode VGA functions */
#include <system.h>

extern int graphical_mode;

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr  = (unsigned short *)0xB8000;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;


int g_csr_x = 0;
int g_csr_y = 0;

volatile int shell_csr_x = 1;
volatile int shell_csr_y = 13;
extern window_t current_window;

extern unsigned g_wd, g_ht;

static void scroll()
{

   // Get a space character with the default colour attributes.
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(csr_y >= 25)
   {
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           textmemptr[i] = textmemptr[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++)
       {
           textmemptr[i] = blank;
       }
       // The cursor should now be on the last line.
       csr_y = 24;
   }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
static void move_csr(void)
{
    //Graphical mode
    if(graphical_mode)
    {
    
    return;
    }
    
    //Text mode
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * 80 + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

/* Puts a single character on the screen */
void monitor_put(unsigned char c)
{
    //Graphical mode
    if(graphical_mode)
    {
         if(graphical_mode == 2)
         if(current_window.id != 0)
              plot_char_abs(shell_csr_x, shell_csr_y, c, WINDOW_COLOUR_TOPBAR_TEXT, current_window.width, (u32int*)current_window.data);
         else
              vgaPutchar(g_csr_x,g_csr_y,c,WINDOW_COLOUR_TOPBAR_TEXT);
         else
              vgaPutchar (g_csr_x,g_csr_y,c,1);
         return;
    }
    
    //Text mode
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space then printing
       a blank character */
    if(c == '\b')
    {
        if(csr_x != 0) csr_x--;
        kprintf(" ");
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* != \n*/
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* \n is identical to \r\n in Cake*/
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void monitor_write(unsigned char *text)
{
    //Graphical mode
    if(graphical_mode)
    {
    
    return;
    }
    
    //Text mode
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        monitor_put(text[i]);
    }
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    //Graphical mode
    if(graphical_mode)
    {
    
    return;
    }
    
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Taken from Linux */
void itoa (char *buf, int base, int d)
     {
       char *p = buf;
       char *p1, *p2;
       unsigned long ud = d;
       int divisor = 10;
     
       /* If %d is specified and D is minus, put `-' in the head. */
       if (base == 'd' && d < 0)
         {
           *p++ = '-';
           buf++;
           ud = -d;
         }
       else if (base == 'x')
         divisor = 16;
     
       /* Divide UD by DIVISOR until UD == 0. */
       do
         {
           int remainder = ud % divisor;
     
           *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
         }
       while (ud /= divisor);
     
       /* Terminate BUF. */
       *p = 0;
     
       /* Reverse BUF. */
       p1 = buf;
       p2 = p - 1;
       while (p1 < p2)
         {
           char tmp = *p1;
           *p1 = *p2;
           *p2 = tmp;
           p1++;
           p2--;
       }
}

void kprintf (const char *format, ...)
{
       char **arg = (char **) &format;
       char *p;
       int c;
       unsigned colour;
       char buf[20];
     
       arg++;
     
    //Graphical mode
    if(graphical_mode)
    {
                      
    if(graphical_mode == 2) colour = WINDOW_COLOUR_TOPBAR_TEXT;
    else colour = 1;
    
    while ((c = *format++) != 0)
         {
           if (c != '%')
           {
                //Check if we have a window to write to
                //For now we assume shell
                if(current_window.id != 0)
                     plot_char_abs(shell_csr_x, shell_csr_y, c, colour, current_window.width, (u32int*)current_window.data);
                else
                     vgaPutchar (g_csr_x,g_csr_y,c,colour);
             }
           else
             {
               char *p;
     
               c = *format++;
               switch (c)
                 {
                 case 'd':
                 case 'u':
                 case 'x':
                   itoa (buf, c, *((int *) arg++));
                   p = buf;
                   goto g_string;
                   break;
     
                 case 's':
                   p = *arg++;
                   if (! p)
                     p = "(null)";
     
                 g_string:
                   while (*p)
                     if(current_window.id != 0)
                          plot_char_abs(shell_csr_x, shell_csr_y, *p++, colour, current_window.width, (u32int*)current_window.data);
                     else
                          vgaPutchar (g_csr_x,g_csr_y,*p++,colour);
                   break;
     
                 default:
                     if(current_window.id != 0)
                          plot_char_abs(shell_csr_x, shell_csr_y, *((int *) arg++), colour, current_window.width, (u32int*)current_window.data);
                     else
                          vgaPutchar (g_csr_x,g_csr_y,*((int *) arg++),colour);
                   break;
                 }
             }
    }
    put_buffer(current_window.x,current_window.y,current_window.width,current_window.height,(u32int*)current_window.data);
    //refresh_screen();
    return;
    }
    
    //Text mode
       while ((c = *format++) != 0)
         {
           if (c != '%')
             monitor_put(c);
           else
             {
               c = *format++;
               switch (c)
                 {
                 case 'd':
                 case 'u':
                 case 'x':
                   itoa (buf, c, *((int *) arg++));
                   p = buf;
                   goto string;
                   break;
     
                 case 's':
                   p = *arg++;
                   if (! p)
                     p = "(null)";
     
                 string:
                   while (*p)
                     monitor_put (*p++);
                   break;
     
                 default:
                   monitor_put (*((int *) arg++));
                   break;
                 }
             }
}
}
