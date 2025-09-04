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

#if !defined __ARMCC_VERSION
#define __NAKED __attribute__((naked))
#else
#define __NAKED
#endif

void __NO_RETURN __NAKED log_exception(uint32_t icsr_reg, const uint32_t *sp)
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

/*==== End of file ====*/
