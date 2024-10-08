// File: codegen.c
// ---------------
// C examples that highlight interesting properties/transformations
// when C compiler generates assembly instructions.

// TO DO: Paste functions one by one in Compiler Explorer and review
// translation to assembly. If you hover your mouse over an assembly
// instruction, Compiler Explorer will give you more explanation of it. Neat!

// Part (a): arithmetic

// negate():  `neg` is a pseudo-instruction. Hover to see the actual instruction.
// compare():  change compare to < or <=. Hover to learn about instructions.
// multiply(): change constant value to see different sequence of assembly.

int negate(int arg) {
    return -arg;
}

int compare(int arg, int arg2) {
    return arg == arg2;
}

int multiply(int arg) {
    return arg * 2;
}


// Part (b): if-else

// Branch instructions are used to make decisions about control flow.
// Look at the compiler's generated assembly for the function below.
// Which branch instruction is used?
// Compare the generated assembly for optimization level -Og to that
// for -O2? The optimized version avoids the branch entirely, how?
int conditional(int arg) {
    if (arg != 0) { // change comparison from 0 to 13, what changes?
        return 1;
    } else {
        return 2;
    }
}

// Part (c): loops

// The function loop() has an ordinary for loop.
// Read generated assembly to see how loop is constructed in assembly.
// Which register is used for variable `i`? for variable `result`?
// Where are the instructions for loop init? loop test? loop increment?
// How is control flow routed?
// Compare the generated assembly at optimization level -Og with
// more aggressively optimized -O2.  What has changed?
int loop(int arg, int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += arg;
    }
    return result;
}

// The function wait_until_low() is a C version of
// "wait for button press" from lab1.
// Read generated assembly and compare levels -Og and -O2.
// At -Og, the generated assembly re-loads from address each iteration
// and all is good. At -O2, it loads only once and thereafter assumes value
// will not change. Uh-oh, a later button press is not detected!
// The proper fix to qualify variable that can change unpredictably
// as `volatile`.  But which variable is it that needs to be declared volatile:
// `state` or `PB_DATA`? Try changed one variable at a time and look
// at what it changes in the generated assembly. Which needs to be
// qualified as `volatile` and why?
void wait_until_low(void) {
    unsigned int *PB_DATA  = (unsigned int *)0x02000040;
    unsigned int state;

    do {
        state = *PB_DATA;       // read current state from PB DATA register
    } while ((state & 1) != 0); // loop while button state is high
}

// Part (d): pointers

// The function below accesses memory using pointer/array
// notation. The code is divided into pairs of statements
// which perform a similar operation, but have subtle differences
// (such as adding a typecast or operating on pointer of
// different pointee type). How do those differences affect
// the generated assembly?
void pointers(int m, int n, int *ptr, char *cptr) {
    *ptr = 66;      // compare to next line
    *cptr = 67;

    ptr[5] = 77;     // compare to next line
    cptr[5] = 78;

    *(cptr + 4) = 88;  // compare to next line
    *((int *)cptr + 4) = 89;

    *(cptr + m) = 96;  // compare to next line
    cptr[n] = 97;

    *(ptr + m) = 1234;  // compare to next line
    ptr[n] = 567;
}


// Part (e): surprises
// If you compile what you believe is valid C code and yet when executing it
// something wacky happens ... get to the bottom of it by examining the
// generated assembly to learn what is _really_ going on.
// The assembly instructions are always the Ground Truth!

// This function compiles to always return 0 -- why?
int precedence(int arg) {
    return arg & 1 == 0;
}

// These last few exhibit "undefined behavior" (violates C language standard)
// C compiler does not reject outright, instead allowed to generate whatever asm it wants
// no_return: missing return statement, what is returned?
// semi_initialized: what if local variable is initialized only on some paths, not all?
// never_initialized: local variable is never initialized, used with junk contents
//                  compare difference of what will happen compiled -Og versus -O2

int no_return(int arg) {
    int result = arg * 17;
}

int semi_initialized(int arg) {
    int result;
    if (arg > 0)
        result = 5;
    return result;
}

int never_initialized(int arg) {
    int result;
    for (int i = 0; i <= 6; i++) {
        result *= arg;
    }
    return result;
}
