/*
 * CAN bus interface functions
 */

#ifndef __CANBUS_H__
#define __CANBUS_H__

#include <zephyr/drivers/can.h>

#define CANBUS_NODE DT_CHOSEN(zephyr_canbus)

#if DT_NODE_EXISTS(CANBUS_NODE)
#define PANEL_CANBUS    1
#endif

//function declarations
int canbus_init(void);

#endif

