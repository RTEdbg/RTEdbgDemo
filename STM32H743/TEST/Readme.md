## **Set of demo batch files for use with the RTEdbg toolkit**

The TEST folder contains batch files for data transfer from the embedded system to the host using ST-LINK or J-LINK debug probes. Data decoding and display is also started from the same batch files. These files are just examples of how it is possible to automate the whole process.

**Note:** The new **RTEgdbData command line utility** is available. It allows data to be transferred in a uniform way via a GDB server. This makes it possible to transfer data to the host even for debug probes that do not have a command line utility for transferring data. See the readme in the *c:\RTEdbg\UTIL\RTEgdbData* folder or in the **[RTEgdbData Github repository](https://github.com/RTEdbg/RTEgdbData)**. Examples of batch files are in the TEST subfolder.


Before using batch files in your project, do the following
* Adjust the clock speed and other debug probe parameters accordingly to the probe type and your requirements.
* Change the address and read block size according to the g_rtedbg data structure if the address or RTE_BUFFER_SIZE parameter is changed.

See also the description in the **'Transferring Collected Data to a Host'** section in the **RTEdbg manual**.

See also the [[STM32CubeProgrammer - command line interface]](https://www.st.com/resource/en/user_manual/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf) and [[J-LINK commander]](https://wiki.segger.com/J-Link_Commander) manuals.

**Note:** To use the IDE's built-in debugger or other tools with log data sampling, enable shared mode on the ST-LINK - see [Shared Mode](#j-link-and-st-link-shared-mode). This is not necessary for the Segger J-LINK debug probe.

### **Snapshot_STLINK.bat**
Transfer and decode binary data from the embedded system using the ST-LINK debug probe.

Use the file to transfer data from the embedded system to the host computer. It can also be used to transfer data logged using post-mortem or single-shot data logging. The batch file does the following:
- Read the current message filter value and write it to the Filter.bin temporary file.
- Stop data logging by setting the message filter to zero.
- Transfer dummy data, causing a delay (during this time, the embedded system should finish logging, if it was interrupted by a low-priority process, for example).
- Transfer data from the embedded system to the Data.bin file using the STM32 Programmer command line interface.
- Set the filter to the previous value to enable data logging again.
- Decode the binary data with the RTEmsg application (contents is written to the 'output' folder).
- Launch the applications to view the decoded data (Notepad++, CSV viewer).
- Delete temporary files.

Transferring data while the firmware is running does not affect the operation of the code. Only data logging is paused, not program execution. Stopping logging by setting the filter to zero speeds up data logging because no data is written to circular memory. However, since the logging functions are very fast, the difference is very small.

**Note:** The command CD (change directory) is only necessary in cases where the batch file is run from a program such as Notepad++ - see [Running External Commands with Notepad++](https://npp-user-manual.org/docs/run-menu/).

### **Snapshot_JLINK.bat**
Transfer and decode binary data from the embedded system using the Segger J-LINK debug probe. See the `Snapshot_STLINK.bat` description above.

**Note:** If you are using the J-Link debug probe to transfer data from an embedded system and the CPU cycle counter is used as timestamp timer, please note that the J-Link driver by default clears all debug enable bits of the Cortex-M core when the debug session is closed, and this also disables the DWT unit with the cycle counter. This causes the cycle counter (`DWT->CYCCNT`) to stop after data transfer with the JLink.exe command line application. This can be avoided by transferring data with the RTEgdbData application or by starting the J-Link GDB server and keeping it running while testing the embedded system. You can also use another timer instead of the CPU cycle counter for the timestamp timer to avoid this.
<br>See also [J-Link Cortex-M application uses cycle counter](https://kb.segger.com/J-Link_Cortex-M_application_uses_cycle_counter).

### **Restart_single_shot_STLINK.bat**
Use this batch file to restart single-shot data logging with the ST-LINK debug probe after the data has been transferred from the embedded system to the host.
<br>The batch file restarts single shot data logging. The new filter value must be specified in the batch file. 
The default filter value is 0xFFFFFFFF (enables all message groups). Replace it with a custom value if only a subset of message groups should be enabled. This value determines which message groups are enabled after the restart.

**Note:** This batch file can also be used to restart post-mortem data logging, e.g. if it has been stopped by the firmware (firmware has reset the filter to zero after an error or event).

### **Restart_single_shot_JLINK.bat**
Use this batch file to manually restart single-shot data logging with the J-LINK debug probe after data has been transferred from the embedded system to the host. See also `Restart_single_shot_STLINK.bat`.

### **Snapshot_SingleShotMode_STLINK.bat**
The file is intended for data transfer from the embedded system that records data in single-shot mode. It is useful when logging is started by the host and the firmware stops logging when the circular buffer is full.
<br>The batch file does the following: 
- The message filter is set to zero to pause logging (if not already stopped by the firmware after the circular buffer was full).
- The contents of the `g_rtedbg.filter_copy` variable are written to the `Filter_copy.bin` file.
- The contents of the `g_rtedbg` data structure (logged data) are written to the `Data.bin` file.
- The circular buffer index is reset to zero (`g_rtedbg.buf_index`).
- The contents of the `g_rtedbg.filter_copy` variable is written to the message filter variable `g_rtedbg.filter` (message logging is enabled again).
- Decode the binary data in the `Data.bin` file and write the output to the files in the 'output' folder.
- Launch the applications to view the decoded data (Notepad++, CSV viewer).

### **Snapshot_SingleShotMode_JLINK.bat**
Same functionality for the J-LINK debug probe as described in `Snapshot_SingleShotMode_STLINK.bat`.

### **Decode.bat**
The batch file starts decoding the Data.bin binary data file. This batch file is useful if the decoding needs to be repeated because an error or defect has been detected in a format definition file. There is no need to recompile the firmware, download it to the embedded system, and repeat the test. This is especially important when the problem is difficult to reproduce.
The file can also be used when data is transferred to the PC using a media such as an SD card.
<br>**Note:** The format definition enumeration values should not change as a result of the format file modification.
Otherwise, the data will not be decoded correctly.

### **J-LINK and ST-LINK shared mode**
Shared mode must be enabled for the ST-LINK if log data sampling is to be used in parallel with the IDE's built-in debugger or some other tools. This is not necessary for the Segger J-LINK debug probe.

* **STM32CubeIDE**  
Enable the `Shared ST-LINK` in the `Debug Configuration => Debug Tab`.

* **IAR EWARM**  
See the description of the ST-LINK in the document "EWARM_DebuggingGuide.ENU.pdf" => Enable multicore debugging/shared mode.

* **Keil MDK**  
Enable "Shareable ST-LINK" in the ST-LINK Debugger settings.

* **MCUXpresso**  
The batch files are implemented only for the J-LINK debug probe (including J-LINK LITE on MCU-Link Pro or MCU-Link on-board).
