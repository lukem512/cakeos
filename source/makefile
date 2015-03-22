CFLAGS = -O2 -Wall -std=c99 -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -Wno-system-headers
CPPFLAGS = -O2 -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -Wno-system-headers

all: kernel.bin

strip: kernel.bin
	strip.exe kernel.bin

kernel.bin: core/arch/boot.o main.o textmode.o common.o cpu.o panic.o core/arch/gdt.o core/arch/interrupt.o descriptor_tables.o timer.o paging.o ordered_array.o core/arch/process.o task.o isr.o schedule.o fs.o initrd.o beep.o binary.o syscalls.o vga.o pokex.o rect.o font.o keyb.o math.o line.o window.o ps2.o circle.o bochsvbe.o pci.o bmp.o polygon.o kheap.o elf.o
	djecho.exe "[LINK] kernel.bin"
	ld.exe -T link.ld -o kernel.bin core/arch/boot.o main.o textmode.o common.o cpu.o panic.o core/arch/gdt.o core/arch/interrupt.o descriptor_tables.o timer.o paging.o ordered_array.o core/arch/process.o task.o isr.o schedule.o fs.o initrd.o beep.o binary.o syscalls.o vga.o pokex.o rect.o font.o keyb.o math.o line.o window.o ps2.o circle.o bochsvbe.o pci.o bmp.o polygon.o kheap.o elf.o
 
main.o: core/main.c
	djecho.exe "[GCC ] main"
	gcc.exe $(CFLAGS) -c core/main.c
	
textmode.o: core/textmode.c
	djecho.exe "[GCC ] textmode"
	gcc.exe $(CFLAGS) -c core/textmode.c
	
common.o: core/arch/common.c
	djecho.exe "[GCC ] common"
	gcc.exe $(CFLAGS) -c core/arch/common.c
	
cpu.o: core/arch/cpu.c
	djecho.exe "[GCC ] cpu"
	gcc.exe $(CFLAGS) -c core/arch/cpu.c
	
panic.o: core/arch/panic.c
	djecho.exe "[GCC ] panic"
	gcc.exe $(CFLAGS) -c core/arch/panic.c
	
descriptor_tables.o: core/arch/descriptor_tables.c
	djecho.exe "[GCC ] descriptor_tables"
	gcc.exe $(CFLAGS) -c core/arch/descriptor_tables.c
	
kheap.o: core/arch/kheap.c
	djecho.exe "[GCC ] kheap"
	gcc.exe $(CFLAGS) -c core/arch/kheap.c
	
paging.o: core/arch/paging.c
	djecho.exe "[GCC ] paging"
	gcc.exe $(CFLAGS) -c core/arch/paging.c
	
timer.o: core/arch/timer.c
	djecho.exe "[GCC ] timer"
	gcc.exe $(CFLAGS) -c core/arch/timer.c

isr.o: core/arch/isr.c
	djecho.exe "[GCC ] isr"
	gcc.exe $(CFLAGS) -c core/arch/isr.c
	
ordered_array.o: core/ordered_array.c
	djecho.exe "[GCC ] ordered_array"
	gcc.exe $(CFLAGS) -c core/ordered_array.c
	
task.o: core/arch/task.c
	djecho.exe "[GCC ] task"
	gcc.exe $(CFLAGS) -c core/arch/task.c
	
schedule.o: core/schedule.c
	djecho.exe "[GCC ] schedule"
	gcc.exe $(CFLAGS) -c core/schedule.c
	
fs.o: core/fs.c
	djecho.exe "[GCC ] fs"
	gcc.exe $(CFLAGS) -c core/fs.c
	
initrd.o: core/initrd.c
	djecho.exe "[GCC ] initrd"
	gcc.exe $(CFLAGS) -c core/initrd.c
	
beep.o: core/beep.c
	djecho.exe "[GCC ] beep"
	gcc.exe $(CFLAGS) -c core/beep.c
	
