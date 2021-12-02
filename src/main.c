#include <main.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

// globals
panel_keydata_1_t panel_keydata_1;

void main(void)
{
    static uint32_t prev_encoder;

    LOG_INF("Hello World...! %s", CONFIG_BOARD);

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
        // Encoder testing - valuse for uart output only
        //
        uint32_t encoder = quadrature_get_value(1);
        if (encoder != prev_encoder)
            LOG_INF("Encoder value is: %d", encoder);
        prev_encoder = encoder;

        k_msleep(100);

    }
}
