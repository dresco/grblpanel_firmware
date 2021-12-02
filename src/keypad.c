#include <main.h>
#include <keypad.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

const struct device *i2c_dev;

int keypad_init(void)
{
    if (adp5589_init(&i2c_dev) != 0) {
        LOG_ERR("Error initialising ADP5589 device.");
        return 1;
    } else {
        LOG_INF("ADP5589 initialised successfully.");
        return 0;
    }
}

int keypad_get_event_count(void)
{
    return adp5589_get_event_count(i2c_dev);
}

int keypad_get_events(uint8_t *key_data, uint8_t event_count)
{
    return adp5589_get_register_values(i2c_dev, ADP5589_ADR_FIFO10, key_data, event_count);
}

uint16_t keypad_get_value(uint8_t instance)
{
    // todo: add thread syncronisation?
    switch (instance) {
        case 1:
            return panel_keydata_1.value;

        default:
            return 0;
    }
}

int keypad_process_events(void)
{
    uint8_t key_data[16]  = {0};

    uint8_t event_count = keypad_get_event_count();
    if (event_count) {
        //LOG_INF("Event count %u, reading data..", event_count);

        // read event_count entries into the key_data array, ensure don't overflow the array size
        keypad_get_events(key_data, MIN(event_count, ARRAY_SIZE(key_data)));

        // debug / testing..
        for (int i = 0; i < event_count; i++) {
            if (key_data[i] & 0x80)
                LOG_INF("Key 0x%02X pressed..", (key_data[i] & 0x7F));
            else
                LOG_INF("Key 0x%02X released..", (key_data[i] & 0x7F));
        }

        // insert key states into the global registers that are sent over modbus
        // todo: add thread syncronisation?
        for (int i = 0; i < event_count; i++) {

            uint8_t keydata = key_data[i];

            switch (KEY_CODE(keydata)) {
                case KEY_STOP:
                    panel_keydata_1.stop = KEY_PRESSED(keydata);
                    break;

                case KEY_FEEDHOLD:
                    panel_keydata_1.feed_hold = KEY_PRESSED(keydata);
                    break;

                case KEY_CYCLESTART:
                    panel_keydata_1.cycle_start = KEY_PRESSED(keydata);
                    break;

                default:
                    break;
            }

        }

    }
    return event_count;
}
