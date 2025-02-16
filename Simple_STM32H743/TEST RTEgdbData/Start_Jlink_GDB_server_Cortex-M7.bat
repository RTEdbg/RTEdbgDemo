@echo off
REM Start the J-Link GDB server and connect to the ARM Cortex CPU without stopping it.
REM Do not start this file if J-Link Server is started from the IDE.
:start
"C:\Program Files\SEGGER\JLink\JLinkGDBServerCL.exe" -device Cortex-M7 -if SWD -nohalt -noir -noreset -Speed 15000 -silent
if errorlevel 0 goto start
pause
