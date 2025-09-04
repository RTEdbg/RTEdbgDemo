## Code size optimized Exception Handler for devices with ARM Cortex-M4 or M7

This demo shows how to perform a core dump on an ARM Cortex-M4/M7 processor with additional minimal code in an embedded system where the CPU core registers are logged as binary data. The logged data is decoded (printed) on the host. No printf() functionality and no printf strings are necessary in the embedded system firmware.

**Note:** This fault handler is optimized for bare-metal projects. Use the **[ARM Cortex-M4 / M7 Exception Handler](https://github.com/RTEdbg/RTEdbgDemo/blob/master/Simple_STM32H743/RTEdbg/Demo_code/Cortex_M4-M7_fault_handler.md)** handler version for RTOS-based projects. See also **[List of exception handlers](https://github.com/RTEdbg/RTEdbgDemo/blob/master/STM32H743/RTEdbg/Demo_code/Fault_handler.md)**.

This example shows the logging and formatting capabilities of the **[RTEdbg](https://github.com/RTEdbg/RTEdbg)** toolkit. The format definitions are a superset of the printf() string syntax. The example shows how to output data in a user-friendly format, even when the values in the circular buffer are not in the desired order and certain CPU core registers contain bit fields. 

### A Comparison: ARM Cortex-M4/M7 Core Dump with RTEdbg vs. printf/sprintf
In the case where we log the processor registers (core dump), only 100 bytes of data are stored in the RTEdbg circular memory. Only 32 bytes of additional code are required to prepare the data and 20 bytes of stack are required to execute the RTEdbg library function that logs the data. <br>
If the data were printed using sprintf, around 820 bytes of data (the sprintf output) would be stored in the circular buffer. A larger space consumption in the circular buffer means less room for other data and therefore a shorter history of events before the exception.
In addition, the format strings would occupy about 3 kB of program memory, and a function would also be needed to format the data with sprintf(), requiring considerable stack space and additional program memory. Very stripped-down versions of the sprintf() function occupy around 1 kB of program memory, while fully featured versions can take more than 10 kB.

### Repository structure
In this repository, the demo code for logging the processor registers is located in the following files:
* `./Core/Startup/startup_stm32____.s` &rarr; Default_Handler - default exception handler (assembly)
* `./Core/Src/main.c` &rarr; log_exception() - the C part of the exception handler
* `./RTEdbg/Fmt/cortex_M4_M7_fault_fmt.h` &rarr; Format definition for this exception handler

### Description 
This demo shows a relatively simple way to log processor registers when a system exception or unhandled interrupt occurs. This basic version of the exception handler is suitable for bare-metal projects that only use the Main Stack Pointer (MSP). Both functions together (`Default_Handler` and `log_exception`) use only 32 bytes of program memory.

This is a basic **fault handler** that logs a **core dump** to help you find the cause of a fault. The core dump contains a snapshot of the CPU registers and state at the exact moment a critical error or crash occurs. You can extend it to log additional information, such as important global variables, state machine variables, or the name and data of the current RTOS task. If your most important global variables are grouped in a single structure, you can log the entire structure with just one additional `RTE_MSGN()` macro.

In addition to general-purpose registers, every processor has a number of system registers. For bare-metal systems, it's important to focus on the most essential ones. Including too many details on a single topic leaves less space for the rest of the logged data before the exception is triggered, which reduces the amount of history.

In `startup_stm32____.s` the added code for the exception handler (see `Default_Handler`) replaces the infinite loop of the default exception handler. This  handler is triggered for all exceptions or interrupts that do not have a dedicated service routine implemented by the programmer. As a result, logging is performed not only for HardFault and NMI, but also for any interrupt without an associated service routine.

The complete logged data with complete history (contents of the g_rtedbg structure) can be, for example, stored in internal or external flash memory for later retrieval on-site, or alternatively transmitted to host via an IoT connection.

When generating the initial demo code with the STM32CubeMX, the generation of default handlers for interrupts that do not have their own service routines implemented in the project was disabled - see "STM32CubeMX => NVIC Mode and Configuration => Code Generation - Enabled interrupt table".

**Notes:**
1. Even a small data-logging buffer (e.g. 1 kB) can log information about fatal errors and what the code was doing before the error occurred. This allows for efficient 'post-mortem' analysis. Additional information can be logged, if defined by the programmer, ranging from RTOS internals to task or system stack details and important system variables. A complete data structure or buffer can be logged with a single call to `RTE_MSGN()`.
2. When the exception handler is entered, the registers R0-R3, R12, LR, PC, and xPSR are already stored on the stack. For robust programming, it is advisable to check if there is enough space on the stack for additional registers before saving them to avoid another fatal error that could prevent the completion of the data logging.
3. For projects with an RTOS where the PSP (Program Stack Pointer) is used during task execution, the following must also be considered. When the processor throws an exception, unless it is a tail-chained or late-arriving exception, the processor pushes information onto the current stack. Therefore, when entering the exception handler, it must first check which stack pointer (MSP or PSP) was in use before the exception or interrupt was thrown. In this case, exception logging can be done as shown in the exception handler demo for processors with ARM Cortex-M4 and M7 cores - see **[List of exception handlers](https://github.com/RTEdbg/RTEdbgDemo/blob/master/STM32H743/RTEdbg/Demo_code/Fault_handler.md)**.
4. If you are interested in how data is passed from assembly code to C code, read the [ARM Procedure Call Standard](https://developer.arm.com/documentation/den0013/d/Application-Binary-Interfaces/Procedure-Call-Standard).
5. If you want to learn more about exception handlers, read [Segger AN00016 - Analyzing HardFaults on Cortex-M CPU](https://www.segger.com/downloads/application-notes/AN00016).

<br>

## Snippets of code from the demo

**Example of exception handler for devices with a ARM Cortex-M4/M7 core - see startup_stm32____.s**
```asm
/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
    .global   Default_Handler
Default_Handler:
/*+++++++++++++++++++++++++++++++++++++++++++++*/
/* DEFAULT EXCEPTION HANDLER - See the 'Exception_handler_Cortex-M4_M7.md' for details.
 *
 * This is a simplified version of the exception handler. It assumes that the Main Stack
 * Pointer (MSP) is used for all operations. The handler is not suitable for RTOS-based
 * firmware because the Process Stack Pointer (PSP) is used for the RTOS tasks. When an
 * exception occurs while a task is running, the automatic stacking and unstacking stages
 * will use the PSP, not the MSP that is used during the exception handler execution.
 * This exception handler should only be used for bare-metal systems where the PSP is
 * not enabled.
 *
 * The stack is used as a buffer for the data that is to be logged. It is recommended
 * to check if there is enough space before pushing additional content onto the stack.
 * R0-R3, R12, LR, PC and xPSR are pushed automatically before entering exception handler.
 * The CPU core also pushes the floating-point unit registers to the stack if the FPU
 * was used, and it performs a double-word address SP alignment if alignment is enabled.
 * The pre-exception stack pointer (SP) cannot be calculated with a simple offset from
 * the saved SP value (value logged here). This must be taken into account when analyzing
 * logged data.
 */

    /* Prepare the additional core register values. */
    ldr r3, =0xE000ED00     // System control block address (SCB)
    ldr r2, [r3, #0x28]     // Configurable fault status register (CFSR)
    ldr r1, [r3, #0x38]     // Get offending address from Bus Fault Address Register (BFAR)
                            // BFAR and MMFAR use the same hardware on Cortex-M4/M7
    ldr r0, [r3, #0x04]     // Interrupt control and state register (ICSR)
    mov r3, sp              // Stack pointer post-exception entry
    push {r0-r11}   // Push the remaining core registers (r4-r11) and additional values (r0-r3)

    /* The function for logging the stack contents, where the processor has stored
     * the registers, is started from C because it is easier to implement that way.
     * A programmer does not need to know the details of the implementation of
     * RTEdbg functions and macros for logging data.
     *
     * The SP address where the registers are is passed to Exception_handler() in
     * register R1 and the ICSR (Interrupt control and state register) value in R0.
     */
    mov r1, sp
    b log_exception
/*+++++++++++++++++++++++++++++++++++++++++++++*/
  .size  Default_Handler, .-Default_Handler
```

<br>

**Data logging for the exception handler in the startup file (main.c)**
```C
/**
 * @brief Logs data for the exception handler in the startup_stm32___.s
 *        This is an example of how to save a complete core dump with a
 *        single call to a logging function.
 *
 * @param icsr_reg  Value of the ICSR (Interrupt control and state register)
 * @param sp        Address of data to be logged (Stack pointer address)
 *
 * For more details, refer to the 'Exception_handler_Cortex-M4-M7.md'
 * document in the main repository folder.
 */

void __NO_RETURN __attribute__((naked)) log_exception(uint32_t icsr_reg, const uint32_t *sp)
{
    UNUSED(icsr_reg); // Mark parameter as unused to suppress compiler warnings
    /* The 'icsr_reg' value may be used to check which exception vector is active.
     * Based on this, for example, logging can include either fewer details than
     * in this case or additional ones.
     */

    // Log the exception data using the RTE_MSGN macro
    RTE_MSGN(MSGN_FATAL_EXCEPTION, F_SYSTEM, sp, 4U * 20U);
    /* Legend:
     *      MSGN_FATAL_EXCEPTION - Format code name
     *      F_SYSTEM             - Message filter number
     *      sp                   - Address of the stack where the pushed registers are stored
     *      4 * 20 - Total size of the data to be logged (bytes)
     *          20 - Number of registers in the stack that should be logged
     *               See the implementation of 'Default_Handler' in 'startup_stm32___.s'
     *           4 - Size of each register (bytes)
     */


    /* Add custom code here to handle the exception - for example:
     *  - Set peripherals to an inactive state
     *  - Log additional info such as a global data structure, state machine value(s), etc.
     *  - Perform a software reset to restart the system
     */

    // Infinite loop to prevent the function from returning
    for (;;)
        ;
}
```

<br>

**Formatting definition for the exception handler in this demo - prints the logged exception data (cortex_M4-M7_fault.h).**
```
// MSGN_FATAL_EXCEPTION
#define MSGN_FATAL_EXCEPTION 480U
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
```
Notes:
1. The "#define MSGN_FATAL_EXCEPTION ID_number" is automatically inserted during the precompile phase.
2. The "%[64:32u]08X" is an example of how to set the address of the first bit and the size of a value to be printed. The value starts at bit 64 (third word), has a size of 32 bits and is an unsigned integer.

<br>

**Example of decoded core dump data - displaying register contents before exception state.**
```
N05529 1203,292 MSGN_FATAL_EXCEPTION: 
CPU registers
  R00:0x64171647, R01:0x200000AC, R02:0x07000000, R03:0x00000259
  R04:0x20000088, R05:0xFC000000, R06:0x2000008C, R07:0x200000A8
  R08:0x00EAC4E1, R09:0x080027B4, R10:0xFA481A48, R11:0x00000064
  R12:0x00000000,  SP:0x2001FF28,  LR:0x03000000,  PC:0x08000938
  xPSR:0x21000000, Flags: Q=0, V=0, C=1, Z=0, N=0
     Interrupted exception: #0 - Thread mode (vector table address: 0x00)
  ICSR: 0x00000803
     Active exception: #3 - HardFault (vector table address: 0x0C)
     Pending exception: #0 - Thread mode (vector table address: 0x00)     
  CFSR:0x00008200       
     Bus fault 
        A data bus error has occurred, and the PC value stacked for the exception return points to the instruction that caused the fault.    
        Valid fault address       
  Offending address:0x07000000
```
**Legend:**
* N05529 - sequential numbers of the decoded message
* 1203,292 - time in milliseconds (time at which this message was logged)
* MSGN_FATAL_EXCEPTION - format code name

#### Comment regarding the SP value in the core dump shown above
The stack pointer (SP) points to the location after the exception has been thrown and the exception handler has started. The actual SP value as it was before the exception was triggered depends on the floating-point unit (FPU) - if the device has one and if it was enabled by the firmware. The intelligent stacking mechanism is designed to optimize performance. It avoids the overhead of saving unused registers, which makes exception handling faster when the FPU is not needed. See the details below.

* **FPU is disabled:** If the FPU is not available, not enabled or not used by the program at the time of the exception, the system only saves the core registers. No extra space is allocated on the stack for the FPU registers. This is efficient, as it saves memory and reduces the time required to handle the exception.
* **FPU is enabled:** If the FPU is enabled, the processor recognizes this and allocates additional space on the stack for the FPU registers. The system pushes the FPU registers onto the stack along with the core registers, ensuring that the entire state of the program, including any floating-point operations, is preserved. 

