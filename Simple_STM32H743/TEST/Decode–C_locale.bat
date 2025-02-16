@echo off
REM Decodes the binary file. The files are created in the output folder.
REM Read the documentation in the Readme file.

if not exist output md output

c:\RTEdbg\RTEmsg\RTEmsg.exe output ..\RTEdbg\Fmt -N=10 Data.bin -stat=all -time=m -timestamps -locale=C
REM Show data only if no error (0) or no fatal errors found (3)
IF %ERRORLEVEL% EQU 0 goto ShowData
IF %ERRORLEVEL% EQU 3 goto ShowData

REM Pause used to show error message in the console window
pause
goto exit

REM Display decoded information
:ShowData
start "" output\sin_cos.csv
start "" output\Battery_data.csv
start "" "%ProgramFiles%\Notepad++\notepad++.exe" output\*.log output\*.txt

:exit
