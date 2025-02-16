/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/*******************************************************************************
 * @file    rtedbg_config.h
 * @author  Branko Premzel
 * @brief   Configuration file with compile-time options for data logging.
 *       Any compiler- or application-specific definitions related to message logging
 *       with the RTEdbg library should be in this file or in the files for which an
 *       include is defined here.
 *
 * @note This configuration file was made for testing STM32H743 Simple DEMO code.
 *       The config file template for new projects is in the 'Library\Inc' folder.
 *       The file must be adapted to the CPU core the compiler used in the project.
 *
 *       If you intend to use this configuration file as a starting point for your
 *       project, remove anything that is redundant to your project and make the
 *       necessary changes.
 *
 *       This file contains example code optimization option settings for the GCC,
 *       IAR EWARM, and ARM v5/v6 C compilers.
 ******************************************************************************/

#ifndef RTEDBG_CONFIG_H
#define RTEDBG_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   // Target hardware and project specific definitions
    
#define RTE_TIMER_DRIVER  "rtedbg_timer_cyccnt.h"
#define RTE_GET_TSTAMP_FREQUENCY()  SystemCoreClock // Current core frequency [Hz]
#define RTE_TIMESTAMP_SHIFT   1U
  /* Divide the time-counter value for the timestamp by 2^N (1 = division by 2).
   * Value of 1 is the minimum value that can be used.
   */

/* CPU core-specific functions for buffer space reservation. */
#define RTE_CPU_DRIVER  "rtedbg_cortex_m_mutex.h" // Buffer space reservation using mutex instructions


/*****************************************
 *   SETUP THE RTEdbg FUNCTIONALITY
 ****************************************/

#define RTE_ENABLED                       1
  /* 1 - data logging functionality enabled
   * 0 - data logging disabled (excluded from build)
   */

#define RTE_FMT_ID_BITS                 10
   /* Specifies the number of format ID bits used to log a message.
    * This value must be the same as the RTEmsg command line parameter -N=value.
    * The value should not be larger than necessary to have more bits for the timestamp.
    * Number of bits available for the timestamp = 32 - RTE_FMT_ID_BITS - 1
    */

#define RTE_BUFFER_SIZE                2048
  /* Number of 32-bit words in the circular data buffer.
   * If the value of RTE_BUFFER_SIZE is a power of two, the data logging of messages is
   * a bit faster and the logging functions are also smaller.
   */

#define RTE_MAX_SUBPACKETS              16
  /* The maximum number of data subpackets in a message determines the maximum message size.
   * Length of a subpacket is four 32-bit words plus one FMT word.
   * Maximum logged message length for __rte_msgn() in bytes = RTE_MAX_SUBPACKETS * 16.
   * This parameter must be equal to or greater than the largest data structure to be logged
   * with a call to RTE_MSGN() or RTE_MSGX() macros.
   * This parameter is a safeguard to prevent an erroneously long message from overwriting
   * the entire circular buffer (the minimum value of this parameter is 1). It also indirectly
   * limits the maximum logging time.
   */

#define RTE_MSG_FILTERING_ENABLED         1
  /* 1 - message filtering enabled (mandatory if the single shot logging is enabled)
   * 0 - message filtering disabled (all messages will be logged; single shot logging not possible)
   */

#define RTE_FILTER_OFF_ENABLED            1
  /* 1 - The firmware can completely disable logging with rte_set_filter(0).
   *     In this case, message logging can only be re-enabled by debugger or by calling
   *     rte_set_filter(RTE_FORCE_ENABLE_FILTER) or rte_init(RTE_FORCE_ENABLE_FILTER, ...)
   * 0 - The message filter cannot be completely disabled by the firmware.
   */

#define RTE_FIRMWARE_MAY_SET_FILTER       1
  /* 1 - The rte_set_filter(), rte_get_filter() and rte_restore_filter() functions
   *     are available to the programmer.
   * 0 - The g_rtedbg.filter variable can only be set through the debugger interface or
   *     by calling the rte_init() function.
   */

#define RTE_MINIMIZED_CODE_SIZE           0
  /* 0 - Fastest code execution, reduced stack requirement and larger code size if most
   *     of the data logging functions are used.
   * 1 - Reduced code size but slower execution and higher stack usage for the
   *     functions: __rte_msg0() ... __rte_msg4().
   * 2 - Typically, an even smaller code size than with RTE_MINIMIZED_CODE_SIZE = 1 at
   *     the cost of slower execution.
   */

