/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/***
 * @file    rtedbg_demo.c
 * @author  Branko Premzel
 *
 * @brief The RTEdbg library demonstration file contains examples that show how to
 * use logging functions.
 * Format definitions for this demo are in "Fmt/rte_demo.h".
 *
 * @note The demo does not produce correct results for floating point values if the
 * rte_any32_t is defined as uint32_t (RTE_USE_ANY_TYPE_UNION undefined).
 * Such definition is mandatory if your compiler does not support casting to union
 * or if a strict C standard compatibility is required.
 * Floating point values are truncated to integers and therefore not
 * printed correctly on a host computer using the RTEmsg utility.
 * Floating point values are printed correctly if they are logged with the
 * macro RTE_MSGN().
 */

#include <math.h>
#include "rtedbg.h"
#include "rte_demo.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288
#endif

#if defined RTE_USE_ANY_TYPE_UNION
static float val_float = 0.F;
#endif

// Union used to demonstrate how to pack values using bitfields
static uint32_t counter = 0U;
typedef union
{
    struct
    {
        unsigned int data3 : 3;
        unsigned int data5 : 5;
        signed int data24 : 24;
    } pack;
    uint32_t u32;
} val_struct1_t;


#ifndef __NO_RETURN
#define __NO_RETURN
#endif

