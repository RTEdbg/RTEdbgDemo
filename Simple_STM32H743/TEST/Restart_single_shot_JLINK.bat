@echo off
REM Restart the single-shot data recording. 

REM Read the documentation in the Readme file.

REM See the command sequence description in the 'Restart_single_shot_JLINK.cmd' file.
"c:\Program Files\SEGGER\JLink\JLink.exe" -If SWD -ExitOnError -CommandFile Restart_single_shot_JLINK.cmd
IF %ERRORLEVEL% EQU 0 goto exit

REM Pause to display error message in console window
pause

:exit