#define RTE_DELAYED_TSTAMP_READ           1
  /* 1 - The timestamp counter is read just before its value is needed for data logging.
   *     The code size is typically slightly smaller and the stack usage also.
   *     This setting is generally recommended for simpler microcontrollers (like the
   *     Cortex M0+, M3 or M4 core), since in most cases the code will run faster, be
   *     smaller and use less stack.
   * 0 - The timestamp counter is read early during data logging function execution.
   *     This typically results in faster execution on microcontrollers with complex CPU
   *     cores such as the Cortex M7. This is especially true when using a timestamp
   *     counter with a slower connection to the CPU core or a slower clock for the timer
   *     peripheral bus.
   */

#define RTE_USE_LONG_TIMESTAMP            1
  /* 1 - Long timestamp used in the project - rte_long_timestamp() function enabled.
   * 0 - Long timestamp not used (only relative times between messages are logged).
   */

#define RTE_SINGLE_SHOT_ENABLED           0
  /* 1 - Both post-mortem and single shot logging available to the programmer.
   *     Which logging method will be used is defined by the rte_init() function parameter.
   *     Note: Add the following to the config file
   *        #define RTE_STOP_SINGLE_SHOT_AT_FIRST_TOO_LARGE_MSG
   *     if no messages should be stored in the circular buffer after the first too large
   *     message that does not fit in the buffer.
   * 0 - Only post-mortem logging available (faster execution and smaller code size).
   *     This is the preferred method when single-shot logging is not required for code
   *     testing/debugging.
   */

#define RTE_DISCARD_TOO_LONG_MESSAGES     1
  /* 1 - Discard messages that are longer than the maximum allowed.
   * 0 - Shorten messages that are too long to the maximum size.
   */

/**
 * Some CPU cores do not support unaligned memory access, or it may be possible to disable
 * unaligned memory access via firmware. For such cases, the function __rte_msgn()/RTE_MSGN()
 * can enable special handling of messages with an unaligned address by setting
 * RTE_HANDLE_UNALIGNED_MEMORY_ACCESS to 1. Alternatively, logging of such messages can be
 * disabled by setting RTE_DISCARD_MSGS_WITH_UNALIGNED_ADDRESS to 1. This prevents triggering
 * a system error when accessing an unaligned memory address.
 *
 * If logging a message at an unaligned address occurs while RTE_HANDLE_UNALIGNED_MEMORY_ACCESS
 * is enabled, logging will be slower compared to cases where the memory address is aligned.
 */

#define RTE_HANDLE_UNALIGNED_MEMORY_ACCESS     0
  /* 1 - CPU core does not allow unaligned memory access or unaligned access is disabled.
   * 0 - CPU core supports unaligned memory access, and special handling of this is not necessary.
   */

#define RTE_DISCARD_MSGS_WITH_UNALIGNED_ADDRESS     0
  /* 1 - Discard message if the address parameter provided to the RTE_MSGN macro is not aligned.
   * 0 - Do not discard a message if the address parameter is not aligned.
   */



/*********************************************************************************
 *              COMPILER-SPECIFIC DEFINITIONS
 *
 * Conditional compilation is used here, allowing the same configuration file to
 * be used for all demo projects and all tested compilers. Remove the unnecessary
 * definitions if you'll use this file for your project.
 ********************************************************************************/

/*-----------------------------------------------------------------------------
 * Define memory space for g_rtedbg data structures for logging data. Use an empty
 * macro if the location of the RTEdbg data structure is either not important or is
 * specified by e.g. linker parameters.
 *-----------------------------------------------------------------------------*/
#if (defined __GNUC__) || (defined __ARMCC_VERSION) || (defined __ICCARM__)
#define RTE_DBG_RAM  __attribute__((section("RTEDBG"))) __attribute__((used))
#else // Unknown compiler
#define RTE_DBG_RAM
   /* Insert a definition that is compatible with your compiler. If the __attribute__
    * keyword is not available, the _Pragma keyword can be used instead.
    */
#endif

