
#ifndef __KEYPAD_6X11_H__
#define __KEYPAD_6X11_H__

#ifdef KEYPAD_6X11

// Keycode mappings for 6x11 reference keypad layout..

#define KEYPAD_ROWS         6
#define KEYPAD_COLS         11

#define KEY_CYCLESTART      11
#define KEY_FEEDHOLD        22
#define KEY_RESET           33

#define KEY_JOG_X_P         14
#define KEY_JOG_X_N         12
#define KEY_JOG_Y_P         2
#define KEY_JOG_Y_N         24
#define KEY_JOG_Z_P         4
#define KEY_JOG_Z_N         26
#define KEY_JOG_A_P         5
#define KEY_JOG_A_N         27

#define KEY_JOG_STEP_X1     7
#define KEY_JOG_STEP_X10    8
#define KEY_JOG_STEP_X100   9

#define KEY_MPG_AXIS_X      18
#define KEY_MPG_AXIS_Y      19
#define KEY_MPG_AXIS_Z      20
#define KEY_MPG_AXIS_A      21

#define KEY_WCS_G54         40
#define KEY_WCS_G55         41
#define KEY_WCS_G56         42
#define KEY_WCS_G57         43

#define KEY_SET_ZERO_X      45
#define KEY_SET_ZERO_Y      46
#define KEY_SET_ZERO_Z      47
#define KEY_SET_ZERO_A      48

#define KEY_GOTO_ZERO_X     56
#define KEY_GOTO_ZERO_Y     57
#define KEY_GOTO_ZERO_Z     58
#define KEY_GOTO_ZERO_A     59

#define KEY_FUNCTION_F1     62
#define KEY_FUNCTION_F2     63
#define KEY_FUNCTION_F3     64
#define KEY_FUNCTION_F4     65

#define KEY_FEED_RESET      44
#define KEY_SPINDLE_RESET   55
#define KEY_RAPID_RESET     66

#endif

#endif