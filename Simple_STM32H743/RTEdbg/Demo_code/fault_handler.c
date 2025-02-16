/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/***
 * @file   fault_handler.c
 * @author Branko Premzel
 * @brief  An example of fault and exception handling to demonstrate exception data logging
 *         for STM32 microcontrollers with Cortex M4 and M7 CPU core.
 *         Modify the handler accordingly to your CPU specifics and project requirements.
 *
 * @note See the additional documentation about exceptions and exception handling:
 *    https://interrupt.memfault.com/blog/cortex-m-hardfault-debug
 *    https://www.segger.com/downloads/application-notes/AN00016
 *    https://www.keil.com/appnotes/files/apnt209.pdf
 */

#include "main.h"
#include "fault_handler.h"


// Global variables -----------------------------------------------------------------
// Information about the last error or exception - for data logging and debugger
volatile exception_t g_exception;


/***
 * @brief The first part of the exception handler must be in the assembly code.
 *
 * This function is called from startup_stm32____.s for all exceptions and interrupts
 * that do not have their own handlers.
 */

#if !defined(__ICCARM__) && !defined(__CC_ARM)
#ifndef _lint
void __attribute__((naked)) start_exception_handler(void)
{
    __asm volatile
    (
        " cpsid i                 \n"   /* Disable interrupts */
        " ldr r12,=g_exception    \n"   /* Registers are first variables of this structure */
        " stm r12, {r0-r11}       \n"   /* Save values of all registers for debugging purpose */
        " movs r0, #0             \n"   /* Exception during unprivileged mode execution */
        " tst lr, #4              \n"   /* PSP or MSP was used before the exception was triggered? */
        " itte eq                 \n"
        " moveq r0, #1            \n"   /* Exception during privileged mode execution */
        " mrseq r1, msp           \n"
        " mrsne r1, psp           \n"
        " b main_exception_handler\n"
    );
}
#endif // _lint
#else  /* if !defined( __ICCARM__ ) && !defined( __CC_ARM ) */
/* The start_exception_handler() must be implemented in an assembly language source file if the
 * C compiler does not support "naked" implementation - without the prolog code as shown above.
 */
#endif /* if !defined( __ICCARM__ ) && !defined( __CC_ARM ) */


void __NO_RETURN __USED main_exception_handler(const uint32_t msp_used,
                                               uint32_t * const fault_stack_address)
{
    // Check if exception occurred during data logging of an exception
    if (++g_exception.total_exceptions > 1U)
    {
        // An additional exception occurred before the exception handler execution finished.
        // The structures in RAM or CPU internals are probably corrupted (or code in Flash).
        shutdown_and_restart();
    }

    if (msp_used != 0U)
    {
        __disable_irq();                  // The system code will not be restarted
    }

    g_exception.control = (uint8_t)__get_CONTROL();
    g_exception.basepri = (uint8_t)__get_BASEPRI();
    g_exception.exc_return = (uint8_t)__get_LR();

    g_exception.hfsr = SCB->HFSR;         // Hard Fault Status Register
    SCB->HFSR = g_exception.hfsr;
    g_exception.icsr = SCB->ICSR;         // Interrupt control and state register

    // The AFSR (Auxiliary Fault Status Register) is not saved to the data structure.
    // There are no details about the implementation in the STM32 Cortex M7 programming manual.

#if __CORTEX_M == 7U
    g_exception.abfsr = SCB->ABFSR;       // Auxiliary Bus Fault Status Register
    SCB->ABFSR = g_exception.abfsr;
#elif __CORTEX_M != 4U
#error "Code not defined for other Cortex M core types"
#endif

    // Save the information about code execution before exception, including SP, PC, and LR.
    // The information in the stack depends on the task that was interrupted.
    // If the FPU was active then the FPU registers are also in stack.
    // The FPU registers are not saved in this implementation.
    g_exception.pc = (uint32_t *)fault_stack_address[6U];
    g_exception.lr = (uint32_t *)fault_stack_address[5U];
    g_exception.sp = fault_stack_address;
    g_exception.xpsr = fault_stack_address[7U];
    g_exception.r[12] = fault_stack_address[4U];

    g_exception.cfsr = SCB->CFSR;         // Main fault status Register
    uint32_t addr = 0U;

    if (g_exception.cfsr & SCB_CFSR_MMARVALID_Msk)
    {
        addr = SCB->MMFAR;
    }
    else if (g_exception.cfsr & SCB_CFSR_BFARVALID_Msk)
    {
        addr = SCB->BFAR;
    }

    g_exception.offending_address = addr;
    SCB->CFSR = g_exception.cfsr;         // Write the value back to erase this system register

    /* ----------- Log the fault information for post-mortem debugging ---------------
     *
     * The programmer must decide whether or not to log exception-related data. This is
     * accomplished by adding a call to the rte_set_filter() function with the
     * RTE_FORCE_ENABLE_ALL_FILTERS parameter to the exception handler. This ensures that
     * messages enabled with the F_SYSTEM filter are logged.
     * The RTE_FORCE_ENABLE_ALL_FILTERS parameter, on the other hand, can cause the same
     * error to be logged repeatedly in the case of repeated exceptions, and therefore
     * information about what led to the condition that caused the errors may be lost.
     */
    rte_set_filter(RTE_FORCE_ENABLE_ALL_FILTERS); // Enable the data logging and log the timestamp - add the
                                                  //  information about the time when the execution was stopped
#if __CORTEX_M == 7U
    RTE_MSGN(MSGN23_FATAL_EXCEPTION, F_SYSTEM, &g_exception, sizeof(g_exception))
#else
    RTE_MSGN(MSGN22_FATAL_EXCEPTION, F_SYSTEM, &g_exception, sizeof(g_exception))
#endif

    /* Disable message logging by setting the filter to 0 if you do not want additional
     * information to be logged after a system restart, for example, or if you want the
     * information to remain in a circular buffer until a programmer or technician reads
     * it from the embedded system.
     * The error information logged in the circular buffer is preserved after e.g. a watchdog
     * reset (system restart) if the RTE_FILTER_OFF_ENABLED != 0 and the firmware does not use
     * the RTE_FORCE_ENABLE_ALL_FILTERS to force the filter to be enabled again.
     */
    rte_set_filter(0U);

    shutdown_and_restart();
}
/*-------------------------------------------------------------------------------*/

/*==== End of file ====*/
