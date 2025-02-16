@echo off
REM Transfer data from embedded system using ST-Link Debug Probe GDB Server
"c:\RTEdbg\UTIL\RTEgdbData\RTEgdbData.exe" 61234 0x10000000 0 -filter=0xFFFFFFFF

IF %ERRORLEVEL% EQU 0 goto ShowData
REM Pause to display error message in console window
pause
goto end

REM Decode and display the logged messages
:ShowData
Decode.bat

:end
