#include <zephyr.h>
#include <keypad.h>
#include <quadrature.h>
#include <modbus.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

void main(void)
{
    static uint8_t key_data[16]  = {0};
    static uint32_t prev_encoder;

    LOG_INF("Hello World...! %s", CONFIG_BOARD);

    if (keypad_init()) {
        LOG_ERR("Error initialising keypad device.%s", "");
    }

    quadrature_init();

    if (init_modbus_server()) {
        LOG_ERR("Modbus RTU server initialization failed.%s", "");
    }

    while (1) {

        //
        // Keypad testing
        //
        uint8_t event_count =  keypad_get_event_count();

        if (event_count) {
            //LOG_INF("Event count %u, reading data..", event_count);

            keypad_get_events(key_data, event_count);

            for (int i = 0; i < event_count; i++) {
                if (key_data[i] & 0x80)
                    LOG_INF("Key 0x%02X pressed..", (key_data[i] & 0x7F));
                else
                    LOG_INF("Key 0x%02X released..", (key_data[i] & 0x7F));
            }
        }

        //
        // Encoder testing
        //
        uint32_t encoder = quadrature_get_value();
        if (encoder != prev_encoder)
            LOG_INF("Encoder value is: %d", encoder);
        prev_encoder = encoder;

        k_msleep(100);

    }
}
