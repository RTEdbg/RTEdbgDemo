@echo off
REM Transfer data from embedded system using J-Link Debug Probe GDB Server
"c:\RTEdbg\UTIL\RTEgdbData\RTEgdbData.exe" 2331 0x04000000 0 -p -decode=Decode.bat -filter=0xFFFFFFFF -priority -driver=JLinkGDBServerCL.exe -clear

