@echo off
REM Transfer data from embedded system using ST-Link Debug Probe GDB Server
REM RTEgdbData is started in persistent mode (-p) to allow multiple data
REM transfers from the embedded system to the host. Since the parameter
REM  -decode=batch_file is defined, the decoding and display of the data
REM starts automatically after the data transfer.

REM The -start=continue.cmd argument executes the continue.cmd file that
REM resumes the firmware operation since the ST-LINK GDB server stops it
REM after the RTEgdbData utility connects to the GDB server.
REM This is not required for the J-Link GDB Server.

"c:\RTEdbg\UTIL\RTEgdbData\RTEgdbData.exe" 61234 0x10000000 0 -p -decode=Decode.bat -filter=0xFFFFFFFF -start=continue.cmd -priority -driver=stlinkserver.exe -driver="ST-LINK_gdbserver.exe"
IF %ERRORLEVEL% EQU 0 goto End
pause
:End