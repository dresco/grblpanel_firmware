Initial Zephyr implementation of a modbus RTU control panel for grblHAL
see accompanying plugin at https://github.com/dresco/Plugin_panel

**Initial PoC development hardware**
ST Nucleo F446ZE
Analog Devices ADP5589 IO Expander
MSP3520 SPI TFT LCD display (ILI9488 driver)

**Current software stack**
This project is using zephyr as the development framework, configured as a PlatformIO project for easy setup & IDE integration.

Zephyr provides native support for modbus and for the LVGL graphics library. However, it does not currently support STM2 timers in hardware quadrature encoder mode, or for the ADP5589 IO expander. 

The quadrature encoders will be read using the STM LL timer APIs, and a local driver will be implemented for the IO expander.