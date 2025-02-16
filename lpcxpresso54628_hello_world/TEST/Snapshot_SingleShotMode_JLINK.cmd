speed 12000
device Cortex-M7
savebin Temp.bin 0x04000000 0x400       // Load some data - delay to enable the remaining data to be written (e.g. low level task interrupted during data logging)
savebin Data.bin 0x04000000 0x7D28      // Save the g_rtedbg structure contents (logged data and header)
W4 0x04000000 0                         // Reset the circular buffer index
W4 0x04000004 0xFFFFFFFF                // Set the message filter and enable logging (0xFFFFFFFF = enable all message groups)
exit
