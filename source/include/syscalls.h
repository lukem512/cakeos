/* CakeOS */
#ifndef SYSCALLS_H
#define SYSCALLS_H

#define SYS_setup 0 //
#define SYS___exit 1 //
#define SYS_putTtyChar 2 //
#define SYS_read 3
#define SYS_write 4
#define SYS___open 5
#define SYS_close 6
#define SYS_wait 7
#define SYS_creat 8
//link
#define SYS_unlink 10
#define SYS_sbrk 11
//chdir
#define SYS_time 13 //
//mknode
//chnode

#define SYS_lseek 19
#define SYS_getpid 20 //

#define SYS_fstat 28

#define SYS_kill 37 //
#define SYS___install_signal_handler 48
#define SYS_profil 98 //
#define SYS_gettimeofday 156
#define SYS___get_program_arguments 184
#define SYS_isatty 191

void init_syscalls();
#endif
