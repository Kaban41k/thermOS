[GLOBAL inf_loop]
inf_loop:
  jmp inf_loop

[GLOBAL cli]
cli:
  cli
  ret

[GLOBAL sti]
sti:
  sti
  ret

[GLOBAL lidt]
lidt:
  mov eax, dword [esp + 4]
  lidt [eax]
  ret

TSS_SEGMENT equ 0x28

[GLOBAL ltr]
ltr:
  mov ax, TSS_SEGMENT
  ltr ax
  ret

[GLOBAL get_eflags]
get_eflags:
  pushfd
  pop eax
  ret

[GLOBAL set_regs]
set_regs:
  mov eax, 0
  mov ecx, 0xAA
  mov edx, 0xBB
  mov ebx, 0xCC
  mov ebp, 0xDD
  mov esi, 0xFF
  mov edi, 0x11
  ret

[GLOBAL div_zero]
div_zero:
  xor eax, eax
  idiv eax
  ret

[GLOBAL int_n]
int_n:
  int 0x2
  ret

[GLOBAL get_esp]
get_esp:
  mov eax, esp
  ret

extern universal_handler
[GLOBAL collect_context]
collect_context:
  ; context
  push ds
  push es
  push fs
  push gs
  pusha

  ; clear DF flag
  cld

  ; set right segs
  mov eax, 0x10
  mov ds, eax
  mov es, eax
  mov fs, eax
  mov gs, eax

  ; align
  mov ebx, esp
  and esp, 0xFFFFFFF0
  sub esp, 4

  push ebx
  call universal_handler

  ; go back
  mov esp, ebx
  popa
  pop gs
  pop fs
  pop es
  pop ds
  add esp, 8
  iret

[GLOBAL port_read]
port_read:
  mov edx, [esp + 4]
  in al, dx
  ret

[GLOBAL port_write]
port_write:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  out dx, al
  ret

RPL3                   equ 0x3 
USERSPACE_CODE_SEGMENT equ (0x18 | RPL3)
USERSPACE_DATA_SEGMENT equ (0x20 | RPL3)

[GLOBAL userspace_process]
userspace_process:
  mov eax, [esp + 4] ; entry
  mov ecx, [esp + 8] ; stack

  ; context for jmp in userspace with iret

  push USERSPACE_DATA_SEGMENT ; SS
  push ecx                    ; esp
  pushfd                      ; EFLAGS
  
  pop ecx
  and ecx, 0xFFFFCFFF         ; IOPL == 0
  or ecx, 0x00000200          ; IF == 1
  push ecx
  
  push USERSPACE_CODE_SEGMENT ; CS
  push eax                    ; EIP

  mov eax, USERSPACE_DATA_SEGMENT
  mov ds, eax
  mov es, eax
  mov fs, eax
  mov gs, eax

  iret

[GLOBAL userspace_syscall]
userspace_syscall:
  int 0xFA
  ret