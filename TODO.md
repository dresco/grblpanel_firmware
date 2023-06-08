# Todo

Add dfu-util support for custom board

Settings
 - add shell support for changing baud rates
 - add settings support for persisting baud rates
 - seperate shell (usb) and console/logging (uart) outputs
   - custom board - console on  usart 6 (**pa4/pa5**), shell on cdc-acm - wip

Put protocol version into registers
 - change version with breaking changes of registers / positions

Blank display fields if communications lost

Status LEDs
 - add support for status LED on led0
 - define led0 on custom board - done

General
 - update documentation as needed
 - investigate build errors (lvgl.h) when no display defined
 - west integration, driver(s), zephyr etc..
 - custom board support - wip

Drivers
 - make out of tree driver for adp5589
 - make out of tree driver for matrix (see zmk for inspiration)

Add TIM5 to BlackPill map (support encoder #4)
 - note PA0 is user key, may need to override in devicetree

GPIO keypad matrix
 - investigate tristate outputs, wasn't working with devicetree / pin_configure
   (safer than pulling idle rows low, though reference keypad has diodes on all keys)
 - add debouncing to input

Display
 - better formatting
 - change spindle power to load % - done

# Done

Add openocd support for custom board - done

Reference hw design - done

Zephyr 3.3 migration
 - remmove unneed G0 CAN stuff from board file (upstream) - done
 - split G0 config between board and overlay files - done
 - chery-pick/rebase/squash all development commits into something coherent - done

Drivers
 - make out of tree driver for quadrature encoders - done
   - the new in-tree qdec driver doesn't support retreiving raw values :(
   - see other examples, extend / override the in-tree qdec driver - done
   - testing for multiple encoders - done
   - fixme: build fails for other platforms (posix) - done

Zephyr 3.2 migration
 - remove platformio configuration - done
 - change folder structures - done
 - migrate display support to new lvgl version - done
 - initial CAN bus support - done
 - test canbus under posix - done
 - devicetree label deprecations - use node labels + chosen for display - done
 - make various features optional/conditional, based on devicetree overlay definitions - wip
   - modbus - done
   - canbus - done
   - display - done
   - encoders - done
   - keypad i2c - wip
   - keypad matrix - wip
 - test modbus under posix (socat to pts?)
 - test support for G0 processor - done
   - modbus - done
   - canbus (will need CAN patches until 3.3) - done
   - dma - done
   - display - done
   - encoders - done
   - keypad - done
 - map encoders to registers etc as (once!) defined in overlay - done

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
