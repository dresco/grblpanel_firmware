/*
 * Modbus interface functions
 */

#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <modbus/modbus.h>

#define BAUD_RATE       38400
#define UNIT_ID         10

// Input register definitions
#define IREG_ENCODER_1  102
#define IREG_ENCODER_2  103
#define IREG_ENCODER_3  104
#define IREG_ENCODER_4  105
#define IREG_KEYPAD_1   106
#define IREG_KEYPAD_2   107
#define IREG_KEYPAD_3   108
#define IREG_KEYPAD_4   109
#define IREG_KEYPAD_5   110

//function declarations
int modbus_init(void);

#endif

