#include <main.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

// globals
panel_keydata_1_t   panel_keydata_1;
panel_keydata_2_t   panel_keydata_2;
panel_keydata_3_t   panel_keydata_3;
panel_keydata_4_t   panel_keydata_4;
panel_keydata_5_t   panel_keydata_5;
panel_displaydata_t panel_displaydata;

// mutex for accessing the global data structures
struct k_mutex paneldata_mutex;


#define PWM_BACKLIGHT_NODE DT_NODELABEL(pwm_dev0)
#if (DT_NODE_EXISTS(PWM_BACKLIGHT_NODE))
#define PANEL_BACKLIGHT 1
#endif

void main(void)
{
    //static uint16_t q, q1, q2, q3, q4; // temporary quadrature encoder values for debug output

    k_mutex_init(&paneldata_mutex);

#ifdef PANEL_BACKLIGHT
    // PWM backlight control
    static const struct pwm_dt_spec pwm_backlight = PWM_DT_SPEC_GET(PWM_BACKLIGHT_NODE);
    if (device_is_ready(pwm_backlight.dev)) {
        uint32_t period = PWM_MSEC(10);
        pwm_set_dt(&pwm_backlight, period, period/2);
    }
#endif

#ifdef CONFIG_USB_CDC_ACM

    // Enable USB CDC ACM device for shell output
    const struct device *const dev_usb = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
    uint32_t dtr = 0;

    if (!device_is_ready(dev_usb)) {
        LOG_ERR("CDC ACM device not ready");
        return;
    }

    int ret = usb_enable(NULL);

    if (ret != 0) {
        LOG_ERR("Failed to enable USB");
        return;
    }

    /* Poll if the DTR flag was set */
    // LOG_ERR("USB device has been enabled, polling DTR...");
    // while (!dtr) {
    //     uart_line_ctrl_get(dev_usb, UART_LINE_CTRL_DTR, &dtr);
    //     /* Give CPU resources to low priority threads. */
    //     k_sleep(K_MSEC(100));
    // }
    // LOG_ERR("DTR ready...");

#endif

    LOG_INF("Hello World :) ...! %s", CONFIG_BOARD);


#if PANEL_DISPLAY
    if (display_init()) {
        LOG_ERR("Error initialising display device.%s", "");
    }
#endif

#if PANEL_KEYPAD
    if (keypad_init()) {
        LOG_ERR("Error initialising keypad device.%s", "");
    }
#endif

#if PANEL_ENCODER
    quadrature_init();
#endif

#if PANEL_MODBUS
    if (modbus_init()) {
        LOG_ERR("Modbus RTU server initialization failed.%s", "");
    }
#endif

#if PANEL_CANBUS
    if (canbus_init()) {
        LOG_ERR("CAN bus initialization failed.%s", "");
    }
#endif

    while (1) {

#if PANEL_KEYPAD
        keypad_process_events();
#endif

#if PANEL_DISPLAY
        display_update_values();
        display_update();
#endif

// // testing
// #if PANEL_KEYPAD
//         keypad_reset_flags(); // allow queued events to be processed
// #endif

// // testing
//         k_msleep(100);
//         q = quadrature_get_value(1);
//         if (q1 != q) {
//             printk("Enc1: %u\n", q);
//             q1 = q;
//         }
//         q = quadrature_get_value(2);
//         if (q2 != q) {
//             printk("Enc2: %u\n", q);
//             q2 = q;
//         }
//         q = quadrature_get_value(3);
//         if (q3 != q) {
//             printk("Enc3: %u\n", q);
//             q3 = q;
//         }
//         q = quadrature_get_value(4);
//         if (q4 != q) {
//             printk("Enc4: %u\n", q);
//             q4 = q;
//         }



        k_yield();
    }
}