__NO_RETURN void rtedbg_demo(void)
{
    uint32_t filter = RTE_ENABLE_ALL_FILTERS;  // Enable all 32 message filters

#ifdef STM32H743xx
    /* This part of code is for the STM32H7 family only (STM32H743, H750, H753, etc.)
     * It demonstrates how to log the reset cause and restart logging after the power-on
     * reset (parameter RTE_RESTART_LOGGING) or continue with data logging if e.g. a
     * watch-dog reset restarted CPU. Parameter RTE_CONTINUE_LOGGING enables continuation
     * of logging after reset unless the filter variable has been set to to zero to stop
     * data logging completely. Setting it to zero enables the data to be collected from
     * the embedded system after a software crash followed by a watch-dog reset.
     */

    // Example: log the cause of reset (power-on, watch-dog, software reset etc.)
    uint32_t reset_cause = RCC->RSR;    // RCC Reset status register
        // The reset cause is in the RCC->CSR for the STM32F4/L4/G4 families

    if (reset_cause & RCC_RSR_PORRSTF_Msk) // Power-on reset?
    {
        // Erase the data logging buffer and restart logging
        rte_init(RTE_ENABLE_ALL_FILTERS, RTE_RESTART_LOGGING);
    }
    else
    {
        // Keep logged data and restart just the timestamp counter
        rte_init(RTE_ENABLE_ALL_FILTERS, RTE_CONTINUE_LOGGING);
    }
    /* Note: The rte_init() function must be called before the interrupts that trigger the logging
     * of the long timestamp are initialized. Just to simplify the example, the rte_init() call
     * for this demo is included in this file.
     */

    RTE_MSG1(MSG1_CPU_RESET, F_DEMO_MSG1, reset_cause)   // Save the reset cause value to the log
    SET_BIT(RCC->RSR, RCC_RSR_RMVF);       // Reset all RSR flags

    // Log the frequency change - should be called after the firmware changes
    // the CPU clock frequency or if the clock supervisor circuit has for example
    // switched from the external crystal oscillator to the internal one.
    // The function which updates the SystemCoreClock variable must be called before
    // the call to RTE_MSG1() macro if necessary.
    RTE_MSG1(MSG1_CPU_FREQUENCY, F_SYSTEM, SystemCoreClock)

    // Log the timestamp frequency to the circular buffer
    // This function has to be called only if there was a frequency change
    rte_timestamp_frequency(SystemCoreClock);
#else  /* ifdef STM32H743xx */
    // Keep logged data and restart just the timestamp counter
    rte_init(RTE_ENABLE_ALL_FILTERS, RTE_CONTINUE_LOGGING);
#endif /* ifdef STM32H743xx */

    struct BattData
    {
        uint16_t voltage_x100;      // Battery voltage [V x 100]
        int16_t current_x100;       // Battery current [A x 100]
        uint16_t charged_ah_x10;    // Charged capacity [Ah x 10]
        uint16_t discharged_ah_x10; // Discharged capacity [Ah x 10]
        uint8_t temperature1_x2;    // Sensor #1, offset 100 (0 = -50ºC)
        uint8_t temperature2_x2;    // Sensor #2
        uint16_t soc_x10;           // SOC [% x 10]
    }
    battery =
    {
        1250U, -2345, 2002U, 3003U, 23U * 2U + 100U, 24U * 2U + 100U, 995U
    };

    RTE_MSGN(MSGN3_BATT_DATA, F_DEMO_MSG3, &battery, sizeof(battery))
    RTE_MSG1(MSG1_BIN_VALUE, F_DEMO_MSG2, 0x750FU)  // 0b0111010100001111

    // Test decoding of half float (16-bit float) values
    RTE_MSG1(MSG1_FLOAT16_A, F_SYSTEM, 0xFA481A48U) // 0,00306702, -51456
    RTE_MSG1(MSG1_FLOAT16_B, F_SYSTEM, 0x3ECFFFFFU) // -nan, 1,70215
    RTE_MSG1(MSG1_FLOAT16_C, F_SYSTEM, 0x6FAF0000U) // 0, 7868

    RTE_MSG0(MSG0_EVENT1, F_SYSTEM)

#if defined RTE_USE_ANY_TYPE_UNION
    val_float = 1.0F;
#endif

    for ( ;; )
    {
        rte_set_filter(filter);
        filter <<= 1U;     // Disable one message group in each cycle

        if (filter == 0U)
        {
            filter = RTE_ENABLE_ALL_FILTERS;
        }

        // Pack three values into one 32b word and send them in one short message
        // This example shows a possibility how to do it. May not work with all compilers.
        val_struct1_t test_struct;
        test_struct.pack.data3 = (counter & 7U);
        test_struct.pack.data5 = (counter / 10U) & 0x1FU;
        test_struct.pack.data24 = (counter * 100U) & 0x00FFFFFFU;
        RTE_MSG1(MSG1_STRUCT1, F_DEMO_MSG1, test_struct.u32)

#if defined RTE_USE_ANY_TYPE_UNION
        // Print the counter as integer (decimal and hex) and a float value (raw and scaled)
        RTE_MSG2(MSG2_FLOAT, F_DEMO_MSG1, counter, val_float)
        val_float *= 10.F;
#endif

        // Test decoding of half float (16-bit float) values
        RTE_MSG1(MSG1_FLOAT16_A, F_SYSTEM, 0xFA481A48U)
        RTE_MSG1(MSG1_FLOAT16_B, F_SYSTEM, 0x3ECFFFFFU)
        RTE_MSG1(MSG1_FLOAT16_C, F_SYSTEM, 0x6FAF0000U)

        // Additional data in the RTE_EXT_MSG0_4 is an example how to record
        // for example state of a sequencer in a single word message
        RTE_EXT_MSG0_4(EXT_MSG0_4_SEQUENCE, F_DEMO_MSG3, counter & 0xFU)

#if !defined __ARM_ARCH_6M__
/* The code may not work on Cortex M0 or Cortex M0+ because the compiler can put
 * the string at a non-aligned address, which would cause a system error.
 * In general, compilers for 32-bit CPU cores align string variables to word addresses.
 * This is true for string variables, but not for constant strings. If the string address
 * may not be aligned (e.g. the programmer specifies non-aligned compilation/linking
 * to use memory more efficiently), non-aligned addressing must be enabled for the CPU core.
 */
        RTE_STRING(MSGN_STRING, F_SYSTEM, "Quick brown fox in the garden.")
        RTE_STRINGN(MSGN_STRING, F_SYSTEM, "Quick brown fox in the garden.", 8U)
#endif

        // Print scaled integers
        RTE_MSG2(MSG2_UNSIGNEDSCALED, F_DEMO_MSG1, (counter % 100U) + 1200U, counter % 110U)

        // Demo for the extended data in message
        RTE_EXT_MSG1_3(EXT_MSG1_3_DEMO, F_DEMO_MSG3, counter, (counter >> 2U) & 7U)

#if defined RTE_USE_ANY_TYPE_UNION
        // Output data into a CSV file together with the number of sample and time stamp
        float rez = (float)counter / 100.F;
        RTE_MSG3(MSG3_UNION_DEMO, F_DEMO_MSG1, rez, 1.0F + rez, 3.14159265F)
#endif

        HAL_Delay(1U);
        RTE_MSG1(MSG1_TICK, F_DEMO_MSG1, (rte_any32_t)uwTick)

        double val1 = M_PI * 1e9;
        RTE_MSGN(MSGN2_DOUBLE, F_SYSTEM, &val1, sizeof(val1))

#if 1 // Disable this part if not supported by your compiler
        struct
        {
            float angle;
            float result;
        }
        data_struct = { 1.234567F, 99.987F };

#if defined RTE_COMPILE_TIME_PARAMETER_CHECK
        static_assert(sizeof(data_struct) <= (RTE_MAX_MSG_SIZE), "Structure size too large");
        // Example how to do compile time check if the structure is not larger
        // as it could be sent with one call to message logging function
#endif // RTE_COMPILE_TIME_PARAMETER_CHECK

        RTE_MSGN(MSGN2_DATA, F_DEMO_MSG2, &data_struct, sizeof(data_struct))
#endif // 1

        // Log 3 bytes to Main.log and to a binary file also
        RTE_MSGX(MSGX_SAVE_BIN_DATA, F_SYSTEM, &uwTick, 3U)

#if 0 /* Example - re-enable filter if it has been disabled completely.
       * Note: Should not be done if the message filter should be disabled by the debugger
       * or for the purpose of data transfer to the host using i.e. debug probe.
       */
        if (rte_get_filter() == 0U)
        {
            rte_set_filter(RTE_FORCE_ENABLE_FILTER);
            // Re-enable all filters if disabled completely
        }
#endif

#if defined INVALID_ADDRESS
        // Enable this piece of code for the exception logging test
        // Provoke a fatal error => start the demo of exception data logging
        if (counter > 600U)
        {
//            (void)*INVALID_ADDRESS;  // Read from invalid address
            // Note: A write to the invalid address can provoke an imprecise error due to write
            //       buffering (on CPU cores where implemented and enabled).
        }
#endif

        counter++;
    }
}


