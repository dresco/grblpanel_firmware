/*
 * Keypad interface functions - abstract away any specific IO expander 
 */

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <adp5589.h>

// ADP5589 keycodes range from 1 to 88 if using full 8x11 matrix, the chosen keycode mappings
// are dependent on the keypad layout in use, and are set in the appropriate header file
//
// Uncomment just one mapping below;
// #define KEYPAD_4X4
// #define KEYPAD_5X5
#define KEYPAD_6X11

#include <keypad_4x4.h>
#include <keypad_5x5.h>
#include <keypad_6x11.h>
#include <keypad_defaults.h>

#define KEY_BUFFER_SIZE     16 // how many key events we can process

// function protoypes
int keypad_init(void);
int keypad_get_event_count(void);
int keypad_get_events(uint8_t *, uint8_t);
int keypad_process_events(void);
uint16_t keypad_get_value(uint8_t);
void keypad_reset_flags(void);

#endif
