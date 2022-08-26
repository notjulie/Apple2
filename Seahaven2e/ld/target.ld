/* Apple ][e PRG file linker script.
 *
 */

MEMORY {
   /* origin is at 3 beyond 0x6000 to account for the JMP main instruction */
   ram (rw) : ORIGIN = 0x6004, LENGTH = 0x35FC
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
MEMORY { zp : ORIGIN = __rc31 + 1, LENGTH = 0x90 - (__rc31 + 1) }

/* Set initial soft stack address to end of BASIC area. (It grows down.) */
__stack = 0x9FFF;


OUTPUT_FORMAT {
    /* Tells AppleWin where to place the file's contents. */
    SHORT(0x6000)

    /* JMP to main */
    BYTE(0x4C)
    SHORT(main)
    BYTE(0x00)

    /* the program */
    TRIM(ram)
}
