/* CakeOS */
#include <pe.h>
#include <system.h>

int load_pe(u32int offset)
{
     PPEHeader_t header = (PPEHeader_t)offset;
     //if(header->e_magic != (unsigned short)"MZ")
     //     return 1;
          
     //PPEFileHeader_t fheader = (PPEFileHeader_t)(offset+header->e_lfanew);
     PPEOptionalHeader_t opheader = (PPEOptionalHeader_t)(offset+header->e_lfanew+24);
     
     execute_binary(opheader->AddressOfEntryPoint);
     return 0;
}


void execute_module_pe(char* name, u32int size)
{
                    fs_node_t *modulenode = finddir_fs(fs_root, name);
                    char modulebuffer[size];
                    unsigned int modulesize = read_fs(modulenode, 0, size, modulebuffer);
                    unsigned int moduleptr = kmalloc(modulesize);
                    memcpy((void*)moduleptr,(void*) modulebuffer, modulesize);
                    if(load_pe(moduleptr))
                         kprintf("Error loading %s\n",name);
}
