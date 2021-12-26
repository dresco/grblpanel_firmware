
#ifndef __KEYPAD_DEFAULTS_H__
#define __KEYPAD_DEFAULTS_H__

// Define any unmapped keys to invalid keycode mappings (valid ADP5589 keycodes range from 1 to 88)
// this avoids compilation errors for unused keys..

#ifndef KEYPAD_ROWS
#define KEYPAD_ROWS         0
#endif

#ifndef KEYPAD_COLS
#define KEYPAD_COLS         0
#endif

#ifndef KEY_STOP
#define KEY_STOP            100
#endif

#ifndef KEY_RESET
#define KEY_RESET           101
#endif

#ifndef KEY_FEEDHOLD
#define KEY_FEEDHOLD        102
#endif

#ifndef KEY_CYCLESTART
#define KEY_CYCLESTART      103
#endif

#ifndef KEY_JOG_STEP_X1
#define KEY_JOG_STEP_X1     104
#endif

#ifndef KEY_JOG_STEP_X10
#define KEY_JOG_STEP_X10    105
#endif

#ifndef KEY_JOG_STEP_X100
#define KEY_JOG_STEP_X100   106
#endif

#ifndef KEY_JOG_STEP_SMOOTH
#define KEY_JOG_STEP_SMOOTH 107
#endif

#ifndef KEY_MPG_AXIS_X
#define KEY_MPG_AXIS_X      108
#endif

#ifndef KEY_MPG_AXIS_Y
#define KEY_MPG_AXIS_Y      109
#endif

#ifndef KEY_MPG_AXIS_Z
#define KEY_MPG_AXIS_Z      110
#endif

#endif
