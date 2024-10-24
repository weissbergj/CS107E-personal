/* File: backtrace.c
 * -----------------
 * ***** TODO: add your file header comment here *****
 */
#include "backtrace.h"
#include "mango.h"
#include "printf.h"
#include "symtab.h"

// helper function implemented in file backtrace_asm.s
extern unsigned long backtrace_get_fp(void);

int backtrace_gather_frames(frame_t f[], int max_frames) {
    /***** TODO: Your code goes here *****/
    return 0;
}

void backtrace_print_frames(frame_t f[], int n) {
    char labelbuf[128];

    for (int i = 0; i < n; i++) {
        symtab_label_for_addr(labelbuf, sizeof(labelbuf), f[i].resume_addr);
        printf("#%d 0x%08lx at %s\n", i, f[i].resume_addr, labelbuf);
    }
}

void backtrace_print(void) {
    int max = 50;
    frame_t arr[max];

    int n = backtrace_gather_frames(arr, max);
    backtrace_print_frames(arr+1, n-1);   // print frames starting at this function's caller
}


long __stack_chk_guard; /**** TODO: choose your canary value ****/

void __stack_chk_fail(void)  {
    /***** TODO: Your code goes here *****/
    while (1); // noreturn function must not return control to caller
}
