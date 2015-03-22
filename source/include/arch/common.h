/* CakeOS */

#ifndef COMMON_H
#define COMMON_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);
void outw(u16int port, u16int value);
u32int inl(u16int port);
void outl (u16int port, u32int val);
void outb_p(u16int port, u8int value);
u8int inb_p(u16int port);
#define out8_p(value,port) outb_p(port,value)
#define in8_p(port) inb_p(port)

void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

int strcmp(char *str1, char *str2);
int strncmp(char s1,char s2,unsigned long n);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(char *src);
char* strchr (const char* s,int c);

#endif // COMMON_H
