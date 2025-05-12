@echo off
REM Transfer data from embedded system using ST-Link Debug Probe GDB Server

REM The -start=init.cmd command file initializes the data structure for data
REM logging (g_rtedbg) and the timer for timestamps.

REM RTEgetData is started in persistent mode (-p) to allow multiple data
REM transfers from the embedded system to the host. Since the parameter
REM  -decode=batch_file is defined, the decoding and display of the data
REM starts automatically after the data transfer.

"c:\RTEdbg\UTIL\RTEgetData\RTEgetData.exe" 61234 0x20000000 0x1028 -filter=0xFFFFFFFF -p -start=init.cmd -clear -decode=Decode.bat -filter_names=..\..\Debug\Filter_names.txt
