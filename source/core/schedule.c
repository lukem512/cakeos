/* CakeOS */
/* Scheduling algorithms */

#include <system.h>

extern task_t current_task;

void schedule()
{
     if (!current_task.id)
        return;
        
     if(current_task.time_to_run != 0)
        return;
        
     switch(current_task.priority)
     {
     case PRIO_DEAD:
     current_task.time_to_run = 0;     
     break;
     case PRIO_IDLE:
     current_task.time_to_run = 1;
     break;
     case PRIO_LOW:
     current_task.time_to_run = 10;
     break;
     case PRIO_HIGH:
     current_task.time_to_run = 50;
     break;
     default:
     current_task.time_to_run = 10;
     break;
     }
     
     switch_task();
}
