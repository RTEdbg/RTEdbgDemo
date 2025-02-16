/*
 * main.h
 *
 * Definitions of things which are not available in a project generated with the
 * MCUXpresso toolchain or are different from the code generated with STM32CubeIDE.
 *
 * Author: Branko Premzel
 */

#ifndef RTEDBG_INC_MAIN_H_
#define RTEDBG_INC_MAIN_H_

#include "board.h"

extern volatile uint32_t uwTick;	// Tick counter does not exist
void HAL_Delay(uint32_t);			//  and also function which deals with the 1 ms tick
#define HAL_DeInit()				// Replace with the name of function which de-initializes
									//  the peripherals

// Disable the following definition if the fault demo should not be started
#define INVALID_ADDRESS ((volatile uint32_t*)0x0F000000)	// Address used to provoke system fault

#endif /* RTEDBG_INC_MAIN_H_ */
