speed 12000
device Cortex-M0
savebin Filter.bin 0x20000004 0x4       // Save the current filter value
W4 0x20000004 0                         // Disable data logging by setting message filter to zero
savebin Temp.bin 0x20000000 0x800       // Load some data - delay to enable the remaining data to be written
savebin Data.bin 0x20000000 0x1028      // Save the g_rtedbg structure contents (logged data and header)
LoadFile Filter.bin 0x20000004 noreset  // Restore the message filter
exit
