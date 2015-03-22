/* CakeOS */
#include <system.h>

extern int graphical_mode;

//Prototypes
static void CirclePoint(long cx,long cy,long x,long y,long c);

void circle(long xcenter,long ycenter,long radius,long c)
{ 
  if(!graphical_mode)
      return;
  long x=0;
  long y=radius;
  long p=(5-radius*4)/4;
  CirclePoint(xcenter,ycenter,x,y,4);
  while(x<y)
  { x++;
    if(p<0)
     {p+=2*x+1;
     }
    else{y--;p+=2*(x-y)+1;}
    CirclePoint(xcenter,ycenter,x,y,(long)c);
  }
  refresh_screen();
}

static void CirclePoint(long cx,long cy,long x,long y,long c)
{ if(x==0)
  { g_write_pixel(cx,cy+y,c);
  g_write_pixel(cx,cy-y,c);
  g_write_pixel(cx+y,cy,c);
  g_write_pixel(cx-y,cy,c);
  }
else if(x==y)
 {g_write_pixel(cx+x,cy+y,c);
 g_write_pixel(cx-x,cy+y,c);
 g_write_pixel(cx+x,cy-y,c);
 g_write_pixel(cx-x,cy-y,c);
 }
else if(x<y)
 {g_write_pixel(cx+x,cy+y,c);
 g_write_pixel(cx-x,cy+y,c);
 g_write_pixel(cx+x,cy-y,c);
 g_write_pixel(cx-x,cy-y,c);
 g_write_pixel(cx+y,cy+x,c);
 g_write_pixel(cx-y,cy+x,c);
 g_write_pixel(cx+y,cy-x,c);
 g_write_pixel(cx-y,cy-x,c);
 }
}
