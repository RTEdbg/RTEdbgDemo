## Initialize the data logging structure and timestamp timer for the STM32L053 DEMO.
## Omitting this function reduces the program memory consumption on resource-constrained
## systems, e.g. by about 160 bytes in the DEMO_STM32L053 folder. 

## Initialize the SYSTICK timer. It is used as the timestamp timer.
ME000E014,4:FFFFFF00
ME000E010,4:05000000

## If RTE_USE_LONG_TIMESTAMP is 1, set the long timestamp value to zero.
## Otherwise, delete the following two commands.
M20001464,4:00000000
M20001460,4:00000000

## The SYSTICK counter is reset to zero after a reset.
## Use the "ME000E018,4:00000000" to reset the counter here if necessary.

## Initialize the g_rtedbg data structure.
## Set RTEdbg config word and timestamp frequency as in your project.
#init 0x86101016 16000000

## ST-LINK GDB server stops code execution after RTEgetData connects to it.
## Start the code execution.
vCont;c