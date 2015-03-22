/* CakeOS */
#include <system.h>

static void play_sound(u32int nFrequence);
static void nosound();

//Play sound using built in speaker
static void play_sound(u32int nFrequence) {
	u32int Div;
	u8int tmp;

	Div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (u8int) (Div) );
	outb(0x42, (u8int) (Div >> 8));
	tmp = inb(0x61);
	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

//make is shutup
static void nosound() {
	u8int tmp = (inb(0x61) & 0xFC);
    
	outb(0x61, tmp);
}

//Make a beep
void beep() {
	play_sound(1000);
	timer_wait(10);
	nosound();
}
