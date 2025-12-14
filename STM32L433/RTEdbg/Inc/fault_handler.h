/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/***
 * @file   fault_handler.h
 * @author Branko Premzel
 * @brief  Common definitions and functions for fault handlers.
 */

#ifndef INC_FAULT_HANDLER_H_
#define INC_FAULT_HANDLER_H_

#include "main.h"
#include "rtedbg.h"
#include "cortex_M4-M7_fault.h"


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


#endif /* INC_FAULT_HANDLER_H_ */

/*==== End of file ====*/

