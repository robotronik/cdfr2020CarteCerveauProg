target remote | openocd -f interface/stlink-v2-1.cfg -f target/stm32f3x.cfg -f gdb/gdb-pipe.cfg
monitor reset halt