#if 0

/* This part of the code shows how to stop logging before sending the logged data to
 * the host - see the Send_data_to_host() function example.
 * The function rte_get_buffer_info() checks the data in the circular buffer and prepares
 * the pointers to the buffer. The variable buf.count > 0 indicates that data has been
 * logged in the circular buffer (count > 0). It is not necessary to send the entire buffer
 * to the host if only a small amount of data it has been logged.
 * The macro
 *    #define RTE_NON_STATIC_DATA_STRUCTURE
 * in the "rtedbg_config.h" must be uncommented to enable access to the g_rte_dbg data structure.
 *
 * If single shot logging mode was used for data logging, the rte_init() function must be called
 * to re-enable logging after the data has been transferred to the host or written to non-volatile
 * memory.
 */

#include "rtedbg_int.h"

/* Data structure for the rte_get_buffer_info() function.
 * The total number of 32-bit words that must be sent to the host or written to
 * nonvolatile memory = end – start.
 */
typedef struct
{
    uint32_t * start;    // Start and
    uint32_t * end;      //  end of the data logging structure
    uint32_t count;      // Number of data words logged
} rte_buf_data_t;


/***
 * @brief The function checks if the circular buffer has been rewritten at least once
 *        and returns the actual amount of data in the buffer if it has not been completely
 *        rewritten.
 *        It retrieves the circular buffer's start and end positions, along with the amount
 *        of data that has been written to it. Empty fields, indicated by a value of
 *        0xFFFFFFFF, needn't be transferred to the host computer used for data decoding
 *        and analysis, nor be written to the non-volatile memory.
 *
 * @note  Message logging should be disabled (filter set to zero) before this function is
 *        called to prevent further modification of the data in the buffer. In a multi-tasking
 *        system, make sure that all tasks can finish writing to the buffer.
 *
 * @param  rte_buff  A pointer to the structure where the output is written.
 */

void rte_get_buffer_info(rte_buf_data_t * rte_buff)
{
    const uint32_t total_size = sizeof(g_rtedbg.buffer) / sizeof(uint32_t);
    uint32_t count = total_size;

#if RTE_SINGLE_SHOT_ENABLED != 0
    if ((g_rtedbg.rte_cfg & RTE_SINGLE_SHOT_ENABLED) != 0)
    {
        if (count > g_rtedbg.buf_index)
        {
            count = g_rtedbg.buf_index;
        }
    }
    else
#endif
    {
        do
        {
            if (g_rtedbg.buffer[count - 1U] != RTE_ERASED_STATE)
            {
                break;
            }
        }
        while (--count != 0U);
    }

    rte_buff->count = count;
    rte_buff->start = (uint32_t *)&g_rtedbg;
    rte_buff->end = &g_rtedbg.buffer[count];
}


/***
 * @brief Example of a function that checks how many data is in the circular buffer
 * and sends the data to the host.
 */

void Send_data_to_host(void)
{
    rte_set_filter(0U);         // Disable message logging
    // Note: Any further logging attempts are disabled but all data may not be written.
    // Add a delay if writing should end, e.g. in another RTOS task.
    // Data logging function are re-entrant and the writing of data to the
    // circular buffer may not be finished when a task switch occurs.
    // Example: add a call to vTaskDelay() if this is a FreeRTOS task.

    rte_buf_data_t buff_data;
    rte_get_buffer_info(&buff_data);

    if (buff_data.count > 0U)
    {
        // Send data over e.g. serial port, USB, TCP/IP, etc.
        Send_logged_data_to_PC(&buff_data);
    }

    rte_restore_filter();       // Re-enable the message logging with the last filter value
    // Or call the rte_set_filter(RTE_FORCE_ENABLE_ALL_FILTERS) if all
    // message groups should be enabled.
}

#endif // 0

/*==== End of file ====*/
