@echo off
REM Transfer data from embedded system using J-Link Debug Probe GDB Server
"c:\RTEdbg\UTIL\RTEgdbData\RTEgdbData.exe" 2331 0x04000000 0

IF %ERRORLEVEL% EQU 0 goto ShowData
REM Pause to display error message in console window
pause
goto end

REM Decode and display the logged messages
:ShowData
Decode.bat

:end
