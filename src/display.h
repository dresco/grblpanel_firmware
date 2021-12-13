/*
 * Display interface functions
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <drivers/display.h>
#include <lvgl.h>

// function protoypes
int display_init(void);
void display_update(void);
void update_values(void);

// grbl states, from system.h
#define STATE_IDLE          0      //!< Must be zero. No flags.
#define STATE_ALARM         1 << 0 //!< In alarm state. Locks out all g-code processes. Allows settings access.
#define STATE_CHECK_MODE    1 << 1 //!< G-code check mode. Locks out planner and motion only.
#define STATE_HOMING        1 << 2 //!< Performing homing cycle
#define STATE_CYCLE         1 << 3 //!< Cycle is running or motions are being executed.
#define STATE_HOLD          1 << 4 //!< Active feed hold
#define STATE_JOG           1 << 5 //!< Jogging mode.
#define STATE_SAFETY_DOOR   1 << 6 //!< Safety door is ajar. Feed holds and de-energizes system.
#define STATE_SLEEP         1 << 7 //!< Sleep state.
#define STATE_ESTOP         1 << 8 //!< EStop mode, reports and is mainly handled similar to alarm state
#define STATE_TOOL_CHANGE   1 << 9 //!< Manual tool change, similar to #STATE_HOLD - but stops spindle and allows jogging.

typedef enum {
    left,
    center,
    right
} display_align_t;

typedef enum {
    grbl_state = 0,
    spindle_rpm,
    spindle_pwr,
    spindle_override,
    feed_override,
    rapid_override,
    wcs,
    mpg_mode,
    jog_mode,
    x_axis_label,
    y_axis_label,
    z_axis_label,
    x_axis_pos,
    y_axis_pos,
    z_axis_pos,
} display_fields_t;

typedef struct {
    lv_obj_t * obj;
    display_align_t align;
    uint8_t font_size;
    uint16_t x_pos, y_pos;
    uint16_t x_size, y_size;
    char * init;
} display_item_t;

typedef struct {
    uint8_t id;
    char text[8];
} display_text_t;

#endif
