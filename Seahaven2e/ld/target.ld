/* Apple ][e PRG file linker script.
 *
 */


/*=============================================================================
// So here's how I put the program together:
//    6000: JMP main
//    6003: 00
//    6004: JMP exit
//    6007: 00
//    6008: actual origin of linked output
// I put a handy exit function in there so that I can pop into the debugger
// and jump to it to clean up the screen and put me back at the BASIC prompt.
//=============================================================================*/

MEMORY {
   /* origin is at 0x6008... see above */
   ram (rw) : ORIGIN = 0x6008, LENGTH = 0x35FC
}


REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)


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
MEMORY { zp : ORIGIN = __rc31 + 1, LENGTH = 0 }

/* Set initial soft stack address to end of BASIC area. (It grows down.) */
__stack = 0x9FFF;


OUTPUT_FORMAT {
    /* This is not part of the executable, it's just a couple bytes
    // that go in the binary file to tell the receiver where to load
    // it.  This is a format that I use with my hacked version of the
    // AppleWin emulator. */
    SHORT(0x6000)

    /* JMP main, see above for details */
    BYTE(0x4C)
    SHORT(main)
    BYTE(0x00)

    /* JMP exit, see above for details */
    BYTE(0x4C)
    SHORT(exit)
    BYTE(0x00)

    /* so we started at 0x6000 (per the first two bytes above) and
    // added 8 bytes of JMP instructions, so no we are at 0x6008, which is
    // what we declared as the ORIGIN earlier */
    TRIM(ram)
}
