/* CakeOS */
/* Prints a font to screen in gmode ;) */
#include <system.h>
#include <gui/fonts/8x8.h>

extern unsigned long g_csr_x, g_csr_y;
extern unsigned g_wd;
extern int shift_flag, caps_flag;
char* font;

extern int shell_csr_y;
extern int shell_csr_x;
extern window_t current_window;

static void getfontchar(unsigned char character);

void vgaPutchar(int x, int y, unsigned char character, int c1)
{
	int i, l;
	int j = x;
	int h = y;
	int c;
	
	getfontchar(character);
	
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<i)))
			{
				c = c1;
				g_write_pixel(j, h, c);
            }
		}
		h++;
		j = x;
	}

    if((g_csr_x+16) >= g_wd)
    {
         g_csr_x=0;
         g_csr_y+=10;
    } else
         g_csr_x+=9;
         
	return;
}

void vgaPutchar_abs(int x, int y, unsigned char character, int c1)
{
	int i, l;
	int j = x;
	int h = y;
	int c;
    
    getfontchar(character);
	
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<i)))
			{
				c = c1;
				g_write_pixel(j, h, c);
            }
		}
		h++;
		j = x;
	}

	return;
}


void plot_char_abs(int x, int y, unsigned char character, int c1, unsigned width, u32int* buffer)
{
	volatile int i, l;
	volatile int j = x;
	volatile int h = y;
	volatile int c;
	
	getfontchar(character);
	
	//For backspaces
	if(font == font_8x8_bspace)
    {
         plot_rect(shell_csr_x,shell_csr_y,9,8,WINDOW_COLOUR_BACKGROUND,width,buffer);
	     c = WINDOW_COLOUR_BACKGROUND;
	     shell_csr_x-=9;
    }
	
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
            c = c1;
			if ((font[l] & (1<<i)))
			{
				plot_pixel(j, h, c, width, buffer);
            }
		}
		h++;
		j = x;
	}
	
	if((shell_csr_x+16) >= current_window.width-1)
    {
         shell_csr_x=3;
         shell_csr_y+=10;
    } else
         shell_csr_x+=9;

	return;
}

