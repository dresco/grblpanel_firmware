/*
 * Modbus interface functions
 */

#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <modbus/modbus.h>

#define BAUD_RATE       38400
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
#define HREG_SPINDLE_POWER  103
#define HREG_XPOS_LO        105 // todo: figure out the byte ordering
#define HREG_XPOS_HI        106
#define HREG_YPOS_LO        107
#define HREG_YPOS_HI        108
#define HREG_ZPOS_LO        109
#define HREG_ZPOS_HI        110

//function declarations
int modbus_init(void);

#endif

