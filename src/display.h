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

typedef struct {
    uint8_t font_size;
    uint16_t x_pos, y_pos, x_size, y_size;
    display_align_t align;
    lv_obj_t * obj;
} display_item_t;

typedef struct {
    display_item_t grbl_state;
    display_item_t overrides;
    display_item_t x_axis_label;
    display_item_t x_axis_value;
    display_item_t y_axis_label;
    display_item_t y_axis_value;
    display_item_t z_axis_label;
    display_item_t z_axis_value;
} display_fields_t;

typedef struct {
    uint8_t id;
    char text[8];
} display_text_t;


// static display_text_t axis_names[] = {
//     { .id = 0,          .text = "X:" },
//     { .id = 1,          .text = "Y:" },
//     { .id = 2,          .text = "Z:" },
//     { .id = 3,          .text = "A:" },
//     { .id = 4,          .text = "B:" }
// };

// static display_text_t jog_names[] = {
//     { .id = 1,          .text = "x1" },
//     { .id = 2,          .text = "x10" },
//     { .id = 4,          .text = "x100" },
//     { .id = 8,          .text = "smooth" }
// };

#endif
