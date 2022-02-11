/*
 * GPIO keypad matrix functions
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <drivers/gpio.h>
#include <sys/ring_buffer.h>

typedef struct {
    const char          *portname;
    const struct device *port;
    const gpio_pin_t     pin;
} matrix_row_t;

typedef struct {
    const char          *portname;
    const struct device *port;
    const gpio_pin_t     pin;
} matrix_col_t;

typedef struct {
    struct  ring_buf rb;
    uint8_t          buffer[KEY_BUFFER_SIZE];
} matrix_events_t;

//
// Function declarations
//
int8_t  matrix_init(void);
int8_t  matrix_get_event_count(void);
uint8_t matrix_get_events(uint8_t *, uint8_t);

#endif