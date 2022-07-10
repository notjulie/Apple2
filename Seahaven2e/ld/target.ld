/* Apple ][e PRG file linker script.
 *
 */

MEMORY {
    ram (rw) : ORIGIN = 0x0801, LENGTH = 0x97ff
}


SECTIONS {
    INCLUDE c.ld
}

/* Provide imaginary (zero page) registers in the BASIC area. */
__rc0 = 0x0002;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x0002, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x0021, "Inconsistent zero page map.")

/* Set initial soft stack address to end of BASIC area. (It grows down.) */
__stack = 0x9FFF;

OUTPUT_FORMAT {
    /* Tells AppleWin where to place the file's contents. */
    SHORT(0x0801)
    TRIM(ram)
}
