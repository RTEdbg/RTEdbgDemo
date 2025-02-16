@echo off
REM Restart the single-shot data recording. 

REM Read the documentation in the Readme file.

REM The command sequence is as follows:
REM   Set the message filter to zero first (disable data logging),
REM   Reset the circular buffer index, 
REM   Set the filter to 0xFFFFFFFF (enable all message groups - or use your custom value)
"c:\ST\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe" -c port=SWD mode=HOTPLUG shared -q -w32 0x10000004 0 -w32 0x10000000 0 -w32 0x10000004 0xFFFFFFFF
IF %ERRORLEVEL% NEQ 0 goto Error

REM Pause to display error message in console window
pause

:exit
