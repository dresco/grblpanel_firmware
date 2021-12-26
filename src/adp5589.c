#include <main.h>
#include <adp5589.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)

const struct gpio_dt_spec keyevent =
        GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, keyevent_gpios);

const struct device *i2c_dev;

int8_t adp5589_init(void)
{
    uint8_t rc;

     // Set up I2C
    i2c_dev = device_get_binding("KEYPAD");
    if (i2c_dev == NULL)
    {
        LOG_ERR("Keypad devicetree binding not found");
        return -1;
    }

    // Configure the event interrupt pin
    gpio_pin_configure_dt(&keyevent, GPIO_INPUT);

    // Configuration
    uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;
    if (i2c_configure(i2c_dev, i2c_cfg))
    {
        LOG_ERR("Keypad i2c config failed");
        return -1;
    }

    // Check device responds with expected device id
    rc = (adp5589_get_register_value(ADP5589_ADR_ID) & ADP5589_ID_MAN_ID);
    if (rc != ADP5589_ID) {
        LOG_ERR("ADP5589 IO expander not found");
        return -1;
    }

    // Enable internal oscillator and set clock frequency to 500 kHz
    adp5589_set_register_value(ADP5589_ADR_GENERAL_CFG_B,
                               ADP5589_GENERAL_CFG_B_OSC_EN |
                               ADP5589_GENERAL_CFG_B_CORE_FREQ(ADP5589_GENERAL_CFG_B_CORE_FREQ_500KHZ));

    // Enable matrix keypad decoding for the requested columns and rows (note: max of 8 without using C register)
#if KEYPAD_ROWS && KEYPAD_COLS
    adp5589_set_register_value(ADP5589_ADR_PIN_CONFIG_A, KEY_PINMAP(KEYPAD_ROWS));
    adp5589_set_register_value(ADP5589_ADR_PIN_CONFIG_B, KEY_PINMAP(KEYPAD_COLS));
#endif

    // Enable the interrupt pin to signal keypad events available
    adp5589_set_register_value(ADP5589_ADR_INT_EN, ADP5589_INT_EN_EVENT_IEN);

    return 0; // todo: check i2c return values
}

uint8_t adp5589_get_register_value(uint8_t register_address)
{
    static uint8_t write_data[2]  = {0};
    static uint8_t read_data[2]   = {0};
    static uint8_t register_value = 0;

    write_data[0] = register_address;
    i2c_write(i2c_dev, write_data, 1, ADP5589_ADDRESS);
    i2c_read(i2c_dev, read_data, 1, ADP5589_ADDRESS);
    register_value = read_data[0];

    return register_value; // todo: check i2c return values
}

uint8_t adp5589_get_register_values(uint8_t register_address, uint8_t *register_data, uint8_t register_count)
{
    static uint8_t write_data[2]   = {0, 0};

    write_data[0] = register_address;
    i2c_write(i2c_dev, write_data, 1, ADP5589_ADDRESS);
    i2c_read(i2c_dev, register_data, register_count, ADP5589_ADDRESS);

    return register_count; // todo: check i2c return values
}

void adp5589_set_register_value(uint8_t register_address, uint8_t register_value)
{
    static uint8_t write_data[2] = {0, 0};

    write_data[0] = register_address;
    write_data[1] = register_value;
    i2c_write(i2c_dev, write_data, 2, ADP5589_ADDRESS); // todo: check i2c return values
}

int8_t adp5589_get_event_count(void)
{
    if (gpio_pin_get(keyevent.port, keyevent.pin)) {
        // Events available (note that the pin is defined as active low)

        // Clear the event interrupt
        adp5589_set_register_value(ADP5589_ADR_INT_STATUS, ADP5589_INT_STATUS_EVENT_INT);

        // Return the event count
        return ADP5589_STATUS_EC(adp5589_get_register_value(ADP5589_ADR_STATUS));

    } else {
        // No pending events
        return 0;
    }
}
