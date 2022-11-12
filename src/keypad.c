#include <main.h>
#include <keypad.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

static uint8_t keydata_flags[11]; // bitmap of keycodes (1-88) that have already been processed

void keypad_reset_flags(void) {
    // todo: synchronisation
    memset(keydata_flags, 0, ARRAY_SIZE(keydata_flags));
}

int keypad_init(void)
{
#ifdef KEYPAD_GPIO_MATRIX
    // Initialise the GPIO keypad matrix
    if (matrix_init() != 0) {
        LOG_ERR("Error initialising GPIO keypad matrix.");
        return 1;
    } else {
        LOG_INF("GPIO keypad matrix initialised successfully.");
        return 0;
    }
#else
    // Initialise the ADP5589 I2C IO expander
    if (adp5589_init() != 0) {
        LOG_ERR("Error initialising ADP5589 device.");
        return 1;
    } else {
        LOG_INF("ADP5589 initialised successfully.");
        return 0;
    }
#endif
}

int keypad_get_event_count(void)
{
#ifdef KEYPAD_GPIO_MATRIX
    return matrix_get_event_count();
#else
    return adp5589_get_event_count();
#endif
}

int keypad_get_events(uint8_t *key_data, uint8_t event_count)
{
#ifdef KEYPAD_GPIO_MATRIX
    return matrix_get_events(key_data, event_count);
#else
    LOG_DBG("adp5589_get_register_values(): requesting %i entries..", event_count);
    return adp5589_get_register_values(ADP5589_ADR_FIFO10, key_data, event_count);
#endif
}

uint16_t keypad_get_value(uint8_t instance)
{
    // todo: add thread syncronisation?
    switch (instance) {
        case 1:
            return panel_keydata_1.value;

        case 2:
            return panel_keydata_2.value;

        case 3:
            return panel_keydata_3.value;

        case 4:
            return panel_keydata_4.value;

        default:
            return 0;
    }
}

