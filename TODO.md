Todo
----
Reference hw design

Put protocol version into registers
 - change version with breaking changes of registers / positions
 
Blank display fields if communications lost

Add TIM5 to BlackPill map (support encoder #4)
 - note PA0 is user key, may need to override in devicetree

GPIO keypad matrix
 - investigate tristate outputs, wasn't working with devicetree / pin_configure
   (safer than pulling idle rows low, though reference keypad has diodes on all keys)
 - add debouncing to input

Display
 - better formatting

Done
----
Display update performance
 - poor spi performance under zephyr
 - modbus rx callback does not run while spi transmission ongoing, can priorties be changed?
 - workaround with very small CONFIG_LVGL_VDB_SIZE
 - try configuring for DMA transfers.. resolved

Modbus register syncronisation between tasks
 - add a mutex or similar

Display
 - flip speed and feed override positions

Override encoders
 - tune filtering

IO expander in constraint, add support for Nucleo-64 board using GPIO keypad matrix
