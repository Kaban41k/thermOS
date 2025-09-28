SECTIONS {
    . = 0x7C00;

    .boot : {
        .tmp/boot.o (.text);
    }

    .kernel : ALIGN(512) {
        *(.text);
        *(.rodata);
        *(.rodata.*);
        *(.data);
        *(.bss);
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note.gnu.property)
        *(.comment)
    }
}
