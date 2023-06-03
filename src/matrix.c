#include <main.h>

#ifdef KEYPAD_GPIO_MATRIX

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

#define STACKSIZE 1024    // Matrix processing thread stack size
#define PRIORITY  -10     // Matrix processing thread priority

static bool init_okay;

static matrix_row_t matrix_rows[] = {
    [0]       = { .portname = "GPIOB", .pin = 0 },
    [1]       = { .portname = "GPIOB", .pin = 1 },
    [2]       = { .portname = "GPIOB", .pin = 2 },
    [3]       = { .portname = "GPIOB", .pin = 8 },
    [4]       = { .portname = "GPIOB", .pin = 9 },
    [5]       = { .portname = "GPIOB", .pin = 10 },
};

static matrix_col_t matrix_col = { .portname = "GPIOC" };

static matrix_events_t matrix_events;

int8_t  matrix_init(void)
{
    int ret;

    // Sanity check
    if (ARRAY_SIZE(matrix_rows) != KEYPAD_ROWS) {
        LOG_ERR("matrix: unexpected keypad matrix row count");
        return 1;
    }

    // Configure row outputs
    for (int i = 0; i < ARRAY_SIZE(matrix_rows); i++) {

        // get port binding
        matrix_rows[i].port = device_get_binding(matrix_rows[i].portname);
        if (matrix_rows[i].port == NULL) {
            LOG_ERR("matrix: failed to get port binding for %s", matrix_rows[i].portname);
            return 2;
        }

        // configure the pin
        // todo: open drain (or open source) not working - no output on pin..
        ret = gpio_pin_configure(matrix_rows[i].port, matrix_rows[i].pin, GPIO_OUTPUT_LOW);
        if (ret) {
            LOG_ERR("matrix: failed to configure matrix row pin %i", i);
            return 3;
        }

    }

    // Configue column inputs
    // note: assumes the pins are consecutive from 0, and on the same port

    matrix_col.port = device_get_binding(matrix_col.portname);
    if (matrix_col.port == NULL) {
        LOG_ERR("matrix: failed to get port binding for %s", matrix_col.portname);
        return 2;
    }

    for (int i = 0; i < KEYPAD_COLS; i++) {
        ret = gpio_pin_configure(matrix_col.port, i, GPIO_INPUT | GPIO_PULL_DOWN);
        if (ret) {
            LOG_ERR("matrix: failed to configure column pin %i", i);
            return 3;
        }
    }

    // Configure ring buffer for queued key data
    ring_buf_init(&matrix_events.rb, sizeof(matrix_events.buffer), matrix_events.buffer);

    init_okay = true;
    return 0;
}

int8_t matrix_get_event_count(void)
{
    return ring_buf_size_get(&matrix_events.rb);
}

uint8_t matrix_get_events(uint8_t *data, uint8_t count)
{
    int ret = ring_buf_get(&matrix_events.rb, data, count);
    if (ret != count)
        LOG_WRN("matrix: ring buffer, asked for %i bytes, got %i", count, ret);

    return ret;
}

void matrix_thread(void)
{
    int ret;
    unsigned int port_data;

    uint8_t keycode;
    uint8_t current_row = 0;
    uint8_t row_count = ARRAY_SIZE(matrix_rows);
    uint16_t prev_row_data[row_count];
    uint16_t row_data[row_count];

    LOG_INF("Starting matrix thread...");

    LOG_INF("Waiting for matrix driver to be initialised");
    while (!init_okay) {
        k_msleep(100);
    }

    memset(row_data, 0, ARRAY_SIZE(row_data));
    memset(prev_row_data, 0, ARRAY_SIZE(prev_row_data));

    while (1) {

        LOG_DBG("Asserting output pin for row %i", current_row);
        gpio_pin_set_raw(matrix_rows[current_row].port, matrix_rows[current_row].pin, 1);

        // yeild while the output state settles
        k_msleep(5);

        LOG_DBG("Reading column pins for row %i", current_row);
        ret = gpio_port_get(matrix_col.port, &port_data);
        if (ret)
            LOG_ERR("matrix: failed to retrieve pin states");

        // read the port state into the current row data, mask with first 11 bits
        // note: assumes the pins are consecutive from 0
        row_data[current_row] = (port_data & KEY_PINMAP(KEYPAD_COLS));

        if (row_data[current_row] != prev_row_data[current_row]) {
            // something has changed on this row - check individual bits?
            LOG_DBG("matrix: something has changed on row %i", current_row);
            for (int i = 0; i < 11 ; i++) {
                bool prev_row_bit = (prev_row_data[current_row] & (1 << i));
                bool this_row_bit = (row_data[current_row] & (1 << i));

                if (this_row_bit != prev_row_bit) {
                    // this bit has changed...

                    // present the keydata in the same format as the IO expander
                    // low 7 bits are keycode, 8th bit is pressed/released status
                    keycode = ((current_row * KEYPAD_COLS) + i+1);
                    keycode = keycode | (this_row_bit ? 0x80 : 0x00);

                    LOG_DBG("matrix: key %i changed (col %i on row %i), state %i",
                            keycode & 0x7F, // mask out status bit
                            i+1,
                            current_row+1,
                            this_row_bit);


                    // add key change state to buffer..
                    ring_buf_put(&matrix_events.rb, &keycode, sizeof(keycode));
                }

            }

            prev_row_data[current_row] = row_data[current_row];
        }

        LOG_DBG("Clearing output pin for row %i", current_row);
        gpio_pin_set_raw(matrix_rows[current_row].port, matrix_rows[current_row].pin, 0);

        // increment row count, wrap around to beginning
        if (++current_row == row_count)
            current_row = 0;
    }
}

K_THREAD_DEFINE(matrix_thread_id, STACKSIZE, matrix_thread,
                NULL, NULL, NULL, PRIORITY, 0, 0);

#endif // KEYPAD_GPIO_MATRIX
