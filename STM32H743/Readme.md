# Demo for the STM32H743 on the NUCLEO-H743ZI development board

The demo project shows how to integrate the data logging functions into a project. This folder contains code designed and tested for the STMicroelectronics NUCLEO-H743ZI/ZI2 development boards. No I/O has been used (all pins except debug are initialized as analog I/O), so the code will run on any board with a processor from the same family. Therefore, it will at least run on boards with STM32H742/743/753/750 CPUs. The internal oscillator is used to allow operation on boards without crystal or external oscillator.

The basis of this project was created with the STM32CubeMX code generator - see the `STM32H743.CubeMX` folder. Use a tool like WinMerge to compare this project with the CubeMX version. This will allow you to see how the data logging features have been integrated into the project. With this information and other details shown below, the demo can be easily ported to boards with other ARM Cortex-M4 or M7 CPUs. The project was developed with the STM32CubeIDE and tested with the Keil MDK and IAR EWARM to verify the portability of the code, format definitions and the library code optimization settings. Follow the additional instructions in the **RTEdbg Manual**, section ***Simple Demo Project***.

## Changes made to the original code and project settings

The `RTEdbg` folder is added to the main project folder. It contains the RTEdbg library files. The format definition files are in the `Fmt` folder.

The `Demo_code` folder contains the following files:
* `Inc/fault_handler.h` and `fault_handler.c` - Example of a common exception handler with exception handler data logging.
* `simple_demo.c` - Demo code for the RTEdbg library showing the basic functionality.
* `rtedbg_demo.c` - Demo Code for the RTEdbg Library showing how to use the data logging library. This is a demonstration of various data logging functions and library test code. At the beginning of the `rtedbg_demo()` function is an example of logging the cause of a reset. This code part is specific to the STM32H7xx family and must be disabled if the demo is to be tested with other hardware.
* `rtedbg_test.c` - Code to test the data-logging functions after porting to new compilers or CPUs.

The following changes have been made to the main.c file:
1. A call to `rtedbg_test()`, `rtedbg_demo()` and `simple_demo()` has been added. Activate/deactivate the demo functions with   `#if 1 / #if 0`   to test them individually.
2. The code for this demo is linked into RAM. Therefore, the vector table offset register must be set at the beginning of the `main()` function.
3. In the function `Error_Handler()` is a demonstration of how to implement logging of calls to HAL library error handlers.

In the file `Core/Inc/stm32h7xx_hal_conf.h` some of the default enabled headers have been disabled for this demo to speed up compilation. These files are not required for this demo project.

**Note:** The STM32CubeMX includes many files that are not required for code generation - from drivers (`Drivers/STM32H7xx_HAL_Driver/` folder) to include files to files for other CPU cores and other compilers (`Drivers/CMSIS/Include folder`). Most of the unnecessary files have been removed to simplify the project, speed up compilation and reduce the size of the ZIP file containing the RTEdbg distribution.

In the file `startup_stm32h743zitx.s` the `b Infinite_Loop` has been replaced by `b start_exception_handler`. 	
This starts the shared exception handler each time an exception or interrupt is thrown for which the programmer has not provided an exception/interrupt handler. The exception handler with data logging is located in `RTEdbg/Demo_code/fault_handler.c`.
The example above is for the GCC ARM toolkit. Also included in the demo are startup file modification examples for IAR EAWARM and Keil MDK. Linking and include settings have also been done.

### Linking

Compare the original linker configuration file `STM32H743ZITX_RAM.ld` with the linker file in the `STM32H743` folder. The file has been modified to use the fast internal RAM (ITCM and DTCM) to test how fast the data logging functions can execute under optimal conditions. Only a section was added to specify in which part of the RAM the data logging structure should be placed:
```
  . = ALIGN(4);  
  .RTE (NOLOAD) :       /* RTEdbg data logging memory section */
  {
    *(RTEDBG)
    *(RTEDBG*)
  } >RAM_D1
```
This is an example of how we can place the `g_rtedbg` data structure at a specific address (to the RAM_D1).

### GCC compiler settings

1. The default optimization level for the project is -O0. For the demo, the level is set to -Og. <br>
`Project properties => C/C++ Build => Settings => Tool settings => MCU GCC Compiler => Optimization => Optimization level = Optimize for Debug (-Og)`
2. Linker script is changed to `${workspace_loc:/${ProjName}/STM32H743ZITX_RAM.ld}` <br>
`Project properties =>  C/C++ Build => Settings => Tool settings => MCU GCC Linker => General`
3. Convert to binary option for build output was disabled. <br>
`Project properties =>  C/C++ Build => Settings => MCP Post build outputs = Convert to binary = Disabled`

The following folders have been added to the include path list. <br>
`Project properties => C/C++ Build => Settings => MCU GCC Compiler => Include paths:`
```
   ../RTEdbg/Inc
   ../RTEdbg/Fmt
```
### Pre-build settings
The command 
```
   c:\\RTEdbg\\RTEmsg\\RTEmsg.exe . ..\\RTEdbg\\Fmt -N=10 -c
```
has been added to the <br>
    `Project properties => C/C++ Build => Settings => Build Steps => Pre-build command` <br>
to enable the automatic generation of #define's for the filter numbers and format IDs.
See the **RTEdbg manual** - section ***Verifying Format Definitions with RTEmsg*** for a description of how to integrate pre-build validation and format ID generation into other toolchains.

Note: The CPU clock is 64 MHz for this demo.