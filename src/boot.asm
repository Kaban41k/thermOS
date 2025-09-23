[BITS 16]
[ORG 0x7C00]

cli

xor ax, ax
mov ss, ax        ; stack seg init (0x7C00)
mov ds, ax        ; data seg init (0x7C00)
mov sp, 0x7C00    ; stack pointer (0x0000)

mov ax, 0x7E0     ; temporary variable
mov es, ax        ; extra seg (data destination)
xor bx, bx        ; es offset

read_sector:
  mov al, 1       ; num of sectors for reading
  xor ch, ch      ; start cylinder
  mov cl, 2       ; start sector
  xor dh, dh      ; start head
  mov dl, 0       ; floppy disc number (A:) 

  ;call BIOS
  mov ah, 0x2     ; read sectors
  int 0x13        ; disc service interrupt
  
  jc print_err

  mov bx, 0x7E00
  call print 

  jmp inf_loop



print_err:
  mov bx, err_msg
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


inf_loop:
  jmp inf_loop


err_msg: db "Oh... Read error -.-", 0x0A, 0x0D

times 510-($-$$) db 0
dw 0xAA55