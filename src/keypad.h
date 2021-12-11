/*
 * Keypad interface functions - abstract away any specific IO expander 
 */

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <adp5589.h>

#define KEY_BUFFER_SIZE     16 // how many key events we can process

// ADP5589 keycodes range from 1 to 88 if using full 8x11 matrix
#define KEY_STOP            1
#define KEY_FEEDHOLD        2
#define KEY_CYCLESTART      3
#define KEY_JOG_STEP_X1     12
#define KEY_JOG_STEP_X10    13
#define KEY_JOG_STEP_X100   14
#define KEY_JOG_STEP_SMOOTH 15
#define KEY_MPG_AXIS_X      23
#define KEY_MPG_AXIS_Y      24
#define KEY_MPG_AXIS_Z      25

// function protoypes
int keypad_init(void);
int keypad_get_event_count(void);
int keypad_get_events(uint8_t *, uint8_t);
int keypad_process_events(void);
uint16_t keypad_get_value(uint8_t);
void keypad_reset_flags(void);

#endif