/*---------------------------------------------------------------------------
 * Code optimization parameters for the functions in the rtedbg.c file.
 *
 * The RTEdbg library functions in the rtedbg.c file are short and the compiler
 * options do not have much effect on the generated code.
 * Optimize your data-logging code for speed or size as needed. Small code is
 * usually fast, but actual speed, code size, and stack usage should be checked
 * with the compiler used for your project. The most aggressive settings will
 * not always produce the fastest and/or smallest code for all functions.
 *
 * Compilation options can be set in the IDE or Makefile for the redbg.c file.
 * In this case, the macro definitions listed below are not required.
 * Additional macros can be used to set up the compilation of the RTEdbg library
 * separately from the rest of the project.
 * Compilation options can be set in this configuration file or on the level of
 * the entire rtedbg.c file with the RTE_OPTIMIZE_CODE macro or for individual
 * function types with the following macros:
 *   RTE_OPTIM_SIZE  - Optimization of infrequently used functions (e.g. rte_init)
 *   RTE_OPTIM_SPEED - Optimization of frequently used data logging functions
 *   RTE_OPTIM_LARGE - Optimization of large functions rte_msgn() and rte_msgx()
 * If an individual macro is not defined, then the RTE_OPTIMIZE_CODE macro setting
 * or the v setting of the rtedbg.c file in the project is used for the corresponding
 * function types if RTE_OPTIMIZE_CODE is not defined either.
 *
 * The RTE_USE_ANY_TYPE_UNION macro definitions should also be defined for compilers
 * that allow any 32-bit value to be converted to a union and where strict ISO C
 * compatibility is not required. See the RTEdbg library manual for a detailed
 * description.
 *
 * RTE_COMPILE_TIME_PARAMETER_CHECK - If the macro is not defined, compile-time
 * static_assert() checks are skipped. This macro can be used to disable static
 * checking if the compiler does not support it, or if it is disabled by compilation
 * options - e.g. for C99 compatibility.
 *---------------------------------------------------------------------------*/

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if defined __ICCARM__          // IAR C Compiler
#define RTE_COMPILE_TIME_PARAMETER_CHECK
// Example of IAR EWARM project optimization options for the functions in rtedbg.c  
#define RTE_OPTIM_SIZE   _Pragma("optimize=size")
#define RTE_OPTIM_SPEED  _Pragma("optimize=speed")
#define RTE_OPTIM_LARGE  _Pragma("optimize=balanced")
   /* Note: If you use the #pragma optimize directive to specify a level of optimization that
    * is higher than the optimization level you specify using a compiler option, the pragma
    * directive is ignored. See the IAR C/C++ Compiler Reference Guide => #pragma optimize
    */
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#elif defined __ARMCC_VERSION   // ARM C Compiler v5 or v6
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010000) /* ARM Compiler V6 */
#define RTE_COMPILE_TIME_PARAMETER_CHECK
#define RTE_USE_ANY_TYPE_UNION
    // The Arm compiler 6.x does not support changing optimization options within a C-source file.
    // Instead, they must be set on a per-file basis using command-line options.
#else /* ARM Compiler 5 */
    // ARM Compiler 5 does not support compile-time checking with _Static_assert().
//#define RTE_OPTIMIZE_CODE _Pragma("Ospace")
#define RTE_OPTIM_SIZE   _Pragma("Ospace")
#define RTE_OPTIM_SPEED  _Pragma("Otime")
#define RTE_OPTIM_LARGE  _Pragma("Ospace")
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#elif defined __GNUC__          // GNU C Compiler
#define RTE_COMPILE_TIME_PARAMETER_CHECK
#define RTE_USE_ANY_TYPE_UNION
/* Use either RTE_OPTIMIZE_CODE or RTE_OPTIM_SIZE/_SPEED/_LARGE macros or nothing at all. */
//#define RTE_OPTIMIZE_CODE _Pragma("GCC optimize(\"Os\")")
#define RTE_OPTIM_SIZE   __attribute__((optimize("Os")))
#define RTE_OPTIM_SPEED  __attribute__((optimize("O2")))
#define RTE_OPTIM_LARGE  __attribute__((optimize("Os")))
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#else                           // Unknown compiler
/* Insert definitions compatible with your compiler. If the __attribute__ keyword is not
 * available, the _Pragma keyword can be used.
 */
#define RTE_COMPILE_TIME_PARAMETER_CHECK
#endif /* if defined __ICCARM__ */
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef __cplusplus
}
#endif

#endif /* RTEDBG_CONFIG_H */

/*==== End of file ====*/