static void getfontchar(unsigned char character)
{
    switch(character)
	{
         case 'A':
              if(shift_flag || caps_flag)
              font = font_8x8_A;
              else
              font = font_8x8_a;
         break;
         case 'B':
              if(shift_flag || caps_flag)
              font = font_8x8_B;
              else
              font = font_8x8_b;
         break;
         case 'C':
              if(shift_flag || caps_flag)
              font = font_8x8_C;
              else
              font = font_8x8_c;
         break;
         case 'D':
              if(shift_flag || caps_flag)
              font = font_8x8_D;
              else
              font = font_8x8_d;
         break;
         case 'E':
              if(shift_flag || caps_flag)
              font = font_8x8_E;
              else
              font = font_8x8_e;
         break;
         case 'F':
              if(shift_flag || caps_flag)
              font = font_8x8_F;
              else
              font = font_8x8_f;
         break;
         case 'G':
              if(shift_flag || caps_flag)
              font = font_8x8_G;
              else
              font = font_8x8_g;
         break;
         case 'H':
              if(shift_flag || caps_flag)
              font = font_8x8_H;
              else
              font = font_8x8_h;
         break;
         case 'I':
              if(shift_flag || caps_flag)
              font = font_8x8_I;
              else
              font = font_8x8_i;
         break;
         case 'J':
              if(shift_flag || caps_flag)
              font = font_8x8_J;
              else
              font = font_8x8_j;
         break;
         case 'K':
              if(shift_flag || caps_flag)
              font = font_8x8_K;
              else
              font = font_8x8_k;
         break;
         case 'L':
              if(shift_flag || caps_flag)
              font = font_8x8_L;
              else
              font = font_8x8_l;
         break;
         case 'M':
              if(shift_flag || caps_flag)
              font = font_8x8_M;
              else
              font = font_8x8_m;
         break;
         case 'N':
              if(shift_flag || caps_flag)
              font = font_8x8_N;
              else
              font = font_8x8_n;
         break;
         case 'O':
              if(shift_flag || caps_flag)
              font = font_8x8_O;
              else
              font = font_8x8_o;
         break;
         case 'P':
              if(shift_flag || caps_flag)
              font = font_8x8_P;
              else
              font = font_8x8_p;
         break;
         case 'Q':
              if(shift_flag || caps_flag)
              font = font_8x8_Q;
              else
              font = font_8x8_q;
         break;
         case 'R':
              if(shift_flag || caps_flag)
              font = font_8x8_R;
              else
              font = font_8x8_r;
         break;
         case 'S':
              if(shift_flag || caps_flag)
              font = font_8x8_S;
              else
              font = font_8x8_s;
         break;
         case 'T':
              if(shift_flag || caps_flag)
              font = font_8x8_T;
              else
              font = font_8x8_t;
         break;
         case 'U':
              if(shift_flag || caps_flag)
              font = font_8x8_U;
              else
              font = font_8x8_u;
         break;
         case 'V':
              if(shift_flag || caps_flag)
              font = font_8x8_V;
              else
              font = font_8x8_v;
         break;
         case 'W':
              if(shift_flag || caps_flag)
              font = font_8x8_W;
              else
              font = font_8x8_w;
         break;
         case 'X':
              if(shift_flag || caps_flag)
              font = font_8x8_X;
              else
              font = font_8x8_x;
         break;
         case 'Y':
              if(shift_flag || caps_flag)
              font = font_8x8_Y;
              else
              font = font_8x8_y;
         break;
         case 'Z':
              if(shift_flag || caps_flag)
              font = font_8x8_Z;
              else
              font = font_8x8_z;
         break;
         case 'a':
              font = font_8x8_a;
         break;
         case 'b':
              font = font_8x8_b;
         break;
         case 'c':
              font = font_8x8_c;
         break;
         case 'd':
              font = font_8x8_d;
         break;
         case 'e':
              font = font_8x8_e;
         break;
         case 'f':
              font = font_8x8_f;
         break;
         case 'g':
              font = font_8x8_g;
         break;
         case 'h':
              font = font_8x8_h;
         break;
         case 'i':
              font = font_8x8_i;
         break;
         case 'j':
              font = font_8x8_j;
         break;
         case 'k':
              font = font_8x8_k;
         break;
         case 'l':
              font = font_8x8_l;
         break;
         case 'm':
              font = font_8x8_m;
         break;
         case 'n':
              font = font_8x8_n;
         break;
         case 'o':
              font = font_8x8_o;
         break;
         case 'p':
              font = font_8x8_p;
         break;
         case 'q':
              font = font_8x8_q;
         break;
         case 'r':
              font = font_8x8_r;
         break;
         case 's':
              font = font_8x8_s;
         break;
         case 't':
              font = font_8x8_t;
         break;
         case 'u':
              font = font_8x8_u;
         break;
         case 'v':
              font = font_8x8_v;
         break;
         case 'w':
              font = font_8x8_w;
         break;
         case 'x':
              font = font_8x8_x;
         break;
         case 'y':
              font = font_8x8_y;
         break;
         case 'z':
              font = font_8x8_z;
         break;
         case '0':
              font = font_8x8_0;
         break;
         case '1':
              if(shift_flag)
              font = font_8x8_exclamation_mark;
              else
              font = font_8x8_1;
         break;
         case '2':
              font = font_8x8_2;
         break;
         case '3':
              font = font_8x8_3;
         break;
         case '4':
              if(shift_flag)
              font = font_8x8_dollar;
              else
              font = font_8x8_4;
         break;
         case '5':
              font = font_8x8_5;
         break;
         case '6':
              font = font_8x8_6;
         break;
         case '7':
              font = font_8x8_7;
         break;
         case '8':
              font = font_8x8_8;
         break;
         case '9':
              font = font_8x8_9;
         break;
         case ' ':
              font = font_8x8_space;
         break;
         case '@':
              font = font_8x8_at;
         break;
         case '.':
              font = font_8x8_period;
         break;
         case '[':
              font = font_8x8_left_square_brace;
         break;
         case ']':
              font = font_8x8_right_square_brace;
         break;
         case '$':
              font = font_8x8_dollar;
         break;
         case '!':
              font = font_8x8_exclamation_mark;
         break;
         case '\n':
              g_csr_y+=10;
              g_csr_x=0;
              g_csr_x-=9;
              shell_csr_y+=10;
              shell_csr_x=1;
              shell_csr_x-=9;
              font = font_8x8_space;
         break;
         case '\b':
              if(shell_csr_x - 9 < 1 || g_csr_x - 9 < 1){
              }else{
                   g_csr_x-=9;
                   shell_csr_x-=9;
              }
              font = font_8x8_bspace;
         break;
         case '\t':
              shell_csr_x+=64;
              g_csr_x+=0;
              font = font_8x8_bspace;
         break;
         default:
              font = font_8x8_question_mark;//Question mark for unknown
         break;
    }
}
