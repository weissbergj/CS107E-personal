/* File: cstart.c
 * --------------
 * Provided to you pre-written. In an upcoming lecture, we
 * will discuss what this code does and why it is necessary.
 */

// linker memmap places these symbols at start/end of bss
extern char __bss_start, __bss_end;

extern void main(void);

// The C function _cstart is called from the assembly in start.s
// _cstart zeroes out the BSS section and then calls main.
// Before starting main(), turns on the blue ACT LED and
// turns off after as a sign of successful completion.
void _cstart(void)
{
    char *bss = &__bss_start;
    char *bss_end = &__bss_end;
    while (bss < bss_end) {
        *bss++ = 0; // zero out all bytes in BSS section
    }

    // Turn on the blue act led (GPIO PD18) before starting main
    volatile unsigned int *PD_CFG2 =  (unsigned int *)0x02000098;
    volatile unsigned int *PD_DATA  = (unsigned int *)0x020000a0;
    unsigned int bit_18 = 1 << 18;
    *PD_CFG2 = (*PD_CFG2 & ~0xf00) | 0x100;  // configure PD18 for output
    *PD_DATA |= bit_18;    // turn on PD18

    main();

    *PD_DATA &= ~bit_18;    // turn off after main finishes normally
}
