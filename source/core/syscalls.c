/* CakeOS */
#include <system.h>

static int unimplemented();
static int profil();
static int is_a_tty();

static void syscall_handler(registers_t* regs)
{      
       __volatile__ void* func_addr;
       __volatile__ int ret;
       
       dprintf(" [In[0x%x]] ",regs->eax);
       
       switch(regs->eax)
       {
          case SYS___exit:
             func_addr = &exit;
          break;

          case SYS_putTtyChar:
             func_addr = &monitor_put;
          break;
          
          case SYS_read:
             func_addr = &getch;
          break;

          case SYS_getpid:
             func_addr = &getpid;
          break;

          case SYS_kill:
             func_addr = &kill_task;
          break;

          case SYS_profil:
             //Returns 0
             func_addr = &profil;
          break;
          
          case SYS_isatty:
             //For text mode OR shell
             func_addr = &is_a_tty;
          break;
          
          default:
             func_addr = &unimplemented;
          break;
       }
            
       __asm__ __volatile__ (" \
               push %1; \
               push %2; \
               push %3; \
               push %4; \
               push %5; \
               call *%6; \
               pop %%ebx; \
               pop %%ebx; \
               pop %%ebx; \
               pop %%ebx; \
               pop %%ebx; "
       : "=a" (ret)
       : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (func_addr));
       
       dprintf(" [Out[0x%x]] ",regs->eax);
       regs->eax = ret;
}

static void test_handler(registers_t* regs)
{
       panic(regs);
}

void init_syscalls()
{
     kprintf("Initialising syscalls...");
     register_interrupt_handler(105, &syscall_handler); //0x69
     kprintf("OK\n");
     kprintf("Initialising driver syscalls...");
     register_interrupt_handler(85, &test_handler); //0x55
     kprintf("OK\n");
}

static int unimplemented()
{
     return 22;
}

static int profil()
{
     //Zero for UNIMPLEMENTED
     return 0;
}

static int is_a_tty()
{
     return 1;
}
