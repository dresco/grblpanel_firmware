#include <zephyr.h>
#include <keypad.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

const struct device *i2c_dev;

int keypad_init(void)
{
    int rc;

    if (adp5589_init(&i2c_dev) != 0) {
        LOG_ERR("Error initialising ADP5589 device.");
        rc = 1;
    } else {
        LOG_INF("ADP5589 initialised successfully.");
        rc = 0;
    }

    return rc;
}

int keypad_get_event_count(void)
{
    return adp5589_get_event_count(i2c_dev);
}

int keypad_get_events(uint8_t *key_data, uint8_t event_count)
{
    return adp5589_get_register_values(i2c_dev, ADP5589_ADR_FIFO10, key_data, event_count);
}
