# Demo Project for the NUCLEO-L053 Development Board

The STM32L05x family (ARM Cortex-M0+) does not have 32-bit timers. Therefore the SYSTICK timer was used for the timestamp counter in this demo. This is a 24-bit counter and full timestamp resolution is possible and there are no limitations regarding the value of RTE\_FMT\_ID\_BITS. In such case, one of the 16-bit timers (e.g., timer TIM2) must be used for the HAL tick functionality instead of the default SYSTICK. The code is linked for execution in flash memory. The `g_rtedbg` data logging structure is located at address 0x20000000.

See the [Readme.md](../STM32H743/Readme.md) in the STM32H743 folder for additional description.

The NUCLEO-L053 demo also includes an example of how to implement a timestamp driver using a 16-bit timer counter – see the file `RTEdbg/_timer/_stm32L0/_tim2.h`. This driver has two limitations:

- The timestamp resolution is only 16 bits.
- The format ID size defined by RTE\_FMT\_ID\_BITS must be 15 or 16.

Note: The CPU clock is 16 MHz for this demo.