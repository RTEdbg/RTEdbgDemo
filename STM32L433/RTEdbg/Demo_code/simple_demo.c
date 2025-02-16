/*
 * Copyright (c) Branko Premzel.
 *
 * SPDX-License-Identifier: MIT
 */

/***
 * @file    simple_demo.c
 * @author  Branko Premzel
 *
 * @brief The demo shows how to use some of the data logging functions.
 * The uwTick global variable is used in the function as a changing value
 * to enable real-time like data to be captured by the firmware, decoded on the
 * host computer, and displayed using the various tools available to display
 * text and CSV files.
 *
 * Format definitions are in the file "Fmt/simple_demo_fmt.h".
 */

#include <math.h>
#include <stdint.h>
#include "rtedbg.h"
#include "simple_demo_fmt.h"


// If the 'INVALID_ADDRESS' is defined then the exception is triggered after few 100 ms.
// Disable it to enable permanent code execution.
//#define INVALID_ADDRESS    ((volatile uint32_t *)0x07000000)
    // Invalid address for the STM32H7xx microcontroller family


/***
 * @brief Example of data structure used to show to log battery data using
 *        8 and 16-bit values packed into a structure with a total length
 *        of three 32-bit words.
 */

struct _battery_data
{
    uint16_t voltage_x100;      // Battery voltage [V x 100]
    int16_t current_x100;       // Battery current [A x 100]
    uint16_t charged_ah_x10;    // Charged capacity [Ah x 10]
    uint16_t discharged_ah_x10; // Discharged capacity [Ah x 10]
    uint8_t temperature1_x2;    // Sensor #1 [ºC x 2], offset 100 (0 = -50ºC, 255 = 77.5ºC)
    uint8_t temperature2_x2;    // Sensor #2 [ºC x 2], offset 100
    uint16_t soc_x10;           // SOC [% x 10]
}
battery;


/****
 * @brief The function is not a real battery system simulator. It is used only to
 *        produce changing values that are logged using a call to themacro RTE_MSGN()
 *        and decoded/displayed on host computer.
 */

static void simulator_demo(void)
{
    uint32_t temp = (uwTick >> 3U) & 0x1FFU;

    if (temp > 50U)
    {
        battery.current_x100 += 9;

        if (battery.current_x100 > 3000)
        {
            battery.current_x100 = 3000;
        }
    }

    if (temp > 200U)
    {
        battery.current_x100 -= 17;

        if (battery.current_x100 < -1000)
        {
            battery.current_x100 = -1000;
        }
    }

    battery.charged_ah_x10 += (uint16_t)(battery.current_x100 / 100);

    if (battery.charged_ah_x10 > 1000U)
    {
        battery.charged_ah_x10 = 0U;
    }

    battery.discharged_ah_x10 = 333U;
    battery.temperature1_x2 = 150U + ((uwTick / 100U) & 7U);
    battery.temperature2_x2 = 150U - ((uwTick / 160U) & 7U);
    battery.voltage_x100 = (uint16_t)(1100u + uwTick % 222U);

    battery.soc_x10++;
    if (battery.soc_x10 > 100U)
    {
        battery.soc_x10 = 0U;
    }

    RTE_MSGN(MSGN3_BATT_DEMO, F_BATTERY_DATA, &battery, sizeof(battery))
}


/***
 * @brief  Generation of some periodic data. This data is logged and
 *         decoded / displayed on the host computer.
 */

static void sin_cos_demo(void)
{
    // Record start of the function.
    // This allows the calculation of the time required to run the sin_cos_demo() function.
    RTE_MSG0(MSG0_START_SIN_COS, F_SINCOS_DEMO)

    uint32_t tick = uwTick;
    float signal1 = 3.F * sinf((float)tick / 66.F) + .52F * cosf((float)tick / 444.F);
    float signal2 = signal1 - 2.F * sinf((float)tick / 112.F)
                    + .08F * cosf((float)tick / 332.55F) - 2.F + (float)(tick & 15U) / 29.F;

#ifdef RTE_USE_ANY_TYPE_UNION
    /* For compilers that support type conversion with rte_any32_t union, it is
     * possible to float type parameter data in a simpler way - see below. */
    RTE_MSG2(MSG2_SINCOS_DEMO, F_SINCOS_DEMO, signal1, signal2)
#else
    /* For compilers that do not support type conversion with the rte_any32_t union,
     * the float type of the data parameter must be defined as below. */
    RTE_MSG2(MSG2_SINCOS_DEMO, F_SINCOS_DEMO,
             float_par(signal1), float_par(signal2))
#endif
}


/***
 * @brief  A short presentation of some simple use cases of the RTEdbg library.
 */

