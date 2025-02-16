/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("hello world.\r\n");

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // The following code was added for the test of RTEdbg library functions with MCUXpresso toolchain
        SysTick_Config(SystemCoreClock / 1000U);	 // SysTick interrupt set at 1ms

    // Enable the following demo code - one by one and test it

#if 0
    // Binary data logging macros and function test code.
    // It is intended to test data logging code after porting to a new CPU family or compiler.
    void rtedbg_test(void);
    rtedbg_test();
#endif

#if 0
    // RTEdbg demo code - part 1
    void rtedbg_demo(void);
    rtedbg_demo();
#endif

#if 1
    // RTEdbg demo code - part 2
    extern void simple_demo(void);
    simple_demo();
#endif
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}


/********** HELPING CODE FOR THE DEMO ****************/
volatile uint32_t uwTick = 0;      	/* Variable to store millisecond ticks */
#include "rtedbg.h"

void SysTick_Handler(void)  {       /* SysTick interrupt Handler. */
    ++uwTick;                       /* See startup file startup_LPC17xx.s for SysTick vector */

    // Log the long timestamp every eight tick periods (8 ms)
    if ((uwTick & 7u) == 0)
    {
        rte_long_timestamp();
    }
}

void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = uwTick;

  while ((uwTick - tickstart) < Delay)
	  ;
}

void __NO_RETURN shutdown_and_restart(void)
{
    for (;;)
        ;
}
/*****************************************************/
