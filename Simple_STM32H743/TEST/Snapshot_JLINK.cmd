speed 12000
device Cortex-M7
savebin Filter.bin 0x24000004 0x4       // Save the current filter value
W4 0x24000004 0                         // Disable data logging by setting message filter to zero
savebin Temp.bin 0x24000000 0x400       // Load some data - delay to enable the remaining data to be written
savebin Data.bin 0x24000000 0x2028      // Save the g_rtedbg structure contents (logged data and header)
LoadFile Filter.bin 0x24000004 noreset  // Restore the message filter
exit
