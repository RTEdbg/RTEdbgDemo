speed 12000
device Cortex-M7
savebin Filter_copy.bin 0x20000010 0x4  // Save the the filter copy value
W4 0x20000004 0                         // Disable data logging by setting message filter to zero
savebin Temp.bin 0x20000000 0x400       // Load some data - delay to enable the remaining data to be written (e.g. low level task interrupted during data logging)
savebin Data.bin 0x20000000 0x1028      // Save the g_rtedbg structure contents (logged data and header)
W4 0x20000000 0                         // Reset the circular buffer index
LoadFile Filter_copy.bin 0x20000004 noreset  // Restore the message filter
exit
