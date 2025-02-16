speed 12000
device Cortex-M4
savebin Filter.bin 0x04000004 0x4       // Save the current filter value
W4 0x04000004 0                         // Disable data logging by setting message filter to zero
savebin Temp.bin 0x04000000 0x400       // Load some data - delay to enable the remaining data to be written
savebin Data.bin 0x04000000 0x7D28      // Save the g_rtedbg structure contents (8000 * 4 + 40 = 32040 = 0x7D28)
LoadFile Filter.bin 0x04000004 noreset  // Restore the message filter
exit
