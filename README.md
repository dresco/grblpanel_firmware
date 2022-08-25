Initial Zephyr implementation of a modbus RTU control panel for grblHAL  
See accompanying plugin at https://github.com/dresco/Plugin_panel

**PoC development hardware**  
ST Nucleo F446ZE  
Analog Devices ADP5589 I2C IO Expander https://www.analog.com/en/products/adp5589.html  
MSP3520 SPI TFT LCD display (ILI9488 driver) http://www.lcdwiki.com/3.5inch_SPI_Module_ILI9488_SKU:MSP3520  

**Software stack**  
This project is using Zephyr as the development framework, configured as a PlatformIO project for easy setup & IDE integration.

Zephyr provides native support for modbus and for the LVGL graphics library. However, it does not currently have driver support for STM32 timers in hardware quadrature encoder mode, or for the ADP5589 IO expander. The quadrature encoders are set up and accessed using the STM LL timer APIs, and a local driver has been implemented for the IO expander.

Using the **Bitstream Vera Sans Mono** TrueType font, converted for use with LVGL using https://lvgl.io/tools/fontconverter