static void basic_examples(void)
{
    // Example of an event recording
    RTE_MSG0(MSG0_SIMPLE_TEST_START, F_SYSTEM)

    static uint32_t sequence_number = 0U;
    if (sequence_number != 0U)
    {
        return;
    }

    // Log a 32-bit value and a three bit sequence number
    uint32_t data1 = 1926u;
    RTE_EXT_MSG1_3(EXT_MSG1_3_DEMO_EXT_MSG, F_SYSTEM, data1, sequence_number)
    sequence_number = (sequence_number + 1U) & 7U;

    // Log unsigned integer, float and double values with RTE_MSG3() macro
    data1 = 0x12345678U;
    float data2 = 9.19986F;
    double data3 = -88.44e9;
#ifdef RTE_USE_ANY_TYPE_UNION
    RTE_MSG3(MSG3_COMBINED_DEMO, F_SINCOS_DEMO, data1, data2, (float)data3)
#else
    RTE_MSG3(MSG3_COMBINED_DEMO, F_SINCOS_DEMO, data1, float_par(data2), double_par(data3))
#endif

    /* The following example shows how to pack multiple values into one message.
     * The double value is stored as a 32-bit float, and values from value3 to
     * value5 are packed into a 32-bit word.
     * A total of four 32-bit words are stored in the circular buffer instead of seven
     * if every value would be stored separately and a 64-bit value for value2 (double).
     */
    float  value1 = 3.14F;
    double value2 = 1e9 * sqrt(3.);
    int32_t value3 = -1000;        // 16-bit signed value
    uint8_t value4 = 27U;          // 6-bit unsigned value
    int16_t value5 = -500;         // 10-bit signed value
    RTE_MSG3(MSG3_COMBINED, F_SINCOS_DEMO,
        float_par(value1),
        double_par(value2),
        ((uint32_t)value3 & 0xFFFF) |               // Size = 16 bits
        ((value4 & 0x3FU) << 16U) |                 // Size = 6 bits
        (((uint32_t)value5 & 0x3FFU) << (16U + 6U)) // Size = 10 bits
        )
    /* Note: Logical AND on signed values does not comply with MISRA rule 10.1. Operands shall not
     *       be of an inappropriate essential type => bitwise operations as the logical AND should
     *       only be performed on unsigned type. However, the code shown above has nothing to do
     *       with functional safety. Its purpose is just to pack the variables for data logging.
     */
}


/***
 * @brief  A demonstration of how to log data using the RTEdbg library functions.
 */

void simple_demo(void)
{
/* Use one of the four options below to initialize the data logging and select
 * one the desired logging modes:
 *    RTE_RESTART_LOGGING       - Erase the data circular buffer and enable data logging
 *    RTE_CONTINUE_LOGGING      - Continue data logging without erasing the circular buffer
 *    RTE_SINGLE_SHOT_LOGGING   - Enable single shot logging and reset circular buffer index
 *    RTE_SINGLE_SHOT_AND_ERASE_BUFFER - Enable single shot logging and completely erase the buffer
 * See the RTEdbg manual for a detailed description.
 */
#if RTE_SINGLE_SHOT_ENABLED != 0
    rte_init(RTE_FORCE_ENABLE_ALL_FILTERS, RTE_SINGLE_SHOT_LOGGING);
#else
    rte_init(RTE_FORCE_ENABLE_ALL_FILTERS, RTE_RESTART_LOGGING);
#endif
    /* Note: The rte_init() function must be called before the interrupts that trigger the logging
     * of the long timestamp are initialized. Just to simplify the example, the rte_init() call
     * for this demo is included in this file.
     */

    // Generate some data as if collected from a real-time control system
    for (;;)
    {
        basic_examples();

        uint32_t time = uwTick;
        RTE_MSG1(MSG1_UWTICK, F_SYSTEM, time)

        sin_cos_demo();

        if ((time % 3U) == 0U)
        {
            simulator_demo();
        }

        // Wait for one ms to elapse
        while (time == uwTick)
        {
        }

        /***
         * To demonstrate logging data from the exception handler, add the following
         * directive to this file or to a header file - e.g. main.h.
         * Use an address that does not exist in the processor's address space => an
         * address that throws an exception when accessed.
         *
         * #define INVALID_ADDRESS ((volatile uint32_t*)0x07000000)
         */
#if defined INVALID_ADDRESS
        // The fatal error will be triggered after 600 ms (starts the exception handler demo)
        if (uwTick > 600U)
        {
            RTE_MSG0(MSG0_TRIGGER_SYS_ERROR, F_SYSTEM); // Example of an UTF-8 format definition
            (void)*INVALID_ADDRESS;                     // Read from invalid address (provoke an error)
        }
#endif
    }
}

/*==== End of file ====*/
