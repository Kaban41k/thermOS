# =============================================================================
# Variables

# Build tools
KERNEL_SIZE = 16384
NASM = nasm -f bin -dKERNEL_SIZE=$(KERNEL_SIZE)


# =============================================================================
# Tasks

all: clean build test result

.tmp/boot.bin: src/boot.asm
	$(NASM) src/boot.asm -o .tmp/boot.bin

boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	dd if=foo of=boot.img conv=notrunc seek=1

build: boot.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -display gtk

result: test
	hexdump -C foo
	hexdump -C -s 0x200 -n $(KERNEL_SIZE) boot.img

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -display gtk -s -S &
	gdb

.PHONY: all build clean test debug

# hexdump -C -s 0x200 -n 1024 boot.img
# hexdump -C foo