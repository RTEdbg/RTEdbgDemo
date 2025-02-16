speed 12000
device Cortex-M4
savebin Filter.bin 0x10000004 0x4       // Save the current filter value
W4 0x10000004 0                         // Disable data logging by setting message filter to zero
savebin Temp.bin 0x10000000 0x400       // Load some data - delay to enable the remaining data to be written
savebin Data.bin 0x10000000 0x2028      // Save the g_rtedbg structure contents (logged data and header)
LoadFile Filter.bin 0x10000004 noreset  // Restore the message filter
exit
