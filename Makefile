# =============================================================================
# Variables

# Build tools
KERNEL_SIZE = 16384
NASM = nasm -f bin -dKERNEL_SIZE=$(KERNEL_SIZE)


# =============================================================================
# Tasks

all: clean build test

.tmp/boot.o:
	$(NASM) -felf src/boot.asm -o .tmp/boot.o

.tmp/kernel.o:
	gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector -c src/kernel.c -o .tmp/kernel.o

.tmp/os.elf: .tmp/boot.o .tmp/kernel.o
	ld -m elf_i386 .tmp/boot.o .tmp/kernel.o -T link.ld -o .tmp/os.elf

.tmp/os.bin: .tmp/os.elf
	objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin	

	dd if=/dev/zero of=os.img bs=1024 count=1440
	dd if=.tmp/os.bin of=.tmp/os.img conv=notrunc
	
.tmp/boot.bin: src/boot.asm
	$(NASM) src/boot.asm -o .tmp/boot.bin

boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	dd if=foo of=boot.img conv=notrunc seek=1

build: .tmp/os.bin

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda .tmp/os.img -monitor stdio -device VGA -display gtk

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda .tmp/os.img -monitor stdio -device VGA -display gtk -s -S &
	gdb

.PHONY: all build clean test debug
