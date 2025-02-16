/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/***
 * @file   fault_handler.h
 * @author Branko Premzel
 * @brief  Example of a ARM Cortex M4/M7 exception handler.
 *
 * @note Modify the handler accordingly to the target CPU and project requirements.
 */

#ifndef INC_FAULT_HANDLER_H_
#define INC_FAULT_HANDLER_H_


void __NO_RETURN shutdown_and_restart(void);


#include "main.h"
#include "rtedbg.h"
#if __CORTEX_M == 7U
#include "cortex_M7_fault.h"
#else
#include "cortex_M4_fault.h"
#endif

// Data structure for exception data collection in the fault handler
typedef struct
{
   /* The space reserved for registers must be the first variable in this structure
    * because this is necessary due to the way data is stored in the exception handler,
    * which is partly written in assembly language. */
   uint32_t   r[13];              // Values of registers R0 .. R12
   uint32_t * sp;                 // Stack pointer and
   uint32_t * lr;                 // Link register values at a point when the exception handler was called
   uint32_t * pc;                 // Program counter
   uint32_t   xpsr;               // Program status register (including the vector number of interrupted code)
   uint8_t    exc_return;         // Lowest 8 bits of the exception return value (high bits are 1)
                                  // Information about which SP was used, thread/handler mode and FPU use
   uint8_t    basepri;            // CPU BASEPRI register value
   uint8_t    control;            // CPU CONTROL register value
   uint8_t    total_exceptions;   // Total number of handled fatal exceptions
   uint32_t   cfsr;               // Value of the SCB_CFSR register showing possible error cause(s)
   uint32_t   hfsr;               // Value of the Hard Fault Status Register
#if __CORTEX_M == 7U
   uint32_t   abfsr;              // ABFSR is not available on devices with Cortex M4 cores
#endif
   uint32_t   offending_address;  // Address accessed when a MPU or Bus error occurred (zero if the value is onknown)
   uint32_t   icsr;               // Value of SCB->ICSR
} exception_t;

extern volatile exception_t g_exception;


/**
 @brief   Get Link Register
 @details Returns the current value of the Link Register (LR).
 @return  LR register value
*/
#if defined ( __ICCARM__ )
// __get_LR() is EWARM intrinsic function
#elif defined ( __CC_ARM )
register uint32_t __reglr  __ASM("lr");
__STATIC_INLINE uint32_t __get_LR(void)
{
    return(__reglr);
}
#elif defined ( __GNUC__ )
__STATIC_FORCEINLINE uint32_t __get_LR (void)
{
    register uint32_t result;
    __ASM volatile ("MOV %0, LR\n" : "=r" (result) );
    return(result);
}
#else  /* if defined( __ICCARM__ ) */
#error "Unknown compiler"
#endif /* if defined( __ICCARM__ ) */


void __NO_RETURN __USED main_exception_handler(const uint32_t msp_used,
                                               uint32_t * const fault_stack_address);

#endif /* INC_FAULT_HANDLER_H_ */

/*==== End of file ====*/

