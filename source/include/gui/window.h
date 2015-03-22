/* CakeOS */

#ifndef WINDOW_H
#define WINDOW_H

#define HWND_DESKTOP 0

//Colours
#define WINDOW_COLOUR_BORDER 0x00397D02

#define WINDOW_COLOUR_TOPBAR 0x0083F52C
#define WINDOW_COLOUR_FOCUS_TOPBAR 0x007FFF00
#define WINDOW_COLOUR_TOPBAR_TEXT 0x00397D02

#define WINDOW_COLOUR_BACKGROUND 0x00C5E3BF

typedef struct window
{
        char* name;
        int id;
        int parentid;
        int x;
        int y;
        int width;
        int height;
        unsigned long* data;
        int z;
} window_t;

void CreateWindow(char* windowname, int x, int y, int width, int height, int parent);
void DestroyWindow(int id);

void init_window_manager();

#endif
