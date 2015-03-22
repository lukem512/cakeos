/* CakeOS */

void execute_binary(unsigned int addr);

void execute_module_binary(char *name, u32int size);

void execute_module_pe(char *name, u32int size);
int load_pe(u32int offset);

