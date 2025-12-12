[BITS 16]

START_SECTOR  equ   2
SECTORS_N     equ   KERNEL_SIZE / 512 + 1
MAX_SECTORS   equ   18
MAX_HEADS     equ   1
MAX_CYLINDERS equ   79

%macro READ_OVERFLOW_CHECK 0
  %if ((START_SECTOR + SECTORS_N) / 18) > MAX_CYLINDERS 
    %error "--- READ OVERFLOW --- 0o0"
  %endif
%endmacro


READ_OVERFLOW_CHECK

cli

xor ax, ax
mov ds, ax        ; data seg init (0x7C00)
mov ss, ax        ; stack seg init (0x7C00)
mov sp, 0x7C00    ; stack pointer (0x0000)

mov di, 0x7E0
mov es, di        ; extra seg (data destination)
xor bx, bx        ; es offset

mov al, 1         ; num of sectors for reading
xor ch, ch        ; start cylinder
mov cl, 2         ; start sector
xor dh, dh        ; start head

mov si, SECTORS_N

read_sectors:
  ;call BIOS
  mov ah, 0x2         ; read sectors
  int 0x13            ; disc service interrupt
  jc print_read_err   ; error check

  ; done check
  dec si
  jz read_complete

  ; next sector
  add di, 0x20          ; next sector of data destination
  mov es, di
  
  inc cl                ; next sector of read
  cmp cl, MAX_SECTORS   ; check head change 
  jle read_sectors
  mov cl, 1

  inc dh                ; next head of read
  cmp dh, MAX_HEADS     ; check cylinder change
  jle read_sectors
  xor dh, dh

  inc ch                ; next cylinder
  jmp read_sectors


; print function 
; Using BIOS calls, it displays the string
; 
; Inputs:
;   bx: string pointer
;
; Clobbers: ah, al, bx
print:
  mov ah, 0x0E
  .loop:
    mov al, byte [bx]
    cmp al, 0
    jz .end
    int 0x10
    inc bx
    jmp .loop
  .end:
    ret

print_read_err:
  mov bx, read_error_msg
  call print
err_loop:
  jmp err_loop

read_complete:
  mov bx, read_complete_msg
  call print

; 32 bit mode
lgdt [gdt_descriptor]
cld

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 0x8:next

[BITS 32]
next:
  mov eax, 0x10
  mov ds, eax
  mov ss, eax
  mov es, eax
  mov fs, eax
  mov gs, eax

  mov esp, 0x7C00

extern kernel_entry
call kernel_entry

[GLOBAL inf_loop]
inf_loop:
  jmp inf_loop

gdt_descriptor:
  dw gdt_end - gdt - 1  ; gdt_size - 1
  dd gdt                ; gdt base address

align 8
gdt:      ; global descriptor table
  .null:                dq 0
  csd:
    .limitLo:           dw 0xFF
    .baseLo:            dw 0x0
    .baseMid:           db 0x0
    .P_DPL_S_type:      db 0b10011010
    .G_B_0_AVL_limitHi: db 0b11001111
    .baseHi:            db 0x0

  dsd:
    .limitLo:           dw 0xFF
    .baseLo:            dw 0x0
    .baseMid:           db 0x0
    .P_DPL_S_type:      db 0b10010010
    .G_B_0_AVL_limitHi: db 0b11001111
    .baseHi:            db 0x0
gdt_end:

read_error_msg:    db "[FAILED] Read error -x-", 0x0A, 0x0D
read_complete_msg: db "[  OK  ] Reading kernel completed successfully owo"

times 510-($-$$) db 0
dw 0xAA55
