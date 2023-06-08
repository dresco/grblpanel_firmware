# grblPANEL Zephyr firmware

A Zephyr firmware implementation for the [grblPANEL](https://github.com/dresco/grblPANEL) control panel project. This firmware provides support for a custom STM32G0 based reference board, along with limited support for a number of STM32 Nucleo development boards.

## Software stack

This project uses [Zephyr](https://www.zephyrproject.org/) as the development framework.

Zephyr was chosen  for it's portability between processors, and for the good built-in support for Modbus, CAN bus, and the LVGL graphics libraries. 

Quadrature encoder support is provided by an out of tree driver, based on the upstream `st,stm32-qdec` sensor driver, but modified to access the necessary raw encoder values. (The sensor subsystem channel is mapped to degrees rotation in the upstream driver).

Support for the ADP5589 I/O expander, and for GPIO matrix keyboard scanning (where used), has been added to the project as local low level drivers. Intention is to port these to the Zephyr driver model in due course.

## Configuration

The available peripherals and pin mappings for each target board are configured through per-board configuration files & devicetree overlays, in addition to the project wide configuration file (prj.conf).

A custom board `grblpanel_g0b1re` has been defined for the [reference hardware implementation](https://github.com/dresco/grblpanel_main_board).

## Build instructions

The initial version was configured as a PlatformIO project for easy setup & IDE integration. However, due to limited support for later Zephyr versions, the project has since transitioned away from PlatformIO to a native Zephyr build. 

At time of writing, this project is being built and tested with Zephyr v3.3

*todo: include links for setting up a functional zephyr & west build environment*

    west build -b grblpanel_g0b1re --pristine
    west flash --runner openocd

*todo: add support & instructions for flashing custom board over DFU*

---
Using the **Bitstream Vera Sans Mono** TrueType font for display output, converted for use with LVGL using https://lvgl.io/tools/fontconverter