binary.o: core/arch/binary.c
	djecho.exe "[GCC ] binary"
	gcc.exe $(CFLAGS) -c core/arch/binary.c
	
syscalls.o: core/syscalls.c
	djecho.exe "[GCC ] syscalls"
	gcc.exe $(CFLAGS) -c core/syscalls.c
	
core/arch/process.o: core/arch/process.asm
	djecho.exe "[ASM ] process"
	nasm.exe -faout core/arch/process.asm 
	
core/arch/interrupt.o: core/arch/interrupt.asm
	djecho.exe "[ASM ] interrupt"
	nasm.exe -faout core/arch/interrupt.asm 
	
core/arch/gdt.o: core/arch/gdt.asm
	djecho.exe "[ASM ] gdt"
	nasm.exe -faout core/arch/gdt.asm 

core/arch/boot.o: core/arch/boot.asm
	djecho.exe "[ASM ] boot"
	nasm.exe -faout core/arch/boot.asm 
	
vga.o: core/vga.c
	djecho.exe "[GCC ] vga"
	gcc.exe $(CFLAGS) -c core/vga.c
	
pokex.o: core/arch/pokex.c
	djecho.exe "[GCC ] pokex"
	gcc.exe $(CFLAGS) -c core/arch/pokex.c
	
rect.o: core/gui/rect.c
	djecho.exe "[GCC ] rect"
	gcc.exe $(CFLAGS) -c core/gui/rect.c
	
font.o: core/gui/font.c
	djecho.exe "[GCC ] font"
	gcc.exe $(CFLAGS) -c core/gui/font.c
	
keyb.o: core/keyb.c
	djecho.exe "[GCC ] keyb"
	gcc.exe $(CFLAGS) -c core/keyb.c
	
ps2.o: core/ps2.c
	djecho.exe "[GCC ] ps2 mouse"
	gcc.exe $(CFLAGS) -c core/ps2.c
	
math.o: core/math/math.c
	djecho.exe "[GCC ] math"
	gcc.exe $(CFLAGS) -c core/math/math.c
	
line.o: core/gui/line.c
	djecho.exe "[GCC ] line"
	gcc.exe $(CFLAGS) -c core/gui/line.c

window.o: core/gui/window.c
	djecho.exe "[GCC ] window"
	gcc.exe $(CFLAGS) -c core/gui/window.c
	
circle.o: core/gui/circle.c
	djecho.exe "[GCC ] circle"
	gcc.exe $(CFLAGS) -c core/gui/circle.c
	
polygon.o: core/gui/polygon.c
	djecho.exe "[GCC ] polygon"
	gcc.exe $(CFLAGS) -c core/gui/polygon.c
	
pci.o: core/pci.c
	djecho.exe "[GCC ] pci"
	gcc.exe $(CFLAGS) -c core/pci.c
	
bochsvbe.o: drivers/video/bochs/bochsvbe.c
	djecho.exe "[GCC ] bochs vbe"
	gcc.exe $(CFLAGS) -c drivers/video/bochs/bochsvbe.c
	
	
	
	
	
elf.o: core/elf.c
	djecho.exe "[GCC ] elf"
	gcc.exe $(CFLAGS) -c core/elf.c
	




rtl8139.o: drivers/network/realtek/rtl8139.c
	djecho.exe "[GCC ] rtl8139"
	gcc.exe $(CFLAGS) -c drivers/network/realtek/rtl8139.c

pe.o: core/pe.c
	djecho.exe "[GCC ] pe"
	gcc.exe $(CFLAGS) -c core/pe.c

fdc.o: drivers/block/floppy/fdc.c
	djecho.exe "[GCC ] fdc"
	gcc.exe $(CFLAGS) -c drivers/block/floppy/fdc.c
	
bmp.o: core/gui/bmp.c
	djecho.exe "[GCC ] bmp"
	gcc.exe $(CFLAGS) -c core/gui/bmp.c
