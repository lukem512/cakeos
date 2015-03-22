/* CakeOS */
/* VGA functions */

/**
   Initialise the VGA mode
**/
void set_vga_mode(int mode);

/**
   Put X pixel(s) in (x,y) in colour c
**/
void (*g_write_pixel)(unsigned x, unsigned y, unsigned c);
void (*g_put_pixel)(unsigned x, unsigned y, unsigned c);
u32int (*g_read_pixel)(int x, int y);

void put_pixel_256(unsigned x, unsigned y, unsigned c); //vpokeb

void write_pixel_256(unsigned x, unsigned y, unsigned c); //double buffer

void put_pixel_32bpp(unsigned x,unsigned y, unsigned c);

void write_pixel_32bpp(unsigned x,unsigned y, unsigned c);

//Plots a pixel in a buffer
void plot_pixel(unsigned x,unsigned y, unsigned c, unsigned width, u32int* buffer);

//Writes buffer to double buffer memory
void write_buffer(unsigned x, unsigned y, unsigned width, unsigned height, u32int* buffer);

//..straight to LFB
void put_buffer(unsigned x, unsigned y, unsigned width, unsigned height, u32int* buffer);

/**
   Read X pixel(s) in (x,y)
**/
u32int read_pixel_256(int x, int y);

/**
   This changes the current colour to the specified RGB
   value
**/
void set_color(int color, int red, int green, int blue);

/**
   Draw a rectangle
**/
void draw_rect(int x, int y, int w, int h, int c);

//Buffer
void plot_rect(int x, int y, int w, int h, int c, unsigned width, u32int* buffer);

/**
   Draw a line
**/
void line(int x1, int y1, int x2, int y2, int color);

/**
   Refresh screen for double buffering
**/
void (*refresh_screen)(void);
void (*refresh)(u32int x, u32int y, u32int w, u32int h);

void refresh_screen_32bpp();
void refresh_32bpp(u32int x, u32int y, u32int w, u32int h);

void refresh_screen_256();
void refresh_256(u32int x, u32int y, u32int w, u32int h);

/**
   Print a character to screen
**/
void vgaPutchar(int x, int y, unsigned char character, int c1);

void vgaPutchar_abs(int x, int y, unsigned char character, int c1);

void plot_char_abs(int x, int y, unsigned char character, int c1, unsigned width, u32int* buffer);

/**
   Displays a windows BMP file
**/
int (*g_displaybmp)(int x, int y, BMPheader_t* bmp);

int displaybmp(int x, int y, BMPheader_t* bmp);

/**
   Circles
**/
void circle(long xcenter,long ycenter,long radius,long c);

/**
   Other polygons
**/
void polygon(int num_vertices, int *vertices, unsigned colour);

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned colour);

/**
   Prints the mandelbrot set
**/
void mandelbrot();

/**
   Prints the Sierpinski triangle
**/
void sierpinski();

/**
   Bochs VBE
**/
void bochs_vbe_set_mode(unsigned short width, unsigned short height, unsigned short depth);
void bochs_vbe_exit_mode();
