#include <main.h>

#if PANEL_DISPLAY

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

LV_FONT_DECLARE(VeraMono_24);
LV_FONT_DECLARE(VeraMono_48);

// Using C99 designated initializers (order does not have to match the display_fields_t enum)
static display_item_t display_items[] = {
    [grbl_state]       = { .x_pos = 0,   .y_pos = 0,    .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_rpm]      = { .x_pos = 0,   .y_pos = 50,   .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_load]     = { .x_pos = 0,   .y_pos = 75,   .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [feed_override]    = { .x_pos = 0,   .y_pos = 125,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_override] = { .x_pos = 0,   .y_pos = 150,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [rapid_override]   = { .x_pos = 0,   .y_pos = 175,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [wcs]              = { .x_pos = 0,   .y_pos = 225,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [mpg_mode]         = { .x_pos = 0,   .y_pos = 250,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [jog_mode]         = { .x_pos = 0,   .y_pos = 275,  .x_size = 160,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [x_axis_label]     = { .x_pos = 170, .y_pos = 150,  .x_size = 55,   .y_size = 50, .align = right, .font_size = 48, .init = "X:"},
    [y_axis_label]     = { .x_pos = 170, .y_pos = 200,  .x_size = 55,   .y_size = 50, .align = right, .font_size = 48, .init = "Y:"},
    [z_axis_label]     = { .x_pos = 170, .y_pos = 250,  .x_size = 55,   .y_size = 50, .align = right, .font_size = 48, .init = "Z:"},
    [x_axis_pos]       = { .x_pos = 225, .y_pos = 150,  .x_size = 255,  .y_size = 50, .align = right, .font_size = 48, .init = ""},
    [y_axis_pos]       = { .x_pos = 225, .y_pos = 200,  .x_size = 255,  .y_size = 50, .align = right, .font_size = 48, .init = ""},
    [z_axis_pos]       = { .x_pos = 225, .y_pos = 250,  .x_size = 255,  .y_size = 50, .align = right, .font_size = 48, .init = ""},
};

const char * const axis_str[] =
{
    [0] = "X",
    [1] = "Y",
    [2] = "Z",
    [3] = "A",
    [4] = "B",
    [5] = "C",
};

const char * const jog_str[] =
{
    [1] = "x1",
    [2] = "x10",
    [4] = "x100",
    [8] = "smooth",
};

const char * const state_str[] =
{
    [STATE_IDLE] = "Idle",
    [STATE_ALARM] = "Alarm",
    [STATE_CHECK_MODE] = "Check",
    [STATE_HOMING] = "Home",
    [STATE_CYCLE] = "Run",
    [STATE_HOLD] = "Hold",
    [STATE_JOG] = "Jog",
    [STATE_SAFETY_DOOR] = "Door",
    [STATE_SLEEP] = "Sleep",
    [STATE_ESTOP] = "Alarm",
    [STATE_TOOL_CHANGE] = "Tool",
};

void display_update_values()
{
    // set all members to invalid data, so that first pass initialises all display fields
    static panel_displaydata_t last_displaydata = {.grbl_state = 0xFFFF,
                                                   .spindle_speed = 0xFFFF,
                                                   .spindle_load = 0xFFFF,
                                                   .spindle_override = 0xFF,
                                                   .feed_override = 0xFF,
                                                   .rapid_override = 0xFF,
                                                   .wcs = 0xFF,
                                                   .mpg_mode = 0xFF,
                                                   .jog_mode = 0xFF,
                                                   .x_pos.value = 0xFFFF,
                                                   .y_pos.value = 0xFFFF,
                                                   .z_pos.value = 0xFFFF,
                                                   };

    // Thread syncronisation
    panel_displaydata_t local_displaydata;
    k_mutex_lock(&paneldata_mutex, K_FOREVER);
    memcpy(&local_displaydata, &panel_displaydata, sizeof(local_displaydata));
    k_mutex_unlock(&paneldata_mutex);

    if (memcmp(&last_displaydata, &local_displaydata, sizeof(last_displaydata))) {
        // something has changed in the global display data register, check individual members

        if (local_displaydata.grbl_state != last_displaydata.grbl_state) {
            lv_label_set_text_fmt(display_items[grbl_state].obj, "%s", state_str[local_displaydata.grbl_state]);
        }

        if (local_displaydata.spindle_speed != last_displaydata.spindle_speed) {
            lv_label_set_text_fmt(display_items[spindle_rpm].obj, " rpm:%5i", local_displaydata.spindle_speed);
        }

        if (local_displaydata.spindle_load != last_displaydata.spindle_load) {
            lv_label_set_text_fmt(display_items[spindle_load].obj, "load:%5i%%", local_displaydata.spindle_load);
        }

        if (local_displaydata.feed_override != last_displaydata.feed_override) {
            lv_label_set_text_fmt(display_items[feed_override].obj, "   f:%3i%%", local_displaydata.feed_override);
        }

        if (local_displaydata.rapid_override != last_displaydata.rapid_override) {
            lv_label_set_text_fmt(display_items[rapid_override].obj, "   r:%3i%%", local_displaydata.rapid_override);
        }

        if (local_displaydata.spindle_override != last_displaydata.spindle_override) {
            lv_label_set_text_fmt(display_items[spindle_override].obj, "   s:%3i%%", local_displaydata.spindle_override);
        }

        if (local_displaydata.wcs != last_displaydata.wcs) {
            lv_label_set_text_fmt(display_items[wcs].obj, " wcs:G%2i", local_displaydata.wcs+54);
        }

        if (local_displaydata.mpg_mode != last_displaydata.mpg_mode) {
            lv_label_set_text_fmt(display_items[mpg_mode].obj, "axis:%s", axis_str[local_displaydata.mpg_mode]);
        }

        if (local_displaydata.jog_mode != last_displaydata.jog_mode) {
            lv_label_set_text_fmt(display_items[jog_mode].obj, " jog:%s", jog_str[local_displaydata.jog_mode]);
        }

        if (local_displaydata.x_pos.value != last_displaydata.x_pos.value) {
            lv_label_set_text_fmt(display_items[x_axis_pos].obj, "%.3f", local_displaydata.x_pos.value);
        }
        if (local_displaydata.y_pos.value != last_displaydata.y_pos.value) {
            lv_label_set_text_fmt(display_items[y_axis_pos].obj, "%.3f", local_displaydata.y_pos.value);
        }
        if (local_displaydata.z_pos.value != last_displaydata.z_pos.value) {
            lv_label_set_text_fmt(display_items[z_axis_pos].obj, "%.3f", local_displaydata.z_pos.value);
        }

    }

    memcpy(&last_displaydata, &local_displaydata, sizeof(last_displaydata));
}

void item_create(display_item_t * item, char * value)
{
    // v8 style settings...
    static lv_style_t style_align_left;
    lv_style_init(&style_align_left);
    lv_style_set_text_align(&style_align_left, LV_TEXT_ALIGN_LEFT);

    static lv_style_t style_align_center;
    lv_style_init(&style_align_center);
    lv_style_set_text_align(&style_align_center, LV_TEXT_ALIGN_CENTER);

    static lv_style_t style_align_right;
    lv_style_init(&style_align_right);
    lv_style_set_text_align(&style_align_right, LV_TEXT_ALIGN_RIGHT);

    static lv_style_t style_Vera24;
    lv_style_init(&style_Vera24);
    lv_style_set_text_font(&style_Vera24, &VeraMono_24); /* styles are just property-value pairs now, no state or part */

    static lv_style_t style_Vera48;
    lv_style_init(&style_Vera48);
    lv_style_set_text_font(&style_Vera48, &VeraMono_48); /* styles are just property-value pairs now, no state or part */

    static lv_style_t style_opa50;
    lv_style_init(&style_opa50);
    lv_style_set_bg_opa(&style_opa50, LV_OPA_50);

    item->obj = lv_label_create(lv_scr_act());
    lv_obj_align(item->obj, LV_ALIGN_OUT_TOP_LEFT, item->x_pos, item->y_pos);

    switch (item->align) {
        case right:
            lv_obj_add_style(item->obj, &style_align_right, LV_PART_MAIN);
            break;    
        case center:
            lv_obj_add_style(item->obj, &style_align_center, LV_PART_MAIN);
            break;    
        case left:
            lv_obj_add_style(item->obj, &style_align_left, LV_PART_MAIN);
            break;    
    }

    lv_label_set_long_mode(item->obj, LV_LABEL_LONG_CLIP);
    lv_obj_set_size(item->obj, item->x_size, item->y_size);

    // useful for debugging label positions...
    // lv_obj_add_style(item->obj, &style_opa50, LV_PART_MAIN);

    switch (item->font_size) {
        case 24:
            lv_obj_add_style(item->obj, &style_Vera24, LV_PART_MAIN);
            break;
        case 48:
            lv_obj_add_style(item->obj, &style_Vera48, LV_PART_MAIN);
            break;
        default:
            break;
    }

    lv_label_set_text(item->obj, item->init);
}

int display_init(void)
{
    const struct device *display_dev;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device not found.");
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE(display_items); i++) {
        item_create(&display_items[i], "");
    }

    lv_task_handler();
    display_blanking_off(display_dev);

    return 0;
}

void display_update(void)
{
    lv_task_handler();
}

#endif