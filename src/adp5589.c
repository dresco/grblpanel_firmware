#include <adp5589.h>

int8_t adp5589_init(const struct device **i2c_dev)
{
    uint8_t rc;
    //const struct device *i2c_dev;

     // Set up I2C
    *i2c_dev = device_get_binding("I2C_1");
    if (*i2c_dev == NULL)
    {
        printk("I2C: Device driver not found.\n");
        return -1;
    }

    // Configuration
    uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;
    if (i2c_configure(*i2c_dev, i2c_cfg))
    {
        printk("I2C: config failed\n");
        return -1;
    }

    // Write a test command and check for ACK
    //uint8_t buf[] = {0};
    //rc = i2c_write(*i2c_dev, buf, sizeof(buf), 0x34); // hard coded ADP5589 address for initial test
    //
    //if (rc < 0)
    //{
    //    printk("I2C: Unable to communicate with slave device.\n");
    //    return -1;
    //}

    // Check device responds with expected device id
    rc = (adp5589_get_register_value(*i2c_dev, ADP5589_ADR_ID) & ADP5589_ID_MAN_ID);
    if (rc != ADP5589_ID) {
        printk("I2C: ADP5589 device not found.\n");
        return -1;
    }

    /* Enable internal oscillator and set clock frequency to 500 kHz. */
    adp5589_set_register_value(*i2c_dev,
                   ADP5589_ADR_GENERAL_CFG_B,
                   ADP5589_GENERAL_CFG_B_OSC_EN |
                   ADP5589_GENERAL_CFG_B_CORE_FREQ(ADP5589_GENERAL_CFG_B_CORE_FREQ_500KHZ));

    /* Enable matrix keypad decoding for the first four columns and rows */
    adp5589_set_register_value(*i2c_dev, ADP5589_ADR_PIN_CONFIG_A, 0x0F);
    adp5589_set_register_value(*i2c_dev, ADP5589_ADR_PIN_CONFIG_B, 0x0F);

    return 0; // todo: check i2c return values
}

uint8_t adp5589_get_register_value(const struct device *i2c_dev, uint8_t register_address)
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

uint8_t adp5589_get_register_values(const struct device *i2c_dev, uint8_t register_address, uint8_t *register_data, uint8_t register_count)
{
    static uint8_t write_data[2]   = {0, 0};

    write_data[0] = register_address;
    i2c_write(i2c_dev, write_data, 1, ADP5589_ADDRESS);
    i2c_read(i2c_dev, register_data, register_count, ADP5589_ADDRESS);

    return register_count; // todo: check i2c return values
}

void adp5589_set_register_value(const struct device *i2c_dev,
                uint8_t register_address,
                uint8_t register_value)
{
    static uint8_t write_data[2] = {0, 0};

    write_data[0] = register_address;
    write_data[1] = register_value;
    i2c_write(i2c_dev, write_data, 2, ADP5589_ADDRESS); // todo: check i2c return values
}

int8_t adp5589_get_event_count(const struct device  *i2c_dev)
{
    return ADP5589_STATUS_EC(adp5589_get_register_value(i2c_dev, ADP5589_ADR_STATUS));
}
