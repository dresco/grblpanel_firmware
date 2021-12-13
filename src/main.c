#include <main.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

// globals
panel_keydata_1_t   panel_keydata_1;
panel_keydata_2_t   panel_keydata_2;
panel_keydata_3_t   panel_keydata_3;
panel_displaydata_t panel_displaydata;

void main(void)
{
    LOG_INF("Hello World...! %s", CONFIG_BOARD);

    if (display_init()) {
        LOG_ERR("Error initialising display device.%s", "");
    }

    if (keypad_init()) {
        LOG_ERR("Error initialising keypad device.%s", "");
    }

    if (modbus_init()) {
        LOG_ERR("Modbus RTU server initialization failed.%s", "");
    }

    quadrature_init();

    while (1) {
        keypad_process_events();
        update_values();
        display_update();
    }
}
