/* CakeOS */
/* Rectangle drawing functions */
#include <system.h>

extern unsigned fb_seg;
extern u8int* double_buffer;

/**
   OPTIMISATION NEEDED
**/

void draw_rect(int x, int y, int w, int h, int c)
{
     for(int j=y; j < (y+h); j++)
          for(int i=x; i < (x+w); i++)
               g_write_pixel(i,j,c);
               
     refresh_screen();
}

void plot_rect(int x, int y, int w, int h, int c, unsigned width, u32int* buffer)
{
     for(int j=y; j < (y+h); j++)
          for(int i=x; i < (x+w); i++)
               plot_pixel(i,j,c, width, buffer);
}
