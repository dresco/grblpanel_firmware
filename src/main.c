#include <main.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

// globals
panel_keydata_1_t   panel_keydata_1;
panel_keydata_2_t   panel_keydata_2;
panel_displaydata_t panel_displaydata;

void main(void)
{
    static uint32_t prev_encoder;

    LOG_INF("Hello World...! %s", CONFIG_BOARD);

    //
    // LVGL testing
    //

    const struct device *display_dev;

    display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);

    if (display_dev == NULL) {
            LOG_ERR("device not found.  Aborting test.");
            return;
    }

    lv_obj_t *hello_world_label;
    hello_world_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(hello_world_label, "Hello world!");
    lv_obj_align(hello_world_label, NULL, LV_ALIGN_IN_TOP_MID, 5, 5);

    lv_task_handler();
    display_blanking_off(display_dev);

    if (keypad_init()) {
        LOG_ERR("Error initialising keypad device.%s", "");
    }

    quadrature_init();

    if (modbus_init()) {
        LOG_ERR("Modbus RTU server initialization failed.%s", "");
    }

    while (1) {

        //
        // Keypad testing - polling for events
        // todo: interrupt driven notification
        //
        keypad_process_events();

        //
        // Encoder testing - values for uart output only
        //
        uint32_t encoder = quadrature_get_value(1);
        if (encoder != prev_encoder)
            LOG_INF("Encoder value is: %d", encoder);
        prev_encoder = encoder;

        lv_task_handler();
        k_msleep(100);

    }
}
