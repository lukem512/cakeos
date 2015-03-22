/* CakeOS */
/* Main kernel entry point */
#include <system.h>

/**
   WIP's:
         - cd command
              - symlink support needed
         - kill command
         - fix tasks (max is 129 atm)
              - heap is issue
              - kfree() doesn't work
**/

u32int initial_esp;
extern u32int placement_address;

extern unsigned g_wd, g_ht;

extern unsigned fb_seg;
extern u8int double_buffer;

extern page_directory_t *current_directory;

extern heap_t* kheap;

extern window_t current_window;


/* This is VERY important */
volatile int graphical_mode = 0;

/* Prototypes of tasks */
void hello_world();
void test_userland();
void sysidle();

void test_dynamic();

/* Drivers */
void init_vga();
void shell();

void kmain(struct multiboot *mbd, u32int initial_stack)
{
     initial_esp = initial_stack; //Set up for stack switch
     
     kprintf("Loading...\n");
     
     init_descriptor_tables();
     kprintf("IDT/GDT initialised...\n");
     
     init_timer(100);
     kprintf("CMOS timer initialised to 100hz...\n");
     
     //Make sure placement_address doesn't destroy
     //the location of our initrd and other modules
     u32int initrd_location = *((u32int*)mbd->mods_addr);
     u32int initrd_end = *(u32int*)(mbd->mods_addr+4);
     placement_address = initrd_end;
     
     //Initialise paging with the memory amount reported by GRUB
     u32int memsz = ((mbd->mem_lower + mbd->mem_upper) * 1024); //Bytes
     init_paging(memsz);
     kprintf("Paging initialised with %dMB of memory...\n",(memsz/1024/1024));
     
     u32int malloc_test = kmalloc(100);
     if(malloc_test != 0)
     {
          kprintf("MMU working as expected:\n\tMemory test assigned 100 bytes at 0x%x...\n",malloc_test);
          kfree((void*)malloc_test);
     }
     else
     kprintf("MMU failed test...\n");
     
     fs_root = init_initrd(initrd_location);
     kprintf("RAMFS initialised...\n");
     
     kprintf("Initialising PCI bus...\n");
     init_pci();
     
     static int mouse;
     mouse = detect_ps2_mouse();
     if(mouse)
     {
          init_ps2_mouse();
          kprintf("PS/2 mouse initialised...\n");
     }else{
          kprintf("No PS/2 mouse found. Looking for alternative...\n");
          //Scan for non-PS/2 mouse here
     }
     
     init_keyboard();
     kprintf("PS/2 keyboard initialised...\n");
     
     //Lets read our Cake.txt file to test
     fs_node_t *fsnode = finddir_fs(fs_root, "cake.txt");
     u8int buf[256];
     u32int sz = read_fs(fsnode, 0, 256, buf);
     for (int j = 0; j < sz; j++)
     monitor_put(buf[j]);
     kprintf("\n");
     
     datetime_t n = getDatetime();
     kprintf("It is now:\n\t%d:%d:%d %d/%d/%d...\n",n.hour, n.min, n.sec, n.day, n.month, n.year);
     
     init_tasking();
     kprintf("Tasking initialised...\n");
     
     //set up syscalls so our applications can do stuff
     //syscalls are set up to IRQ11(int 43)
     init_syscalls();
     kprintf("Syscalls initialised to IRQ11...\n");
     
     /*unsigned int a = kmalloc(8); dprintf("A: %d (0x%x)\n",a,a);
	 unsigned int b = kmalloc(8); dprintf("B: %d (0x%x)\n",b,b);
	 kfree((void*)a);
	 kfree((void*)b); 
     unsigned int c = kmalloc(8); dprintf("C: %d (0x%x)\n",c,c);*/
     
     sti(); //Let multitasking happen baby!
     
     
     //start_task(test_dynamic,0);
     
     //start_task(hello_world,0);
     
     start_task(test_userland,0);
     start_task(sysidle,0); //Will be made more complex - uses up excess CPU time for heat reasons
     
     //set up the VGA driver
     //start_task(init_vga,(int*)VESA_MODE_824);
     
     //start shell
     //start_task(shell,0);
}

void hello_world()
{
     kprintf("Multitasking working...\n");
}

void test_userland()
{
     set_task_priority(PRIO_HIGH);
     if(load_elf(2, 10000)==1)
          kprintf("Bollocks. The ELF relocator broke.");
     kprintf("Cool\n");
     exit(); //THIS DOESN'T FOOKING WORK
}

void test_dynamic()
{
     set_task_priority(PRIO_HIGH);
     if(load_elf(3, 10000)==1)
          kprintf("Bollocks. The ELF relocator broke.");
     kprintf("\n");
}

void sysidle()
{
     set_task_priority(PRIO_LOW);
     for(;;)     __asm__ __volatile__ ("nop");
}







/* Funtions below this line need to be moved */

