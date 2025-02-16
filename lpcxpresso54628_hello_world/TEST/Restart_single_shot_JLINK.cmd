speed 12000
device Cortex-M4
W4 0x04000004 0             // Disable data logging (set message filter to 0)
W4 0x04000000 0             // Reset the circular buffer index
W4 0x04000004 0xFFFFFFFF    // Set the message filter (0xFFFFFFFF = enable all message groups)
exit
