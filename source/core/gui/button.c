/* CakeOS */
#include <system.h>

//Button
component_t* CreateButton(int x, int y, int w, int h, char text, window_t* window)
{
     plot_rect(x,y,w,h,0x00000000,window.width,(u32int*)window.data);
     
}
