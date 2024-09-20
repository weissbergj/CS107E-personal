/*
 * Asm routines to access CSRs used by interrupts module
 */
 .attribute arch, "rv64im_zicsr"

.globl interrupts_global_enable
interrupts_global_enable:
    li a0,1<<3
    csrs mstatus,a0
    li a0,1<<11
    csrs mie,a0
    ret

.globl interrupts_global_disable
interrupts_global_disable:
    li a0,1<<3
    csrc mstatus,a0
    li a0,1<<11
    csrc mie,a0
    ret

.globl interrupts_get_mcause
interrupts_get_mcause:
    csrr a0,mcause
    ret

.globl interrupts_get_mepc
interrupts_get_mepc:
    csrr a0,mepc
    ret

.globl interrupts_get_mtval
interrupts_get_mtval:
    csrr a0,mtval
    ret

.globl interrupts_set_mtvec
interrupts_set_mtvec:
    csrw mtvec,a0
    ret
