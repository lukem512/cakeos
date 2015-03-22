/* CakeOS */
#include <system.h>

volatile int wid = 0;
extern unsigned long g_wd, g_ht;
extern unsigned long g_csr_x,g_csr_y,g_bpp;

extern u32int* double_buffer;

#define MAX_WINDOWS 256

volatile window_t current_window;
volatile window_t* window_list;

void init_window_manager()
{
     //We need to initialise the Desktop
     static window_t window;
     window.name = "Desktop";
     window.x = 0;
     window.y = 0;
     window.z = 0;
     window.width = g_wd;
     window.height = g_ht;
     window.parentid = 0;
     window.id = HWND_DESKTOP;
     window.data = (unsigned long*)double_buffer;
     
     current_window = window;
     
     window_list = (window_t*)kmalloc(sizeof(window_t)*MAX_WINDOWS);
     window_list[window.id] = current_window;
}

static int getnewwid()
{
       wid++;
       return wid;
}

void DestroyWindow(int id)
{
     window_list[id].data = 0;
     window_list[id].name = 0;
     window_list[id].id = 0;
     window_list[id].x = 0;
     window_list[id].y = 0;
     window_list[id].z = 0;
     window_list[id].width = 0;
     window_list[id].height = 0;
     window_list[id].parentid = 0;
}

void CreateWindow(char* windowname, int x, int y, int width, int height, int parent)
{
     static window_t window;
     window.name = windowname;
     window.x = x;
     window.y = y;
     window.z = 1;
     window.width = width;
     window.height = height;
     window.parentid = parent;
     window.id = getnewwid();
     window.data = (unsigned long*)kmalloc((width*height)*(g_bpp/8)); //Creates buffer for window
     
     /* Fill */
     plot_rect(0,10,width,height-10,WINDOW_COLOUR_BACKGROUND,width,(u32int*)window.data);
     
     /* Topbar */
     plot_rect(0,0,width,10,WINDOW_COLOUR_TOPBAR,width,(u32int*)window.data);
     
     /* Border */
     int i;
     for(i=0;i<=width;i++)
          plot_pixel(i,0,WINDOW_COLOUR_BORDER,width,(u32int*)window.data); //Top
     for(i=0;i<=width;i++)
          plot_pixel(i,height-1,WINDOW_COLOUR_BORDER,width,(u32int*)window.data); //Bottom
     for(i=0;i<=height;i++)
     {
          plot_pixel(0,i,WINDOW_COLOUR_BORDER,width,(u32int*)window.data); //Left
          plot_pixel(width-1,i,WINDOW_COLOUR_BORDER,width,(u32int*)window.data); //And right...
     }
     
     /* Title */
     for (i = 0; i < strlen(windowname); i++)
        plot_char_abs(1+(i*8)+1,2,windowname[i],WINDOW_COLOUR_TOPBAR_TEXT,width,(u32int*)window.data);
     write_buffer(x,y,width,height,(u32int*)window.data);
     
     refresh_screen();
     
     //And set window focus
     current_window = window;
     window_list[window.id] = current_window;
}

/*static int ChangeFocusXY(int x, int y, int w, int h)
{      
}

int ChangeFocus(int id)
{
}*/
