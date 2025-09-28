[BITS 16]
[ORG 0x7C00]

%define SECTORS_N KERNEL_SIZE / 512 + 1

cli

mov ax, 0x0
mov ss, ax        ; stack seg init (0x7C00)

xor ax, ax
mov ds, ax        ; data seg init (0x7C00)
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
  mov ah, 0x2       ; read sectors
  int 0x13          ; disc service interrupt
  jc print_read_err ; error check

  ; done check
  dec si
  jz read_complete

  ; next sector
  add di, 0x20    ; next sector of data destination
  mov es, di
  
  inc cl          ; next sector of read
  cmp cl, 18      ; check head change 
  jle read_sectors
  mov cl, 1

  inc dh          ; next head of read
  cmp dh, 1       ; check cylinder change
  jle read_sectors
  xor dh, dh

  inc ch          ; next cylinder
  cmp ch, 79      ; overflow check
  jle read_sectors

print_read_overflow:
  mov bx, read_overflow_msg
  call print
  jmp inf_loop

print_read_err:
  mov bx, read_error_msg
  call print
  jmp inf_loop

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

read_complete:
  mov bx, read_complete_msg
  call print

inf_loop:
  jmp inf_loop

read_error_msg: db "[X] Read error -x-", 0x0A, 0x0D
read_overflow_msg: db "[X] READ OVERFLOW 0o0", 0x0A, 0x0D
read_complete_msg: db "[OK] Reading kernel completed successfully -w-"

times 510-($-$$) db 0
dw 0xAA55