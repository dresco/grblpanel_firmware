#include <zephyr.h>
#include <adp5589.h>
#include <quadrature.h>

void main(void)
{
    const struct device *i2c_dev;
    static uint8_t key_data[16]  = {0};
    static int prev_encoder;

    printk("Hello World...! %s\n", CONFIG_BOARD);

    if (adp5589_init(&i2c_dev) != 0) {
        printk("Error initialising ADP5589 device.\n");
    } else {
        printk("ADP5589 initialised successfully.\n");
    }

    quadrature_init();

    while (1) {

        //
        // Keypad testing
        //
        uint8_t event_count =  adp5589_get_event_count(i2c_dev);

        if (event_count) {
            //printk("Event count %u, reading data..\n", event_count);

            adp5589_get_register_values(i2c_dev, ADP5589_ADR_FIFO10, key_data, event_count);
            for (int i = 0; i < event_count; i++) {
                if (key_data[i] & 0x80)
                    printk("Key 0x%02X pressed..\n", (key_data[i] & 0x7F));
                else
                    printk("Key 0x%02X released..\n", (key_data[i] & 0x7F));
            }
        }

        //
        // Encoder testing
        //
        int encoder = quadrature_get_value();
        if (encoder != prev_encoder)
            printk("Encoder value is: %d\r\n", encoder);
        prev_encoder = encoder;

        k_msleep(100);

    }
}
