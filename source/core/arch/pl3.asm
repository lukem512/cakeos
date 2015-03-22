;CakeOS
;Code for moving to userland

[GLOBAL _move_to_pl3]

_move_to_pl3:
     mov ax, 0x23 
     mov ds, ax
     mov es, ax
     mov fs, ax
     mov gs, ax
     mov eax, esp
     push 0x23
     push eax
     pushf
     
     ;Re-enable interrupts
     ;STI would cause a fault
     pop eax
     or eax, 0x200
     push eax
     
     push 0x1B
     push exit
     iret
exit:
     ret
