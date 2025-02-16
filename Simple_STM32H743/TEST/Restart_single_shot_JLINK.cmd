speed 12000
device Cortex-M7
W4 0x24000004 0             // Disable data logging (set message filter to 0)
W4 0x24000000 0             // Reset the circular buffer index
W4 0x24000004 0xFFFFFFFF    // Set the message filter (0xFFFFFFFF = enable all message groups)
exit
