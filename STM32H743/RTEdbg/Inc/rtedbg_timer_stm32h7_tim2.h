/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/**********************************************************************************
 * @file    rtedbg_timer_stm32h7_tim2.h
 * @author  Branko Premzel
 * @brief   Time measurement for the data logging functions.
 *          TIM2 32-bit timer driver for the STM32H7 family.
 * @version RTEdbg library v1.01.00
 *
 * @note This is a minimalist version of the driver created without using the STM32 HAL
 *       or low-level HAL functions.
 **********************************************************************************/

#ifndef RTEDBG_TIMER_STM32H7_TIM2_H
#define RTEDBG_TIMER_STM32H7_TIM2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtedbg.h"

#define RTE_TIMESTAMP_COUNTER_BITS  32U  // Number of timer counter bits available for the timestamp

#if !defined RTE_USE_INLINE_FUNCTIONS
#if RTE_USE_LONG_TIMESTAMP != 0
struct _tstamp64
{
    uint32_t l;    // Bottom part of the 64-bit timestamp
    uint32_t h;    // Top part of the 64-bit timestamp
} t_stamp;
#endif // RTE_USE_LONG_TIMESTAMP != 0


/***
 * @brief Initialize the peripheral for the timestamp counter.
 */

__STATIC_FORCEINLINE void rte_init_timestamp_counter(void)
{
    // Enable the TIM2 clock
    SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN);
    (void)RCC->APB1LENR;    // Delay after enabling an RCC peripheral clock

    // Reset TIM2 registers to their default values.
    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM2RST);
    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM2RST);

    // Configure only the registers that require non-default values.
    TIM2->PSC = (RTE_TIMESTAMP_PRESCALER) - 1;
    TIM2->CR1 = TIM_CR1_CEN;        // Enable the timer counter
#if (RTE_TIMESTAMP_PRESCALER) != 1U
    TIM2->EGR = TIM_EGR_UG;         // Reload the prescaler with the new value
#endif

#if RTE_USE_LONG_TIMESTAMP != 0
    t_stamp.l = t_stamp.h = 0;      // Reset the long timestamp
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
    return TIM2->CNT;
}


#if (RTE_USE_LONG_TIMESTAMP != 0) && (!defined RTE_USE_INLINE_FUNCTIONS)

/*********************************************************************************
 * @brief  Writes a message with a long timestamp to the buffer.
 *         The low bits of the timestamp are included in the message words with the
 *         format ID. Only the higher 32 bits are transmitted in the message's
 *         data part.
 *
 * @note   This function is not reentrant. Typically, calls should be made from a
 *         single section of the program that is periodically executed - e.g.,
 *         from a timer interrupt routine.
 *********************************************************************************/

RTE_OPTIM_SIZE void rte_long_timestamp(void)
{
    uint32_t timestamp =
        (uint32_t)(rte_get_timestamp() << (32U - (RTE_TIMESTAMP_COUNTER_BITS)));

    if (t_stamp.l > timestamp)    // Has the counter rolled over?
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

#endif /* RTEDBG_TIMER_STM32H7_TIM2_H */

/*==== End of file ====*/

