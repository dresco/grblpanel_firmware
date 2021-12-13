#include <zephyr.h>

#include <keypad.h>
#include <modbus.h>
#include <quadrature.h>
#include <registers.h>
#include <display.h>

// global variable declarations
extern panel_keydata_1_t   panel_keydata_1;
extern panel_keydata_2_t   panel_keydata_2;
extern panel_keydata_3_t   panel_keydata_3;
extern panel_displaydata_t panel_displaydata;

