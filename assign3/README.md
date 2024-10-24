The printf function is located in printf.c. It is an elementary implementation. Use printf freely to test that it works. Printing pinout is a good test. The disassembler was implemented in the same file. This functions for the entire RV32/64G instruction set listings in chapter 24, page 129-136 of https://cs107e.github.io/readings/riscv-spec-20191213.pdf#page=34. This includes the RV32I base instruction set, RV64I base, RV32/RV64 Zifencei extension, RV32M, RV64M, RV32A, RV64A, RV32F, RV64F, RV32D, RV64D, RV32Q, and RV64Q. The function is not error-free or robust; still, it functions and passes test cases in the file test_strings_printf.c. Please run make test to verify.