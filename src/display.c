#include <main.h>
#include <display.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

LV_FONT_DECLARE(VeraMono_24);
LV_FONT_DECLARE(VeraMono_48);

static display_fields_t fields;

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

void update_values()
{
    char buf[16];

    switch (panel_displaydata.grbl_state) {
        case STATE_IDLE:
            sprintf(buf, "Idle");
            break;

        case STATE_CYCLE:
            sprintf(buf, "Run");
            break;

        case STATE_HOLD:
            sprintf(buf, "Hold");
            break;

        case STATE_JOG:
            sprintf(buf, "Jog");
            break;

        case STATE_HOMING:
            sprintf(buf, "Home");
            break;

        case STATE_ESTOP:
        case STATE_ALARM:
            sprintf(buf, "Alarm");
            break;

        case STATE_CHECK_MODE:
            sprintf(buf, "Check");
            break;

        case STATE_SAFETY_DOOR:
            sprintf(buf, "Door");
            break;

        case STATE_SLEEP:
            sprintf(buf, "Sleep");
            break;

        case STATE_TOOL_CHANGE:
            sprintf(buf, "Tool");
            break;

        default:
            sprintf(buf, "???");
            break;
    }

    lv_label_set_text_fmt(fields.grbl_state.obj, "%s", buf);

    lv_label_set_text_fmt(fields.overrides.obj, "rpm:%6i\n kW:%6.2f\n\nf:%3i%%\nr:%3i%%\ns:%3i%%\n\nG%2i\naxis:%s\njog:%s", 
                                                    panel_displaydata.spindle_speed,
                                                    (float)panel_displaydata.spindle_power / 10.0,
                                                    panel_displaydata.feed_override, 
                                                    panel_displaydata.rapid_override,
                                                    panel_displaydata.spindle_override,
                                                    panel_displaydata.wcs+54,
                                                    axis_str[panel_displaydata.mpg_mode],
                                                    jog_str[panel_displaydata.jog_mode]);

    lv_label_set_text_fmt(fields.x_axis_value.obj, "%.3f", panel_displaydata.x_pos.value);
    lv_label_set_text_fmt(fields.y_axis_value.obj, "%.3f", panel_displaydata.y_pos.value);
    lv_label_set_text_fmt(fields.z_axis_value.obj, "%.3f", panel_displaydata.z_pos.value);
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
     lv_obj_set_style_local_bg_opa(item->obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    
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

    lv_label_set_text(item->obj, value);
}

int display_init(void)
{
    const struct device *display_dev;

    display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

    if (display_dev == NULL) {
            LOG_ERR("device not found.  Aborting test.");
            return 1;
    }

    lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, 
                           LV_THEME_DEFAULT_COLOR_SECONDARY, 
                           LV_THEME_MATERIAL_FLAG_DARK,
                           LV_THEME_DEFAULT_FONT_SMALL, 
                           LV_THEME_DEFAULT_FONT_NORMAL, 
                           LV_THEME_DEFAULT_FONT_SUBTITLE, 
                           LV_THEME_DEFAULT_FONT_TITLE);

    fields.grbl_state.x_pos = 0;
    fields.grbl_state.y_pos = 0;
    fields.grbl_state.x_size = 150;
    fields.grbl_state.y_size = 25;
    fields.grbl_state.align = left;
    fields.grbl_state.font_size = 24;

    fields.overrides.x_pos = 0;
    fields.overrides.y_pos = 50;
    fields.overrides.x_size =150;
    fields.overrides.y_size = 270;
    fields.overrides.align = left;
    fields.overrides.font_size = 24;

    fields.x_axis_label.x_pos = 170;
    fields.x_axis_label.y_pos = 170;
    fields.x_axis_label.x_size = 55;
    fields.x_axis_label.y_size = 50;
    fields.x_axis_label.align = right;
    fields.x_axis_label.font_size = 48;

    fields.y_axis_label.x_pos = 170;
    fields.y_axis_label.y_pos = 220;
    fields.y_axis_label.x_size = 55;
    fields.y_axis_label.y_size = 50;
    fields.y_axis_label.align = right;
    fields.y_axis_label.font_size = 48;

    fields.z_axis_label.x_pos = 170;
    fields.z_axis_label.y_pos = 270;
    fields.z_axis_label.x_size = 55;
    fields.z_axis_label.y_size = 50;
    fields.z_axis_label.align = right;
    fields.z_axis_label.font_size = 48;

    fields.x_axis_value.x_pos = 200;
    fields.x_axis_value.y_pos = 170;
    fields.x_axis_value.x_size = 280;
    fields.x_axis_value.y_size = 50;
    fields.x_axis_value.align = right;
    fields.x_axis_value.font_size = 48;

    fields.y_axis_value.x_pos = 200;
    fields.y_axis_value.y_pos = 220;
    fields.y_axis_value.x_size = 280;
    fields.y_axis_value.y_size = 50;
    fields.y_axis_value.align = right;
    fields.y_axis_value.font_size = 48;

    fields.z_axis_value.x_pos = 200;
    fields.z_axis_value.y_pos = 270;
    fields.z_axis_value.x_size = 280;
    fields.z_axis_value.y_size = 50;
    fields.z_axis_value.align = right;
    fields.z_axis_value.font_size = 48;

    item_create(&fields.grbl_state, "IDLE");
    item_create(&fields.overrides, "");

    item_create(&fields.x_axis_label, "X:");
    item_create(&fields.y_axis_label, "Y:");
    item_create(&fields.z_axis_label, "Z:");
    
    item_create(&fields.x_axis_value, "0.000");
    item_create(&fields.y_axis_value, "0.000");
    item_create(&fields.z_axis_value, "0.000");

    // lv_obj_t *hello_world_label;
    // hello_world_label = lv_label_create(lv_scr_act(), NULL);
    // lv_label_set_text(hello_world_label, "Hello world!");
    // lv_obj_align(hello_world_label, NULL, LV_ALIGN_IN_TOP_MID, 5, 5);

    // lv_obj_t * cont;
    // cont = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_auto_realign(cont, true);                    /*Auto realign when the size changes*/
    // lv_obj_align_mid(cont, NULL, LV_ALIGN_CENTER, 0, 0);    /*This parametrs will be sued when realigned*/
    // lv_cont_set_fit(cont, LV_FIT_MAX);
    // lv_cont_set_layout(cont, LV_LAYOUT_PRETTY_MID);


    //lv_obj_t *labels[3];

    // lv_theme_t * th = lv_theme_
    // lv_theme_set_current(th);
    
    // fields.x_label = lv_label_create(lv_scr_act(), NULL);
    // lv_obj_set_style_local_text_font(fields.x_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &lv_font_montserrat_48);
    // lv_obj_align(fields.x_label, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 50);
    // lv_label_set_align(fields.x_label, LV_LABEL_ALIGN_RIGHT);
    // lv_label_set_text(fields.x_label, "X:");

    // labels[1] = lv_label_create(lv_scr_act(), NULL);
    // lv_obj_set_style_local_text_font(labels[1], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &lv_font_montserrat_48);
    // lv_obj_align(labels[1], NULL, LV_ALIGN_IN_TOP_LEFT, 200, 100);
    // lv_label_set_align(labels[1], LV_LABEL_ALIGN_RIGHT);
    // //lv_obj_align(labels[1], NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    // //lv_label_set_align(labels[1], LV_LABEL_ALIGN_CENTER);
    // lv_label_set_text(labels[1], "Y:");

    // labels[2] = lv_label_create(lv_scr_act(), NULL);
    // lv_obj_set_style_local_text_font(labels[2], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &lv_font_montserrat_48);
    // lv_obj_align(labels[2], NULL, LV_ALIGN_IN_TOP_LEFT, 200, 150);
    // lv_label_set_align(labels[2], LV_LABEL_ALIGN_RIGHT);
    // //lv_obj_align(labels[2], NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    // //lv_label_set_align(labels[2], LV_LABEL_ALIGN_CENTER);
    // lv_label_set_text(labels[2], "Z:");

    // static lv_style_t win_style;
    // lv_style_copy(&win_style, &lv_style_transp);
    // win_style.body.padding.inner = 20;

    // static lv_obj_t *win;
    // win = lv_win_create(lv_disp_get_scr_act(NULL), NULL);
    // lv_win_set_title(win, "Hello!");
    // // lv_win_set_style(win, LV_WIN_STYLE_CONTENT, &win_style);
    // lv_win_add_btn(win, LV_SYMBOL_HOME);

    // /*Set a layout for the window*/
    // lv_obj_t *cont;
    // cont = lv_win_get_content(win);
    // lv_win_set_layout(win, LV_LAYOUT_GRID);

    // lv_obj_t * headings_cont;
    // headings_cont = lv_cont_create(lv_scr_act(), NULL);
    // lv_obj_set_size(headings_cont, 100, 100);
    // lv_obj_set_pos(headings_cont, 100, 100);
    // lv_cont_set_fit(headings_cont, LV_FIT_MAX);
    // lv_cont_set_layout(headings_cont, LV_LAYOUT_COLUMN_LEFT);

    // /*Add labels*/
    // const char * txts[] = {"X:", "Y:", "Z:", NULL};
    // lv_obj_t *obj;
    // uint8_t i;
    // for(i = 0; txts[i]; i++) {
    //     obj = lv_label_create(headings_cont, NULL);
    //     lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT,  &lv_font_montserrat_48);
    //     lv_label_set_text(obj, txts[i]);
    // }

    lv_task_handler();
    display_blanking_off(display_dev);

    return 0;
}

void display_update(void)
{
    lv_task_handler();
}
