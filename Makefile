# =============================================================================
# Variables

# Build tools
KERNEL_SIZE = 65536
NASM = nasm -f bin -dKERNEL_SIZE=$(KERNEL_SIZE)

DEBUG = 1

ifeq ($(DEBUG),1)
	CFLAGS = -DDEBUG -std=c23 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
else
	CFLAGS = -std=c23 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
endif

C_SOURCES = $(wildcard src/*.c)
C_OBJECTS = $(patsubst src/%.c, .tmp/%.o, $(C_SOURCES))


# =============================================================================
# Tasks

all: clean build test

.tmp/%.o: src/%.c
		gcc -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/boot.o: src/boot.asm
		nasm -felf -dKERNEL_SIZE=$(KERNEL_SIZE) src/boot.asm -o .tmp/boot.o

.tmp/os.elf: .tmp/boot.o $(C_OBJECTS) link.ld
		ld -m elf_i386 .tmp/boot.o $(C_OBJECTS) -T link.ld -o .tmp/os.elf

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
