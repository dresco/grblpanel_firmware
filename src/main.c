#include <main.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

// globals
panel_keydata_1_t   panel_keydata_1;
panel_keydata_2_t   panel_keydata_2;
panel_keydata_3_t   panel_keydata_3;
panel_keydata_4_t   panel_keydata_4;
panel_displaydata_t panel_displaydata;

// mutex for accessing the global data structures
struct k_mutex paneldata_mutex;

void main(void)
{
    k_mutex_init(&paneldata_mutex);

#ifdef CONFIG_USB_CDC_ACM
    // Enable USB CDC ACM device for console output
    const struct device *dev_usb;
    dev_usb = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);
    if (dev_usb == NULL || usb_enable(NULL)) {
        return;
    }
#endif

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
        k_yield();
    }
}
