# Demo Project for the NUCLEO-L433 Development Board

The CPU core type is the Cortex-M4. The project is basically the same as the demo for the NUCLEO-H743ZI. See the **[STM32H743 Readme.md](../STM32H743/Readme.md)** in the STM32H743 folder for additional description.

The `g_rtedbg` data logging structure is located at address 0x10000000. For timestamp functionality, either the CYCCNT counter or the TIM2 timer can be used without code modification (enable one or the other in the `rtdbg_config.h`). An example for the SYSTICK timer is also included. However, to use it as a timestamp timer, the project code would have to be modified, since the SYSTICK is used for HAL tick functionality.

Note: The CPU clock is 16 MHz for this demo.
