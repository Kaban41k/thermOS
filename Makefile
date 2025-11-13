KERNEL_SIZE = 65536
NASM = nasm -f bin -dKERNEL_SIZE=$(KERNEL_SIZE)

DEBUG = 0

ifeq ($(DEBUG),1)
	CFLAGS = -DDEBUG -std=c23 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
else
	CFLAGS = -std=c23 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
endif

all: clean build test

.tmp/%.o: src/%.c
		gcc -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/%.o: src/%.asm
		nasm -felf -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@


C_SOURCES = $(wildcard src/*.c)
C_OBJECTS = $(patsubst src/%.c, .tmp/%.o, $(C_SOURCES))

ASM_SOURCES = $(wildcard src/*.asm)
ASM_OBJECTS = $(patsubst src/%.asm, .tmp/%.o, $(ASM_SOURCES))

.tmp/os.elf: $(ASM_OBJECTS) $(C_OBJECTS) link.ld
		ld -m elf_i386 $(ASM_OBJECTS) $(C_OBJECTS) -T link.ld -o .tmp/os.elf

.tmp/os.bin: .tmp/os.elf
		objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin

os.img: .tmp/os.bin
		dd if=/dev/zero of=os.img bs=1024 count=1440
		dd if=.tmp/os.bin of=os.img conv=notrunc

build: os.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda .tmp/os.bin -monitor stdio -device VGA -display gtk

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda .tmp/os.bin -monitor stdio -device VGA -display gtk -s -S &
	gdb

.PHONY: all build clean test debug
