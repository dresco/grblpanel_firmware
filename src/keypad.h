/*
 * Keypad interface functions - abstract away any specific IO expander 
 */

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <adp5589.h>

// function protoypes
int keypad_init(void);
int keypad_get_event_count(void);
int keypad_get_events(uint8_t *, uint8_t);

#endif
