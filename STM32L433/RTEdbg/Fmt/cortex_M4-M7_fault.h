#ifndef RTE_CORTEX_M4_M7_FAULT_H
#define RTE_CORTEX_M4_M7_FAULT_H

/* cortex_M4-M7_fault.h - format definitions for the source file "fault_handler.c" */

/* Format definitions are suitable for both Cortex-M4 and Cortex-M7 core processors */
/* The spreadsheet 'Address calculator - M4,M7.ods' was used to calculate the bit addresses. */

// IN_FILE(EXC_NAMES, "cortex_M4-M7_exceptions.txt")

// MSGN_FATAL_EXCEPTION
// "\nCPU registers"
// "\n  R00:0x%[384:32u]08X, R01:0x%08X, R02:0x%08X, R03:0x%08X"
// "\n  R04:0x%[128:32u]08X, R05:0x%08X, R06:0x%08X, R07:0x%08X"
// "\n  R08:0x%[256:32u]08X, R09:0x%08X, R10:0x%08X, R11:0x%08X"
// "\n  R12:0x%[512:32u]08X,  SP:0x%[96:32u]08X,  LR:0x%[544:32u]08X,  PC:0x%[576:32u]08X"
// "\n  xPSR:0x%[608:32u]08X, Flags: Q=%[-5:1]u, V=%[1]u, C=%[1]u, Z=%[1]u, N=%[1]u"
// <EXC_NAMES "\n     Interrupted exception: #%[-32:6u]d - %[-6:6]Y (vector table address: 0x%[-6:6u](*4)02X)"

// "\n  ICSR: 0x%[0:32u]08X"
// <EXC_NAMES "\n     Active exception: #%[-32:9u]u - %[-9:9u]Y (vector table address: 0x%[-9:9u](*4)02X)"
// <EXC_NAMES "\n     Pending exception: #%[+3:9u]u - %[-9:9u]Y (vector table address: 0x%[-9:9u](*4)02X)"
// "%[+1:1u]{ |\n     Interrupt is pending}Y"
// "%[+0:1u]{ |\n     Services a pending exception}Y"
// "%[+2:1u]{ |\n     SysTick exception is pending}Y"
// "%[+1:1u]{ |\n     PendSV exception is pending}Y"
// "%[+2:1u]{ |\n     NMI exception is pending}Y"

/*------------------------------------------------------------------------------*/
/* Decoding of CFSR register (Configurable fault status register) */
// "\n  CFSR:0x%[64:32u]08X"
/* Individual bits represent information about usage, memory management and bus faults. */
/* The RTEmsg processed values starting from bit 0. */
/* Therefore we start with bit 0 of the CFSR register. */
// "%[-32:8]{ |\n     Memory management fault}Y"
// "%[-8:1]{ |\n        The processor attempted an instruction fetch from a location that does not permit execution}Y"
// "%[1]{ |\n        The processor attempted a load or store at a location that does not permit the operation}Y"
// "%[+1:1]{ |\n        Unstack for an exception return has caused one or more access violations}Y"
// "%[1]{ |\n        Stacking for an exception entry has caused one or more access violations}Y"
// "%[1]{ |\n        A MemManage fault occurred during floating-point lazy state preservation}Y"
// "%[+1:1]{ |\n        Valid fault address.}Y"
// "%[8u]{ |\n     Bus fault}Y"
// "%[-8:1]{ |\n        The processor detects the instruction bus error on prefetching an instruction, but it sets the IBUSERR flag to 1 only if it attempts to issue the faulting instruction.}Y"
// "%[1]{ |\n        A data bus error has occurred, and the PC value stacked for the exception return points to the instruction that caused the fault.}Y"
// "%[1]{ |\n        A data bus error has occurred, but the return address in the stack frame is not related to the instruction that caused the error.}Y"
// "%[1]{ |\n        Unstack for an exception return has caused one or more BusFaults.}Y"
// "%[1]{ |\n        Stacking for an exception entry has caused one or more BusFaults}Y"
// "%[1]{ |\n        A bus fault occurred during floating-point lazy state preservation.}Y"
// "%[+1:1]{\n        Non valid fault address|\n        Valid fault address}Y"
// "%[16]{ |\n     Usage fault}Y"
// "%[-16:1]{ |\n        The processor has attempted to execute an undefined instruction.}Y"
// "%[1]{ |\n        The processor has attempted to execute an instruction that makes illegal use of the EPSR}Y"
// "%[1]{ |\n        The processor has attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value.}Y"
// "%[1]{ |\n        No coprocessor}Y"
// "%[+4:1]{ |\n        The processor has made an unaligned memory access.}Y"
// "%[1]{ |\n        Divide by zero}Y"
/*------------------------------------------------------------------------------*/
// "\n  Offending address:0x%[32:32]08X"

#endif
