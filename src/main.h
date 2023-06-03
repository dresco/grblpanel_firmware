#include <zephyr/kernel.h>

#include <modbus.h>
#include <canbus.h>
#include <canbus_ids.h>
#include <keypad.h>
#include <keypad_bitfields.h>
#include <quadrature.h>
#include <display.h>
#include <matrix.h>
#include <utils.h>
#include <stdio.h>

#include <zephyr/drivers/pwm.h>

#ifdef CONFIG_USB_CDC_ACM
#include <zephyr/usb/usb_device.h>
#endif

extern struct k_mutex paneldata_mutex;

typedef union {
    float   value;
    uint8_t bytes[4];
} float32_data_t;

typedef struct {
    uint16_t  grbl_state;
    uint16_t  spindle_speed;
    uint16_t  spindle_load;
    uint8_t   spindle_override;
    uint8_t   feed_override;
    uint8_t   rapid_override;
    uint8_t   wcs;
    uint8_t   mpg_mode;
    uint8_t   jog_mode;

    float32_data_t x_pos;
    float32_data_t y_pos;
    float32_data_t z_pos;
    float32_data_t a_pos;
    float32_data_t b_pos;
    float32_data_t c_pos;
    float32_data_t u_pos;
    float32_data_t v_pos;

} panel_displaydata_t;

// global variable declarations
extern panel_keydata_1_t   panel_keydata_1;
extern panel_keydata_2_t   panel_keydata_2;
extern panel_keydata_3_t   panel_keydata_3;
extern panel_keydata_4_t   panel_keydata_4;
extern panel_keydata_5_t   panel_keydata_5;
extern panel_displaydata_t panel_displaydata;