void shell()
{
     static char* cmd;
     static int c;
     static fs_node_t *fsnode;
     fsnode = fs_root;
     static struct dirent *node = 0;
     char* currentdir = (char*)"root";
     kprintf("\nInitialising shell...\n");
     
     while(1)
     {
          kprintf("cake[%s] $ ",currentdir);
          cmd = gets();
          
          if(strcmp(cmd,"help") == 0)
          {
               kprintf("Command list:\n\tnow - print date/time [now]\n\tmemusage - print memory usage [memusage]\n\thelp - print command list [help]\n\techo - print text [echo hello world!]\n\tls - list files/directories in current directory [ls]\n\tstart - start program [start gui]\n\texit - kill this terminal[exit]\n\tkill - kills a task [kill 32]\n\tabout - about the project [about]\n\tgui - switches to GUI [gui]\n");
          } else {
          if(strcmp(cmd,"now") == 0)
          {
               datetime_t n = getDatetime();
               kprintf("%d:%d:%d %d/%d/%d\n",n.hour, n.min, n.sec, n.day, n.month, n.year);
          } else {
          if(strcmp(cmd,"memusage") == 0)
          {
               kprintf("%dkB\n",get_memory_usage()/1024);
          } else {
          if(strcmp(cmd,"about") == 0)
          {
               kprintf("This version (%s) of Cake built on %s at %s.\nCake is copyright Luke Mitchell and is released under the Creative Commons license.\n",CAKE_VERSION,CAKE_BUILD_DATE,CAKE_BUILD_TIME);
          } else {
          if(strcmp(cmd,"exit") == 0)
          {
               exit();
          } else {
          if(strcmp(cmd,"gui") == 0)
          {
               kprintf("Press any key to continue...\n");
               init_vga((int*)VESA_MODE_824);
          } else {
          if(strcmp(cmd,"ls") == 0)
          {
               int i = 0;
               
               while ( (node = readdir_fs(fsnode, i)) != 0)
               {
                    kprintf(node->name);
                    fs_node_t *tmpfsnode = finddir_fs(fsnode, node->name);

                    if ((tmpfsnode->flags&0x7) == FS_DIRECTORY)
                         kprintf(" (dir)\n");
                    else
                         kprintf("\n");
                    i++;
               }
               if(i == 0)
                    kprintf("This folder is empty.\n");
          } else {
                 
          /* These are commands with arguments */
          
                unsigned char buf[strlen(cmd)]; //We have to overshoot here so we eliminate BoF
                int i = 0;
                int realcmd = 1;
                
                while ((c = *cmd++) != ' ' && c != 0)
                {
                    buf[i] = (unsigned char)c;
                    i++;
                } 
                if(c == 0)
                    realcmd = 0; 
                buf[i] = 0;
                i = 0;
                
                if(realcmd){
                if(strcmp(buf,"echo") == 0)
                {
                     unsigned char buffer[strlen(cmd)-4];
               
                     while ((c = *cmd++) != 0)
                     {
                          buffer[i] = (unsigned char)c;
                          monitor_put(buffer[i]);
                          i++;
                     }
                     kprintf("\n");
                } else {
                if(strcmp(buf,"start") == 0)
                {
                     unsigned char buffer[strlen(cmd)-5];
                     while ((c = *cmd++) != 0)
                     {
                          buffer[i] = (unsigned char)c;
                          i++;
                     }
                     buffer[i] = 0;
                     
                     //This will spawn a thread when shell is converted to App
                     //We also need to do some more checks (e.g - not directory, filetype)
                     if(!finddir_fs(fsnode, buffer))
                          kprintf("File does not exist!");
                     else
                          execute_module_binary(buffer,4096); //This also needs to read from FS what size it is
                     kprintf("\n");
                } else {
                if(strcmp(buf,"cd") == 0)
                {
                     unsigned char buffer[strlen(cmd)-2];

                     while ((c = *cmd++) != 0)
                     {
                          buffer[i] = (unsigned char)c;
                          i++;
                     }
                     buffer[i] = 0; 
                     
                     //TODO: add mountpoint + symlink support
                     fs_node_t *fsnode2 = finddir_fs(fs_root, buffer);
                     if ((fsnode2->flags&0x7) == FS_DIRECTORY){
                          strcpy(currentdir,(char*)buffer);
                          fsnode = fsnode2;
                     }
                     else
                          kprintf("Invalid directory. Type 'ls' for list.\n");
                } else {
                if(strcmp(buf,"kill") == 0)
                {
                     //We need some security for this command
                     if((c = *cmd++) == 0)
                          kprintf("Task zero is not valid\n");
                     
                     if(kill_task(abs((int)c)))
                          kprintf("Task %d was not found",abs((int)c));
                     kprintf("\n");
                } else
                     kprintf("Unknown command. Type 'help' for list.\n");           
                }}}} else
                         kprintf("Unknown command. Type 'help' for list.\n");
          }}}}}}}}
}

void init_vga(int mode)
{
     kprintf("Initialising VGA mode...\n");
     set_task_priority(PRIO_HIGH);
     
     //We disable interrupts to speed up the process of switching modes
     cli();
     //Set up the VGA mode
     set_vga_mode(mode);
     sti();
     
     /*init_window_manager();
     
     CreateWindow("Test Window",450,300,400,300,HWND_DESKTOP);
     CreateWindow("CakeOS Shell",100,100,600,400,HWND_DESKTOP);
     
     kprintf("Welcome to cakeOS in graphical mode!\n");*/
     
     /*fs_node_t *cursornode = finddir_fs(fs_root, "cursor.bmp");
     u8int cursor[2934];
     read_fs(cursornode, 0, 2934, cursor);
     
     BMPheader_t* bmp = (BMPheader_t*) cursor;
     displaybmp(0,0,bmp);*/
     
     //Exit gracefully
     //exit();
}
