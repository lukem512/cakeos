/* CakeOS */
#include <system.h>

//running task
volatile task_t *current_task;

//linked list
volatile task_t *ready_queue;

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern u32int initial_esp;
extern u32int read_eip();
extern void move_to_pl3();

//Next PID
u32int next_pid = 1;
u32int next_thread = 1;

void init_tasking()
{
    cli();

    //This is going to be our userland stack
    move_stack((void*)0x10000000, 0x2000);

    //kernel
    current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->thread = 0;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->priority = PRIO_LOW;
    current_task->time_to_run = 10;
    current_task->ready_to_run = 1;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    sti();
}

void move_stack(void *new_stack_start, u32int size)
{
  u32int i;
  // Allocate some space for the new stack.
  for( i = (u32int)new_stack_start;
       i >= ((u32int)new_stack_start-size);
       i -= 0x1000)
  {
    //usermode stack
    alloc_frame( get_page(i, 1, current_directory), 0, 1);
  }
  
  //flush TLB
  u32int pd_addr;
  __asm__ __volatile__("mov %%cr3, %0" : "=r" (pd_addr));
  __asm__ __volatile__("mov %0, %%cr3" : : "r" (pd_addr));

  // Old ESP and EBP, read from registers.
  u32int old_stack_pointer; __asm__ __volatile__("mov %%esp, %0" : "=r" (old_stack_pointer));
  u32int old_base_pointer;  __asm__ __volatile__("mov %%ebp, %0" : "=r" (old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  u32int offset            = (u32int)new_stack_start - initial_esp;

  // New ESP and EBP.
  u32int new_stack_pointer = old_stack_pointer + offset;
  u32int new_base_pointer  = old_base_pointer  + offset;

  // Copy the stack.
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  // Backtrace through the original stack, copying new values into
  // the new stack.  
  for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  {
    u32int tmp = * (u32int*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      u32int *tmp2 = (u32int*)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  __asm__ __volatile__("mov %0, %%esp" : : "r" (new_stack_pointer));
  __asm__ __volatile__("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void switch_task()
{
    // Read esp, ebp now for saving later on.
    u32int esp, ebp, eip;
     
    if (!current_task)
        return;
        
              
    __asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
    __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));

    // Read the instruction pointer. We do some cunning logic here:
    // One of two things could have happened when this function exits - 
    //   (a) We called the function and it returned the EIP as requested.
    //   (b) We have just switched tasks, and because the saved EIP is essentially
    //       the instruction after read_eip(), it will seem as if read_eip has just
    //       returned.
    // In the second case we need to return immediately. To detect it we put a dummy
    // value in EAX further down at the end of this function. As C returns values in EAX,
    // it will look like the return value is this dummy value! (0x12345).
    eip = read_eip();

    //0x12345 = magic bit for a task that's just switched
    if (eip == 0x12345)
        return;
    
    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;
    
    // Get the next task to run.
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task) current_task = ready_queue;
    
    /*if (!current_task->ready_to_run) 
    {
         while(!current_task->ready_to_run)
              current_task = current_task->next;
    }     */        
 
    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;
    
    DBGPRINTF(' ')
    DBGPRINTF(current_task->id);

    // Make sure the memory manager knows we've changed page directory.
    current_directory = current_task->page_directory;
    // Here we:
    // * Stop interrupts so we don't get interrupted.
    // * Temporarily puts the new EIP location in ECX.
    // * Loads the stack and base pointers from the new task struct.
    // * Changes page directory to the physical address (physicalAddr) of the new directory.
    // * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    //   switched task.
    // * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    //   the next instruction.
    // * Jumps to the location in ECX (remember we put the new EIP in there).
    __asm__ __volatile__("         \
      cli;                 \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      sti;                 \
      jmp *%%ecx           "
                 : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

int fork()
{
    cli();
    
    task_t *parent_task = (task_t*)current_task;

    //clone directory space
    page_directory_t *directory = clone_directory(current_directory);

    //create stuct for new process
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    new_task->id = next_pid++;
    new_task->thread = 0;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->priority = PRIO_LOW;
    new_task->time_to_run = 10;
    new_task->ready_to_run = 1;
    new_task->page_directory = directory;
    new_task->next = 0;

    //add to linked list
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    //entry point
    u32int eip = read_eip();


    if (current_task == parent_task)
    {
        // We are the parent, so set up the esp/ebp/eip for our child.
        u32int esp; __asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
        u32int ebp; __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        sti();

        return new_task->id;
    }
    else
    {
        // We are the child.
        return 0;
    }

}

void set_task_priority (int prio)
{
     cli();
     current_task->priority = prio;
     sti();
}

int getpid()
{
    //dprintf(" PID: %d ",current_task->id);
    return current_task->id;
}

void start_task(void (*func)(void*), void *arg)
{
  if (fork() == 0)
  {
    func(arg);
    exit();
    for(;;);
  }
}

int kill_task(int pid)
{
     task_t *task_d = 0;
     task_t *task_r = (task_t*)ready_queue;
     for(;task_r->next!=0;task_r=task_r->next)
     {
     if(task_r->id == pid)
          //We got the previous task
          task_d=task_r;
     }
     if(!task_d)
          return 1;
          task_r = task_d->next;
          task_d->next = task_r->next;
          
     kfree((void*)task_r);     
     return 0;
}

void exit() // I don't think this works
{
     cli();
     //Just incase the removal doesn't work 100%
     //we make sure we are using as little time as possible
     current_task->priority = PRIO_DEAD;
     current_task->time_to_run = 0;
     current_task->ready_to_run = 0;
     
     //Find previous task
     task_t *task_d = 0;
     task_t *task_r = (task_t*)ready_queue;
     for(;task_r->next!=0;task_r=task_r->next)
     {
          if(task_r->next == current_task)
          {
               //We got the previous task
               task_d=task_r;
               //break; //Don't bother with the rest of the list
          }
     }
     
     //We didn't find the task
     if(!task_d)
         return;
     task_d->next = current_task->next;

     //Free the memory
     kfree((void*)current_task);
     
     sti(); //Restart Interrupts before switching - stop CPU lockup
     switch_task(); //Don't waste any time
}

int new_thread()
{
    cli();
    
    task_t *parent_task = (task_t*)current_task;

    task_t *new_thread = (task_t*)kmalloc(sizeof(task_t));

    new_thread->id = parent_task->id;
    new_thread->thread = next_thread++;
    new_thread->esp = parent_task->esp;
    new_thread->esp = parent_task->ebp;
    new_thread->eip = 0;
    new_thread->priority = parent_task->priority;
    new_thread->time_to_run = 10;
    new_thread->ready_to_run = 1;
    new_thread->page_directory = parent_task->page_directory;
    new_thread->next = 0;

    //add to linked list
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_thread;

    //entry point
    u32int eip = read_eip();


    if (current_task == parent_task)
    {
        u32int esp; __asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
        u32int ebp; __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
        new_thread->esp = esp;
        new_thread->ebp = ebp;
        new_thread->eip = eip;
        sti();

        return new_thread->id;
    }
    else
    {
        return 0;
    }

}

void switch_to_userland()
{
   /* We need to set up the stack to return execution in userland after an IRET. 
      We do this by loading the PL3 CS and SS into the registers, and then loading
      ESP, EIP and EFLAGS correctly. We then reenable interrupts using the IF flag
      in EFLAGS (as STI causes an exception in PL3) */
   /* __asm__ __volatile__("\
     mov $0x23, %ax; \
     mov %ax, %ds; \
     mov %ax, %es; \
     mov %ax, %fs; \
     mov %ax, %gs; \
     mov %esp, %eax; \
     pushl $0x23; \
     pushl %eax; \
     pushf; \
     pop %eax; \
     or %eax, $0x200; \
     push %eax; \
     pushl $0x1B; \
     push $1f; \
     iret; \
     1:");*/
     
     return;
}

//Make POSIX
int start_thread(task_t *thread, int attr, void (*func)(void*), void *arg)
{
   if (new_thread() == 0)
   {
       func(arg);
       exit();
       for(;;);
   }
   kill_thread(thread);
   return 1;
}

int kill_thread(task_t *thread)
{
     task_t *task_d = 0;
     task_t *task_r = (task_t*)ready_queue;
     for(;task_r->next!=0;task_r=task_r->next)
     {
     if(task_r->id == thread->id && task_r->thread == thread->thread)
          task_d=task_r;
     }
     if(!task_d)
          return 1;
          task_r = task_d->next;
          task_d->next = task_r->next;
          
     kfree((void*)task_r);     
     return 0;
}
