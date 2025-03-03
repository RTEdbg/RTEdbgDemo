/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/**********************************************************************************
 * @file    rtedbg_timer_systick.h
 * @author  Branko Premzel
 * @brief   Time measurement for the data logging functions using the SYSTICK timer.
 *          ARM Cortex SYSTICK is a 24-bit down counter. The driver inverts the value
 *          to provide an increasing count.
 *
 * @note    If an MPU-protected version of the RTOS is used, no task (even a
 *          privileged one) has access to system peripherals such as the ARM
 *          Cortex-M CPU cycle counter (CYCCNT) or the system clock timer (SYSTICK),
 *          even if the memory area containing the cycle counter or SYSTICK is
 *          enabled for that task. Attempts to access these registers from unprivileged
 *          code result in a HardFault exception.
 *
 * @version RTEdbg library v1.01.00
 **********************************************************************************/

#ifndef RTEDBG_TIMER_SYSTICK_H
#define RTEDBG_TIMER_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtedbg.h"

#define RTE_TIMESTAMP_COUNTER_BITS  24U // Number of timer counter bits available for the timestamp


#if !defined RTE_USE_INLINE_FUNCTIONS
#if RTE_USE_LONG_TIMESTAMP != 0
struct _tstamp64
{
    uint32_t l;    // Lower 32 bits of the 64-bit timestamp
    uint32_t h;    // Upper 32 bits of the 64-bit timestamp
} t_stamp;
#endif // RTE_USE_LONG_TIMESTAMP != 0


/***
 * @brief Initialize the peripheral for the timestamp counter and reset its counter.
 */

__STATIC_FORCEINLINE void rte_init_timestamp_counter(void)
{
    SysTick->LOAD  = 0x00FFFFFFUL;                  /* Set the reload register */
//    SysTick->VAL   = 0UL;                           /* Load the SysTick Counter Value */
// The counter is reset to zero after reset according to the description in
// "https://developer.arm.com/documentation/ddi0413/d/system-control/system-control-register-descriptions/systick-current-value-register"
// Reset the counter with the firmware if it is more convenient for testing your application.
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |   /* Use the CPU core clock and */
                     SysTick_CTRL_ENABLE_Msk;       /* Enable the SysTick Timer */

#if RTE_USE_LONG_TIMESTAMP != 0
    t_stamp.l = t_stamp.h = 0;                      // Reset the long timestamp
#endif
}
#endif  // !defined RTE_USE_INLINE_FUNCTIONS


/***
 * @brief Get the current value of the timestamp counter.
 *
 * @return Current value of the timer counter.
 */

__STATIC_FORCEINLINE uint32_t rte_get_timestamp(void)
{
    return ~SysTick->VAL;   // Invert the value since the SYSTICK is a down counter
}


#if (RTE_USE_LONG_TIMESTAMP != 0) && (!defined RTE_USE_INLINE_FUNCTIONS)

/*********************************************************************************
 * @brief  Writes a message with a long timestamp to the buffer.
 *         The low bits of the timestamp are included in the message words with the
 *         format ID. Only the higher 32 bits are transmitted in the message's
 *         data part.
 *
 * @note    This function is not reentrant. Typically, calls should be made from a
 *          single section of the program that is periodically executed - e.g.,
 *          from a timer interrupt routine.
 *********************************************************************************/

RTE_OPTIM_SIZE void rte_long_timestamp(void)
{
    uint32_t timestamp =
        (uint32_t)(rte_get_timestamp() << (32U - (RTE_TIMESTAMP_COUNTER_BITS)));

    if (t_stamp.l > timestamp)    // Counter rolled over?
    {
        t_stamp.h++;
    }

    t_stamp.l = timestamp;
    uint64_t timestamp_64 = (uint64_t)timestamp | ((uint64_t)t_stamp.h << 32U);
    uint32_t long_t_stamp = (uint32_t)(timestamp_64 >>
                                ((32U - ((uint32_t)(RTE_FMT_ID_BITS))) - 1U + (RTE_TIMESTAMP_SHIFT) +
                                 (32U - (RTE_TIMESTAMP_COUNTER_BITS))));
    RTE_MSG1(MSG1_LONG_TIMESTAMP, F_SYSTEM, long_t_stamp);
}

#endif // (RTE_USE_LONG_TIMESTAMP != 0) && (!defined RTE_USE_INLINE_FUNCTIONS)

#ifdef __cplusplus
}
#endif

#endif /* RTEDBG_TIMER_SYSTICK_H */

/*==== End of file ====*/
