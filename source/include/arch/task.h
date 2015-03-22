/* CakeOS */

#ifndef TASK_H
#define TASK_H

#include <system.h>

#define PRIO_DEAD 99
#define PRIO_IDLE 0
#define PRIO_LOW 1
#define PRIO_HIGH 2

// This structure defines a 'task' - a process.
struct task
{
    int id;                // Process ID.
    int thread;            // 0 if not a thread or thread ID
    
    u32int esp, ebp;       // Stack and base pointers.
    u32int eip;            // Instruction pointer.
    
    int priority;
    int time_to_run;       // Time left on quanta
    int time_running;      // Time spent running
    int ready_to_run;
    
    int argc;
    char** argv;
    
    page_directory_t *page_directory; // Page directory.
    
    struct task *next;     // The next task in a linked list.
};

typedef struct task task_t;

// Initialises the tasking system.
void init_tasking();

// Called by the timer hook, this changes the running process.
void switch_task();

// Forks the current process, spawning a new one with a different
// memory space.
int fork();

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, u32int size);

// Returns the pid of the current process.
int getpid();

void start_task(void (*func)(void*), void *arg);

int kill_task(int pid);

void exit();

void set_task_priority (int prio);

//Threads
int new_thread();
int start_thread(task_t *thread, int attr, void (*func)(void*), void *arg);
int kill_thread(task_t *thread);

#endif
