## ARM Cortex-M4 / M7 Exception Handler Example

This demo shows how to perform a core dump on an ARM Cortex-M4/M7 processor with additional minimal code in an embedded system where the CPU core registers are logged as binary data. The logged data is decoded (printed) on the host. No printf() functionality and no printf strings are necessary in the embedded system firmware.

**Note:** This fault handler is suitable for RTOS-based and bare-metal projects. A **code size optimized version** of **[ARM Cortex-M4 / M7 Exception Handler](https://github.com/RTEdbg/RTEdbgDemo/blob/master/STM32L433/RTEdbg/Demo_code/Simple_Cortex_M4-M7_fault_handler.md)** is available. See also **[List of exception handlers](https://github.com/RTEdbg/RTEdbgDemo/blob/master/STM32H743/RTEdbg/Demo_code/Fault_handler.md)**.

This example shows the logging and formatting capabilities of the **[RTEdbg](https://github.com/RTEdbg/RTEdbg)** toolkit. The format definitions are a superset of the printf() string syntax. The example shows how to output data in a user-friendly format, even when the values in the circular buffer are not in the desired order and certain CPU core registers contain bit fields. 

If a fatal error occurs while the debug probe is not connected, it is important to keep the most important information such as the CPU core dump and important global variables. The logged data allows you to analyze not only the error itself, but also the embedded system data and events that preceded the error, which may provide information about the cause of the error.
The complete logged data with complete history (contents of the `g_rtedbg` structure) can be, for example, stored in internal or external flash memory for later retrieval on-site, or alternatively transmitted to host via an IoT connection.

The demo fault handler (core dump logging) is implemented in `./RTEdbg/Demo_code/fault_handler.c` and is invoked from the startup file `startup_stm32____.s`. The default infinite loop in `Default_Handler` in `startup_stm32____.s` is replaced with a call to the logging function `start_exception_handler()` defined in `fault_handler.c`.
This handler is compatible with all STM32 devices based on Cortex-M4 or Cortex-M7 cores. If you plan to use it with other processor families featuring Cortex-M4 or Cortex-M7, review the implementation and adapt it as needed.
Developers can also extend the handler to log additional diagnostic information, such as stack contents, key global variables, or RTOS state variables.

The handler logs the values of general and system CPU registers using the data logging function of the **RTEdbg** library. Data logging is stopped by setting the message filter to zero. An automatic restart with the watchdog does not overwrite the data in the circular buffer unless the programmer requests it with the `rte_init()` or `rte_set_filter()` parameters (force buffer clear or force new filter value) - see *Locking Data Logging After a Fatal Error* in the RTEdbg manual. This data can be analyzed later when an automatic restart has already been performed and, for example, the host computer is connected to the embedded module. The hardware drivers and communication functions typically require a restart to reconnect to the host after a fatal error.

Even a small data-logging buffer (e.g. 1 kB) can log information about fatal errors and what the code was doing before the error occurred. This allows for efficient 'post-mortem' analysis. Additional information can be logged, if defined by the programmer, ranging from RTOS internals to task or system stack details and important system variables. A complete data structure or buffer can be logged with a single call to `RTE_MSGN()`.

**Note:** For the IAR EWARM and Keil MDK startup code versions, the exception handler entry code is implemented in the startup file (in assembly language). It calls `main_exception_handler()`.

The bus fault is triggered by the following line in `rtedbg_demo.c` and `simple_demo.c` to demonstrate the handler.
```
  #define INVALID_ADDRESS ((volatile uint32_t*)0x07000000)    // Bad address used to throw an exception

  (void)*INVALID_ADDRESS;        // Read from invalid address
```

### Format definitions
Exception data is logged with the `RTE_MSGN()` macro. The format definition for the ARM Cortex-M7 exception message logging is in the file **[cortex_M7_fault.h](../Fmt/cortex_M7_fault.h)**. Exception vector names are in the file **[cortex_M7_exceptions.txt](../Fmt/cortex_M7_exceptions.txt)**. 

The format definition for the ARM Cortex-M4 is in the file **[cortex_M4_fault.h](../Fmt/cortex_M4_fault.h)** and exception vector names in the **[cortex_M4_exceptions.txt](../Fmt/cortex_M4_exceptions.txt)**.

### Example of data printed to the Main.log file after decoding the bus fault exception (ARM Cortex-M7)
```
N02804 603.797 MSGN_FATAL_EXCEPTION:   Register dump
  R00:0xFFFFFFF0, R01:0x00000012, R02:0x07000000, R03:0x0000012D
  R04:0x20000030, R05:0xFFFFFFE0, R06:0x00000064, R07:0x007594F5
  R08:0x00001FE9, R09:0xFA481A48, R10:0x0000000A, R11:0x20000000
  R12:0x00001335,  SP:0x2001FF28,  LR:0x00000675,  PC:0x00000684
  Status(xPSR = 0x21000000): ISR_No=0, Flags: Q=0, V=0, C=1, Z=0, N=0
  EXC_RETURN:0x000E9, BASEPRI:0x00000, CONTROL:0x00000
  CFSR:0x00008200:       
     Bus fault
        A data bus error has occurred, and the PC value stacked for the exception return
           points to the instruction that caused the fault.    
        Valid fault address       
  HFSR:0x40000000, ABFSR:0x00000000, Offending address:0x07000000
  ICSR:0x00000803, VECTACTIVE: 3-HardFault, VECTPENDING: 0-Thread mode
```
**Legend:**
* `N02804` - message number
* `603.797` - time [ms]
* `MSGN_FATAL_EXCEPTION` - format ID name