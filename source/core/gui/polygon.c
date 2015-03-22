/* CakeOS */
#include <system.h>

//Shamelessly modified from brackeem VGA
//Number of vertices; Array of vertices; Colour
void polygon(int num_vertices, int *vertices, unsigned colour) 
{
  int i;

  for(i=0; i < num_vertices-1; i++)
  {
    line(vertices[(i<<1)+0],
         vertices[(i<<1)+1],
         vertices[(i<<1)+2],
         vertices[(i<<1)+3],
         colour);
  }
  line(vertices[0],
       vertices[1],
       vertices[(num_vertices<<1)-2],
       vertices[(num_vertices<<1)-1],
       colour);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned colour)
{
     //int vertices[6]={x1,y1, x2,y2, x3,y3};
     //polygon(3,vertices,colour);
     line(x1,y1,x2,y2,colour);
     line(x1,y1,x3,y3,colour);
     line(x2,y2,x3,y3,colour);
}
