/*
 * Keypad interface functions - abstract away any specific IO expander 
 */

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <adp5589.h>

// ADP5589 keycodes range from 1 to 88 if using full 8x11 matrix
#define KEY_STOP        1
#define KEY_FEEDHOLD    2
#define KEY_CYCLESTART  3

// function protoypes
int keypad_init(void);
int keypad_get_event_count(void);
int keypad_get_events(uint8_t *, uint8_t);
int keypad_process_events(void);
uint16_t keypad_get_value(uint8_t);

#endif
