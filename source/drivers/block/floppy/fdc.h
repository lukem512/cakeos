/* CakeOS */

#ifndef FDC_H
#define FDC_H

int initialise_floppy_driver();

int floppy_write(char data, unsigned cylinder);
char floppy_read(unsigned cylinder);
void floppy_timer();

#endif