int keypad_process_events(void)
{
    // todo: the keydata registers are polled over modbus, this interval is quite slow, and we can easily miss a keypress
    // if for instance a key is pressed and released before the next poll. We need to put only the first instance of a state
    // change (for any specific key) into the register data, and queue any subsequent states for further polling iterations
    // (we can't ignore the subsequent states or else the modbus master would not see the release event).

    uint8_t new_key_count, total_key_count = 0;

    // working buffer
    uint8_t working_key_data[KEY_BUFFER_SIZE] = {0};

    // key events already waiting to be sent
    static uint8_t queued_key_count = 0;
    static uint8_t queued_key_data[KEY_BUFFER_SIZE] = {0};

    // key events that can't be sent this time are placed here
    uint8_t deferred_key_count = 0;
    uint8_t deferred_key_data[KEY_BUFFER_SIZE] = {0};

    // process any queued key data first, copy into the working buffer
    if (queued_key_count) {
        LOG_DBG("Queued event count %u..", queued_key_count);
        memcpy(working_key_data, queued_key_data, queued_key_count);
    }

    // are new key events waiting to be retrieved?
    new_key_count = keypad_get_event_count();

    if (new_key_count) {
        LOG_DBG("%u new keypad events waiting..", new_key_count);
        // don't try and get any more data if already full of queued key events
        if (queued_key_count < KEY_BUFFER_SIZE) {

            // read new events into the working_key_data array, starting after any queued events, and taking care not to overflow
            int rc = keypad_get_events(&working_key_data[0]+queued_key_count, MIN(new_key_count, KEY_BUFFER_SIZE-queued_key_count));
            LOG_DBG("Got %u keypad events..", rc);

            // new keys only
            for (int i = queued_key_count; i < new_key_count; i++) {
                if (working_key_data[i] & 0x80)
                    LOG_INF("Key %02i pressed..", (working_key_data[i] & 0x7F));
                else
                    LOG_INF("Key %02i released..", (working_key_data[i] & 0x7F));
            }
        } else {
            LOG_DBG("Queued event buffer is full, no new data read..");
            new_key_count = 0;
        }
    } 

    total_key_count = new_key_count + queued_key_count;

    if (total_key_count) {
        uint8_t * pPending = &deferred_key_data[0];

        // Thread syncronisation
        k_mutex_lock(&paneldata_mutex, K_FOREVER);

        // insert key states into the global registers that are sent over modbus
        for (int i = 0; i < total_key_count; i++) {

            uint8_t keydata = working_key_data[i];
            uint8_t key_code = KEY_CODE(keydata);

            if (test_bit(keydata_flags, key_code) == 0) {
                // state has not already been updated for this key code, so update the modbus register
                LOG_DBG("first event for key 0x%02X, update modbus registers", key_code);
                set_bit(keydata_flags, key_code);

                switch (KEY_CODE(keydata)) {
                    case KEY_STOP:
                        panel_keydata_1.stop = KEY_PRESSED(keydata);
                        break;

                    case KEY_RESET1:
                    case KEY_RESET2:
                        panel_keydata_1.reset = KEY_PRESSED(keydata);
                        break;

                    case KEY_FEEDHOLD:
                        panel_keydata_1.feed_hold = KEY_PRESSED(keydata);
                        break;

                    case KEY_CYCLESTART:
                        panel_keydata_1.cycle_start = KEY_PRESSED(keydata);
                        break;

                    case KEY_UNLOCK:
                        panel_keydata_1.unlock = KEY_PRESSED(keydata);
                        break;

                    case KEY_HOME:
                        panel_keydata_1.home = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_STEP_X1:
                        panel_keydata_3.jog_step_x1 = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_STEP_X10:
                        panel_keydata_3.jog_step_x10 = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_STEP_X100:
                        panel_keydata_3.jog_step_x100 = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_STEP_SMOOTH:
                        panel_keydata_3.jog_step_smooth = KEY_PRESSED(keydata);
                        break;

                    case KEY_MPG_AXIS_X:
                        panel_keydata_1.mpg_axis_x = KEY_PRESSED(keydata);
                        break;

                    case KEY_MPG_AXIS_Y:
                        panel_keydata_1.mpg_axis_y = KEY_PRESSED(keydata);
                        break;

                    case KEY_MPG_AXIS_Z:
                        panel_keydata_1.mpg_axis_z = KEY_PRESSED(keydata);
                        break;

                    case KEY_MPG_AXIS_A:
                        panel_keydata_1.mpg_axis_a = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_X_P:
                        panel_keydata_3.jog_positive_x = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_X_N:
                        panel_keydata_3.jog_negative_x = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_Y_P:
                        panel_keydata_3.jog_positive_y = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_Y_N:
                        panel_keydata_3.jog_negative_y = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_Z_P:
                        panel_keydata_3.jog_positive_z = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_Z_N:
                        panel_keydata_3.jog_negative_z = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_A_P:
                        panel_keydata_3.jog_positive_a = KEY_PRESSED(keydata);
                        break;

                    case KEY_JOG_A_N:
                        panel_keydata_3.jog_negative_a = KEY_PRESSED(keydata);
                        break;

                    case KEY_WCS_G54:
                        panel_keydata_2.wcs_g54 = KEY_PRESSED(keydata);
                        break;

                    case KEY_WCS_G55:
                        panel_keydata_2.wcs_g55 = KEY_PRESSED(keydata);
                        break;

                    case KEY_WCS_G56:
                        panel_keydata_2.wcs_g56 = KEY_PRESSED(keydata);
                        break;

                    case KEY_WCS_G57:
                        panel_keydata_2.wcs_g57 = KEY_PRESSED(keydata);
                        break;

                    case KEY_SET_ZERO_X:
                        panel_keydata_2.zero_work_offset_x = KEY_PRESSED(keydata);
                        break;

                    case KEY_SET_ZERO_Y:
                        panel_keydata_2.zero_work_offset_y = KEY_PRESSED(keydata);
                        break;

                    case KEY_SET_ZERO_Z:
                        panel_keydata_2.zero_work_offset_z = KEY_PRESSED(keydata);
                        break;

                    case KEY_SET_ZERO_A:
                        panel_keydata_2.zero_work_offset_a = KEY_PRESSED(keydata);
                        break;

                    case KEY_GOTO_ZERO_X:
                        panel_keydata_2.move_to_zero_x = KEY_PRESSED(keydata);
                        break;

                    case KEY_GOTO_ZERO_Y:
                        panel_keydata_2.move_to_zero_y = KEY_PRESSED(keydata);
                        break;

                    case KEY_GOTO_ZERO_Z:
                        panel_keydata_2.move_to_zero_z = KEY_PRESSED(keydata);
                        break;

                    case KEY_GOTO_ZERO_A:
                        panel_keydata_2.move_to_zero_a = KEY_PRESSED(keydata);
                        break;

                    case KEY_FUNCTION_F1:
                        panel_keydata_1.function_f1 = KEY_PRESSED(keydata);
                        break;

                    case KEY_FUNCTION_F2:
                        panel_keydata_1.function_f2 = KEY_PRESSED(keydata);
                        break;

                    case KEY_FUNCTION_F3:
                        panel_keydata_1.function_f3 = KEY_PRESSED(keydata);
                        break;

                    case KEY_FUNCTION_F4:
                        panel_keydata_1.function_f4 = KEY_PRESSED(keydata);
                        break;

                    case KEY_FEED_RESET:
                        panel_keydata_4.feed_override_reset = KEY_PRESSED(keydata);
                        break;

                    case KEY_SPINDLE_RESET:
                        panel_keydata_4.spindle_override_reset = KEY_PRESSED(keydata);
                        break;

                    case KEY_RAPID_RESET:
                        panel_keydata_4.rapid_override_100 = KEY_PRESSED(keydata);
                        break;

                    default:
                        break;
                }

            } else {
                // already had an event for this keycode (could be real key press or unsent event from the queue), so save it for later..

                if (deferred_key_count < KEY_BUFFER_SIZE) {
                    LOG_DBG("subsequent event for key 0x%02X, updating deferred buffer, position %i", key_code, (pPending - &deferred_key_data[0]));
                    *pPending++ = keydata;
                    deferred_key_count = pPending - &deferred_key_data[0];
                    //LOG_INF("pending key count: %i", deferred_key_count);
                } else {
                    LOG_DBG("deferred key buffer is full, event not added..");
                }
            }

        }

        k_mutex_unlock(&paneldata_mutex);

        // calc the pending key count, and copy pending events (& count) to queued for next iteration
        queued_key_count = deferred_key_count;
        memcpy(queued_key_data, deferred_key_data, queued_key_count);
    }
    return total_key_count;
}
