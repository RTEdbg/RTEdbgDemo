# Demo Project for LPCXpresso54628 Board with MCUXpresso

The basis for this demo project was chosen from the NXP example code (hello_world) because it is one of the simplest demo code projects available for the LPCXpresso54628 (ARM Cortex-M4) and thus suitable for demonstrating the integration of data logging. 

The calls to the demo code necessary for testing the data logging and some additional functions have been added to the `hello_world.c` file. The *RTEdbg* subfolder containing the RTEdbg library and the demo files has been added to the project `source` folder. Compilation and linking options have been modified where necessary - see below.

See the **RTEdbg manual** - section ***Simple Demo Project*** for a general description of demo projects.

The `g_rtedbg` structure definitions have been added to the link definition file 
`lpcxpresso54628_hello_world_Debug.ld` 
and the managed linker script was disabled. The automatically generated linker script files have been moved out of the debug folder because they might be deleted by the project clean command. They have been moved up one level for the demo project and the script path has been changed accordingly.

The `g_rtedbg` structure with the circular buffer has been placed into the 32 kB SRAMX memory with the following definition added to the linker definition file `lpcxpresso54628_hello_world_Debug.ld`.

```
    /* RTEdbg data logging memory section */
    . = ALIGN(4);
    .RTE (NOLOAD):
    {
        *(RTEDBG)
        *(RTEDBG*)
    } >SRAMX
```
This places the *g_rtedbg* data logging structure at address 0x04000000. See also ***MCUXpresso IDE User Guide*** section ***Placing code and data into different Memory Regions***.

The following pre-build command 
```
c:\\RTEdbg\\RTEmsg\\RTEmsg.exe . ..\\RTEdbg\\Fmt -N=10 -c
```
has been added to the `Project properties` => `C/C++ Build` => `Settings` => `Build Steps` => `Pre-build command`.

**Caution:**  The main build may be executed regardless of the success/failure of the pre-build step - see [link](https://community.nxp.com/t5/MCUXpresso-IDE/Do-not-ignore-pre-build-error/m-p/1573154).

The file `main.h` has been added to the `source/RTEdbg/Inc` folder. It contains definitions of things which are not available in a project generated with the MCUXpresso toolchain or which are different from the code generated with STM32CubeIDE to maintain RTEdbg sample code compatibility between demo projects.

The debug configuration in the demo is for a Segger J-Link LITE running on the Link2 debug adapter integrated on the board. It was programmed with [LPCScrypt](https://www.nxp.com/design/microcontrollers-developer-resources/lpcscrypt-v2-1-2:LPCSCRYPT?tid=vanLPCSCRYPT).

The following C compiler settings have been changed:

1. `Tool settings => MCU C Compiler => Dialect` has been changed to ISO C11. If the settings are for a C version older than ISO C C11 (or GNU C 11), the parameters of the RTEdbg data logging macro cannot be checked at compile time. In this case, the RTE_CHECK_PARAMETERS definition in the configuration file must be disabled (value set to 0).
1. Two additional folders have been added to the `Tool settings => MCU C Compiler => Includes`:
```
   ${workspace\_loc:/${ProjName}/source/RTEdbg/Inc}
   ${workspace\_loc:/${ProjName}/source/RTEdbg/Fmt}
```   

**Important:** The SDK for the `lpcxpresso54628` must be installed before testing this demo project. The toolchain will not detect and report the problem if the SDK is missing. Various errors will be reported when trying to set the debug configuration or start a debug session instead of the real cause.

Note: The CPU clock is 220 MHz for this demo.