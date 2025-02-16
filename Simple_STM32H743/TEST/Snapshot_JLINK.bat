@echo off
REM Take a snapshot of a running embedded system, run the RTEmsg application to decode the data.
REM and launch Notepad++ and CSV Viewer to view the decoded messages.

REM Read the documentation in the Readme file.

if not exist output md output

REM The command CD (change working directory) is only necessary if the batch file
REM  is started from another program - e.g. Notepad++
REM cd "c:\RTEdbg\Demo\Simple_STM32H743\TEST"

if exist Data.bin del Data.bin

REM Data transfer using the J-LINK debug probe
REM The address and size of the RTEdbg data structure are in the Snapshot_JLINK.cmd
"c:\Program Files\SEGGER\JLink\JLink.exe" -If SWD -ExitOnError -CommandFile Snapshot_JLINK.cmd
IF %ERRORLEVEL% NEQ 0 goto Error

REM Decode the binary data file. The files will be created in the output folder.
c:\RTEdbg\RTEmsg\RTEmsg.exe output ..\RTEdbg\Fmt -N=10 Data.bin -stat=all -time=m -timestamps
REM Show data only if no error (0) or no fatal errors found (3)
IF %ERRORLEVEL% EQU 0 goto ShowData
IF %ERRORLEVEL% EQU 3 goto ShowData

:Error
REM Pause to display error message in console window
pause
goto end

REM Display the decoded information
:ShowData
start "" "output\Battery_data.csv"
start "" "output\sin_cos.csv"
start "" "%ProgramFiles%\Notepad++\notepad++.exe" output\*.log output\*.txt

:end
REM Remove the temporary files
del Filter.bin
del Temp.bin
