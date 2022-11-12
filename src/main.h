#include <zephyr/kernel.h>

#include <keypad.h>
#include <modbus.h>
#include <quadrature.h>
#include <registers.h>
#include <display.h>
#include <matrix.h>
#include <utils.h>

#ifdef CONFIG_USB_CDC_ACM
#include <usb/usb_device.h>
#endif

extern struct k_mutex paneldata_mutex;

// global variable declarations
extern panel_keydata_1_t   panel_keydata_1;
extern panel_keydata_2_t   panel_keydata_2;
extern panel_keydata_3_t   panel_keydata_3;
extern panel_keydata_4_t   panel_keydata_4;
extern panel_displaydata_t panel_displaydata;

