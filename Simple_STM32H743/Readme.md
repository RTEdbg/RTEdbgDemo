# Simple demo for the STM32H743 on the NUCLEO-H743ZI development board

The demo project demonstrates how to integrate data logging functions into an application. It is a simplified version of the demo code found in the STM32H743 folder. This folder contains code designed and tested for the STMicroelectronics NUCLEO-H743ZI/ZI2 development boards. No I/O has been used (all pins except debug are initialized as analog I/O), so the code will run on any board with a processor from the same family. Therefore, it will at least run on boards with STM32H742/743/753/750 CPUs. The internal oscillator is used to allow operation on boards without crystal or external oscillator.

The basis of this project was created with the STM32CubeMX code generator - see the `STM32H743.CubeMX` folder. Use a tool like WinMerge to compare this project with the CubeMX version. This will allow you to see how the data logging features have been integrated into the project. With this information and other details shown below, the demo can be easily ported to boards with other ARM Cortex-M4 or M7 CPUs. The project was developed with the STM32CubeIDE and tested with the Keil MDK and IAR EWARM to verify the portability of the code, format definitions and the library code optimization settings.

See the **RTEdbg manual** - section ***Simple Demo Project*** for a detailed description of this project and the **[STM32H743 Demo](../STM32H743/Readme.md)**.

Note: The CPU clock is 64 MHz for this demo.