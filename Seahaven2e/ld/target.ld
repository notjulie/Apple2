/* Apple ][e PRG file linker script.
 *
 */

MEMORY {
    ram (rw) : ORIGIN = 0x0800, LENGTH = 0x97ff
}


SECTIONS {
    INCLUDE c.ld
}

/*
 Provide imaginary (zero page) registers in the BASIC area.
 This gives a really good rundown of zero page usage:
    https://stason.org/TULARC/pc/apple2/programmer/017-Which-Zero-Page-locations-are-likely-to-be-in-use.html
 For the imaginary registers we need a range of 32 bytes; the range
 0xE0 - 0xFF is only used by basic programs so we should be good
*/

__rc0 = 0x00E0;
INCLUDE imag-regs.ld
ASSERT(__rc0 == 0x00E0, "Inconsistent zero page map.")
ASSERT(__rc31 == 0x00FF, "Inconsistent zero page map.")

/* Set initial soft stack address to end of BASIC area. (It grows down.) */
__stack = 0x9FFF;

OUTPUT_FORMAT {
    /* Tells AppleWin where to place the file's contents. */
    SHORT(0x0800)
    TRIM(ram)
}
