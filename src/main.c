#include <zephyr.h>
#include <adp5589.h>
#include <quadrature.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

void main(void)
{
    const struct device *i2c_dev;
    static uint8_t key_data[16]  = {0};
    static uint32_t prev_encoder;

    LOG_INF("Hello World...! %s", CONFIG_BOARD);

    if (adp5589_init(&i2c_dev) != 0) {
        LOG_ERR("Error initialising ADP5589 device.");
    } else {
        LOG_INF("ADP5589 initialised successfully.");
    }

    quadrature_init();

    while (1) {

        //
        // Keypad testing -- todo: abstract more of this away into wrapper functions
        //
        uint8_t event_count =  adp5589_get_event_count(i2c_dev);

        if (event_count) {
            //LOG_INF("Event count %u, reading data..", event_count);

            adp5589_get_register_values(i2c_dev, ADP5589_ADR_FIFO10, key_data, event_count);
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
