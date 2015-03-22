/* CakeOS */
#include <system.h>

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
    u8int ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outb_p(u16int port, u8int value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
    timer_wait(3);
}


u8int inb_p(u16int port)
{
    u8int ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    timer_wait(3);
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outw(u16int port, u16int value)
{
    __asm__ __volatile__ ( "outw %w0, %1" : : "a" (value), "id" (port) );
}

void outl (u16int port, u32int val)
{
     __asm__ volatile ("outl %0,%1":: "a" (val), "d" (port));
}

u32int inl(u16int port)
{
     static u32int ret;
     __asm__ volatile ("inl %1,%0":: "a" (ret), "dN" (port));
     return ret;
}

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

int strncmp(char s1,char s2,unsigned long n)
{
  unsigned char u1, u2;

  while (n-- > 0)
    {
      u1 = (unsigned char) s1++;
      u2 = (unsigned char) s2++;
      if (u1 != u2)
	return u1 - u2;
      if (u1 == '\0')
	return 0;
    }
  return 0;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    while( *src ) {
	*(dest++) =  *(src++);
    }
    *dest='\0';
    return dest;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
/*char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}*/

int strlen(char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

char* strchr (const char* s,int c)
{
  do {
    if (*s == c)
      {
	return (char*)s;
      }
  } while (*s++);
  return (0);
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
