#include <main.h>
#include <display.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

LV_FONT_DECLARE(VeraMono_24);
LV_FONT_DECLARE(VeraMono_48);

// Using C99 designated initializers (order does not have to match the display_fields_t enum)
static display_item_t display_items[] = {
    [grbl_state]       = { .x_pos = 0,   .y_pos = 0,    .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_rpm]      = { .x_pos = 0,   .y_pos = 50,   .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_pwr]      = { .x_pos = 0,   .y_pos = 75,   .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [spindle_override] = { .x_pos = 0,   .y_pos = 125,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [feed_override]    = { .x_pos = 0,   .y_pos = 150,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [rapid_override]   = { .x_pos = 0,   .y_pos = 175,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [wcs]              = { .x_pos = 0,   .y_pos = 225,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [mpg_mode]         = { .x_pos = 0,   .y_pos = 250,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
    [jog_mode]         = { .x_pos = 0,   .y_pos = 275,  .x_size = 150,  .y_size = 25, .align = left,  .font_size = 24, .init = ""},
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

void update_values()
{
    // todo: syncronisation?

    // set all members to invalid data, so that first pass initialises all display fields
    static panel_displaydata_t last_displaydata = {.grbl_state = 0xFFFF,
                                                   .spindle_speed = 0xFFFF,
                                                   .spindle_power = 0xFFFF,
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

    if (memcmp(&last_displaydata, &panel_displaydata, sizeof(last_displaydata))) {
        // something has changed in the global display data register, check individual members

        if (panel_displaydata.grbl_state != last_displaydata.grbl_state) {
            lv_label_set_text_fmt(display_items[grbl_state].obj, "%s", state_str[panel_displaydata.grbl_state]);
        }

        if (panel_displaydata.spindle_speed != last_displaydata.spindle_speed) {
            lv_label_set_text_fmt(display_items[spindle_rpm].obj, "rpm:%6i", panel_displaydata.spindle_speed);
        }

        if (panel_displaydata.spindle_power != last_displaydata.spindle_power) {
            lv_label_set_text_fmt(display_items[spindle_pwr].obj, " kW:%6.2f", panel_displaydata.spindle_power);
        }

        if (panel_displaydata.feed_override != last_displaydata.feed_override) {
            lv_label_set_text_fmt(display_items[feed_override].obj, "f:%3i%%", panel_displaydata.feed_override);
        }

        if (panel_displaydata.rapid_override != last_displaydata.rapid_override) {
            lv_label_set_text_fmt(display_items[rapid_override].obj, "r:%3i%%", panel_displaydata.rapid_override);
        }

        if (panel_displaydata.spindle_override != last_displaydata.spindle_override) {
            lv_label_set_text_fmt(display_items[spindle_override].obj, "s:%3i%%", panel_displaydata.spindle_override);
        }

        if (panel_displaydata.wcs != last_displaydata.wcs) {
            lv_label_set_text_fmt(display_items[wcs].obj, " wcs:G%2i", panel_displaydata.wcs+54);
        }

        if (panel_displaydata.mpg_mode != last_displaydata.mpg_mode) {
            lv_label_set_text_fmt(display_items[mpg_mode].obj, "axis:%s", axis_str[panel_displaydata.mpg_mode]);
        }

        if (panel_displaydata.jog_mode != last_displaydata.jog_mode) {
            lv_label_set_text_fmt(display_items[jog_mode].obj, " jog:%s", jog_str[panel_displaydata.jog_mode]);
        }

        if (panel_displaydata.x_pos.value != last_displaydata.x_pos.value) {
            lv_label_set_text_fmt(display_items[x_axis_pos].obj, "%.3f", panel_displaydata.x_pos.value);
        }
        if (panel_displaydata.y_pos.value != last_displaydata.y_pos.value) {
            lv_label_set_text_fmt(display_items[y_axis_pos].obj, "%.3f", panel_displaydata.y_pos.value);
        }
        if (panel_displaydata.z_pos.value != last_displaydata.z_pos.value) {
            lv_label_set_text_fmt(display_items[z_axis_pos].obj, "%.3f", panel_displaydata.z_pos.value);
        }

    }

    memcpy(&last_displaydata, &panel_displaydata, sizeof(last_displaydata));
}

void item_create(display_item_t * item, char * value)
{
    item->obj = lv_label_create(lv_scr_act(), NULL);
    lv_obj_align(item->obj, NULL, LV_ALIGN_IN_TOP_LEFT, item->x_pos, item->y_pos);

    switch (item->align) {
        case right:
            lv_label_set_align(item->obj, LV_LABEL_ALIGN_RIGHT);
            break;    
        case center:
            lv_label_set_align(item->obj, LV_LABEL_ALIGN_CENTER);
            break;    
        case left:
            lv_label_set_align(item->obj, LV_LABEL_ALIGN_LEFT);
            break;    
    }

    lv_label_set_long_mode(item->obj, LV_LABEL_LONG_CROP);
    lv_obj_set_size(item->obj, item->x_size, item->y_size);

    // useful for debugging label positions...
    // lv_obj_set_style_local_bg_opa(item->obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    
    switch (item->font_size) {
        case 24:
            lv_obj_set_style_local_text_font(item->obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &VeraMono_24);
            break;
        case 48:
            lv_obj_set_style_local_text_font(item->obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &VeraMono_48);
            break;
        default:
            break;
    }

    lv_label_set_text(item->obj, item->init);
}

int display_init(void)
{
    const struct device *display_dev;

    display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

    if (display_dev == NULL) {
            LOG_ERR("Display device not found.");
            return 1;
    }

    lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, 
                           LV_THEME_DEFAULT_COLOR_SECONDARY, 
                           LV_THEME_MATERIAL_FLAG_DARK,
                           LV_THEME_DEFAULT_FONT_SMALL, 
                           LV_THEME_DEFAULT_FONT_NORMAL, 
                           LV_THEME_DEFAULT_FONT_SUBTITLE, 
                           LV_THEME_DEFAULT_FONT_TITLE);

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
