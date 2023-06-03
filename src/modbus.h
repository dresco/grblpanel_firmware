/*
 * Modbus interface functions
 */

#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <zephyr/modbus/modbus.h>

#define MODBUS_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_modbus_serial)

#if (DT_NODE_EXISTS(MODBUS_NODE))
#define PANEL_MODBUS    1
#endif

#define BAUD_RATE       19200
#define UNIT_ID         10

// Input register definitions
#define IREG_DEBUG_1    100
#define IREG_DEBUG_2    101
#define IREG_ENCODER_1  102
#define IREG_ENCODER_2  103
#define IREG_ENCODER_3  104
#define IREG_ENCODER_4  105
#define IREG_KEYPAD_1   106
#define IREG_KEYPAD_2   107
#define IREG_KEYPAD_3   108
#define IREG_KEYPAD_4   109
#define IREG_KEYPAD_5   110

// Holding register definitions
#define HREG_GRBL_STATE     100
#define HREG_SPINDLE_SPEED  102
#define HREG_SPINDLE_LOAD   103
#define HREG_OVERRIDES_1    104
#define HREG_OVERRIDES_2    105
#define HREG_MPG_MODE       106
#define HREG_XPOS_LO        107 // todo: figure out the byte ordering
#define HREG_XPOS_HI        108
#define HREG_YPOS_LO        109
#define HREG_YPOS_HI        110
#define HREG_ZPOS_LO        111
#define HREG_ZPOS_HI        112

//function declarations
int modbus_init(void);

#endif

