#include <main.h>
#include <zephyr/sys/byteorder.h>

#if PANEL_CANBUS

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel_canbus, LOG_LEVEL_INF);

#define RX_THREAD_STACK_SIZE 512
#define TX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2
#define TX_THREAD_PRIORITY 2

#define SLEEP_TIME K_MSEC(2500)
//#define CONFIG_LOOPBACK_MODE 1
#define TEST_MSG_ID 0x446

CAN_MSGQ_DEFINE(test_msgq, 8);

K_THREAD_STACK_DEFINE(rx_thread_stack, RX_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(tx_thread_stack, TX_THREAD_STACK_SIZE);

struct k_thread rx_thread_data;
struct k_thread tx_thread_data;

const struct device *const can_dev = DEVICE_DT_GET(CANBUS_NODE);

void tx_irq_callback(const struct device *dev, int error, void *arg)
{
    char *sender = (char *)arg;

    ARG_UNUSED(dev);

    if (error != 0) {
        printf("Callback! error-code: %d, Sender: %s\n", error, sender);
    } else {
        printf("Callback okay! Sender: %s\n", sender);
    }
}

void tx_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    int ret;
    uint16_t value;
    struct can_frame frame = {0};

    printf("CAN tx thread starting\n");

    // todo: decide how we are called..
    //       - polling at given frequency?
    //       - on every key state change?
    //       - how do we signal back that (some/all) keydata has been sent?
    while(1) {

        k_sleep(SLEEP_TIME);

        frame.id = CANBUS_PANEL_STATUS;
        frame.dlc = 1;

        // todo: determine what (if anything) to do with this message id..
        //       just sending now to observe can traffic without keypad/encoders
        frame.data[0] = 0x01;  // Placeholder for firmware version

        /* This call is blocking until the message is sent (or failure timeout) */
        // ret = can_send(can_dev, &frame, K_FOREVER, NULL, NULL);
        // if (ret) {
        //     printf("can_send error: %d\n", ret);
        // } else {
        //     printf("can_send successful, frame id %x\n", frame.id);
        // }

#if PANEL_KEYPAD
        frame.id = CANBUS_PANEL_KEYPAD_1;
        frame.dlc = 8;

        value = keypad_get_value(1);
        frame.data[0] = (value >> 8) & 0xFF; // high byte
        frame.data[1] = value & 0xFF;        // low byte

        value = keypad_get_value(2);
        frame.data[2] = (value >> 8) & 0xFF; // high byte
        frame.data[3] = value & 0xFF;        // low byte

        value = keypad_get_value(3);
        frame.data[4] = (value >> 8) & 0xFF; // high byte
        frame.data[5] = value & 0xFF;        // low byte

        value = keypad_get_value(4);
        frame.data[6] = (value >> 8) & 0xFF; // high byte
        frame.data[7] = value & 0xFF;        // low byte

        /* This call is blocking until the message is sent (or failure timeout) */
        ret = can_send(can_dev, &frame, K_FOREVER, NULL, NULL);
        if (ret) {
            printf("can_send error: %d\n", ret);
        } else {
            printf("can_send successful, frame id %x\n", frame.id);
        }

        frame.id = CANBUS_PANEL_KEYPAD_2;
        frame.dlc = 2;

        value = keypad_get_value(5);
        frame.data[0] = (value >> 8) & 0xFF; // high byte
        frame.data[1] = value & 0xFF;        // low byte

        /* This call is blocking until the message is sent (or failure timeout) */
        ret = can_send(can_dev, &frame, K_FOREVER, NULL, NULL);
        if (ret) {
            printf("can_send error: %d\n", ret);
        } else {
            printf("can_send successful, frame id %x\n", frame.id);
        }

        keypad_reset_flags(); // allow queued events to be processed
#endif

#if PANEL_ENCODER
        frame.id = CANBUS_PANEL_ENCODER_1;
        frame.dlc = 8;

        value = quadrature_get_value(1);
        frame.data[0] = (value >> 8) & 0xFF; // high byte
        frame.data[1] = value & 0xFF;        // low byte

        value = quadrature_get_value(2);
        frame.data[2] = (value >> 8) & 0xFF; // high byte
        frame.data[3] = value & 0xFF;        // low byte

        value = quadrature_get_value(3);
        frame.data[4] = (value >> 8) & 0xFF; // high byte
        frame.data[5] = value & 0xFF;        // low byte

        value = quadrature_get_value(4);
        frame.data[6] = (value >> 8) & 0xFF; // high byte
        frame.data[7] = value & 0xFF;        // low byte

        /* This call is blocking until the message is sent (or failure timeout) */
        // ret = can_send(can_dev, &frame, K_FOREVER, NULL, NULL);
        // if (ret) {
        //     printf("can_send error: %d\n", ret);
        // } else {
        //     printf("can_send successful, frame id %x\n", frame.id);
        // }

        frame.id = CANBUS_PANEL_ENCODER_2;
        frame.dlc = 8;

        value = quadrature_get_value(5);
        frame.data[0] = (value >> 8) & 0xFF; // high byte
        frame.data[1] = value & 0xFF;        // low byte

        value = quadrature_get_value(6);
        frame.data[2] = (value >> 8) & 0xFF; // high byte
        frame.data[3] = value & 0xFF;        // low byte

        value = quadrature_get_value(7);
        frame.data[4] = (value >> 8) & 0xFF; // high byte
        frame.data[5] = value & 0xFF;        // low byte

        value = quadrature_get_value(8);
        frame.data[6] = (value >> 8) & 0xFF; // high byte
        frame.data[7] = value & 0xFF;        // low byte

        /* This call is blocking until the message is sent (or failure timeout) */
        // ret = can_send(can_dev, &frame, K_FOREVER, NULL, NULL);
        // if (ret) {
        //     printf("can_send error: %d\n", ret);
        // } else {
        //     printf("can_send successful, frame id %x\n", frame.id);
        // }
#endif
    }
}

void rx_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    printf("CAN rx thread starting\n");

    const struct can_filter test_filter = {
        .flags = CAN_FILTER_DATA,
        .id = 0x000,  //TEST_MSG_ID,
        .mask = 0x000 //CAN_STD_ID_MASK
    };
    struct can_frame frame;
    int filter_id;

    filter_id = can_add_rx_filter_msgq(can_dev, &test_msgq, &test_filter);
    printf("filter id: %d\n", filter_id);

    while (1) {
        k_msgq_get(&test_msgq, &frame, K_FOREVER);

        printf("Data received, frame id: %x\n", frame.id);

        // Thread syncronisation
        k_mutex_lock(&paneldata_mutex, K_FOREVER);

        switch (frame.id) {
            case CANBUS_PANEL_STATE_1:
                panel_displaydata.grbl_state    = (frame.data[2] << 8) | frame.data[3];
                panel_displaydata.spindle_speed = (frame.data[4] << 8) | frame.data[5];
                panel_displaydata.spindle_load  = (frame.data[6] << 8) | frame.data[7];
                break;

            case CANBUS_PANEL_STATE_2:
                panel_displaydata.spindle_override = frame.data[0];
                panel_displaydata.feed_override    = frame.data[1];
                panel_displaydata.rapid_override   = frame.data[2];
                panel_displaydata.wcs              = frame.data[3];
                panel_displaydata.mpg_mode         = frame.data[4];
                panel_displaydata.jog_mode         = frame.data[5];
                break;

            case CANBUS_PANEL_MPOS_1:
                panel_displaydata.x_pos.bytes[0]  = frame.data[1];
                panel_displaydata.x_pos.bytes[1]  = frame.data[0];
                panel_displaydata.x_pos.bytes[2]  = frame.data[3];
                panel_displaydata.x_pos.bytes[3]  = frame.data[2];
                panel_displaydata.y_pos.bytes[0]  = frame.data[5];
                panel_displaydata.y_pos.bytes[1]  = frame.data[4];
                panel_displaydata.y_pos.bytes[2]  = frame.data[7];
                panel_displaydata.y_pos.bytes[3]  = frame.data[6];
                break;

            case CANBUS_PANEL_MPOS_2:
                panel_displaydata.z_pos.bytes[0]  = frame.data[1];
                panel_displaydata.z_pos.bytes[1]  = frame.data[0];
                panel_displaydata.z_pos.bytes[2]  = frame.data[3];
                panel_displaydata.z_pos.bytes[3]  = frame.data[2];
                panel_displaydata.a_pos.bytes[0]  = frame.data[5];
                panel_displaydata.a_pos.bytes[1]  = frame.data[4];
                panel_displaydata.a_pos.bytes[2]  = frame.data[7];
                panel_displaydata.a_pos.bytes[3]  = frame.data[6];
                break;

            case CANBUS_PANEL_MPOS_3:
                panel_displaydata.b_pos.bytes[0]  = frame.data[1];
                panel_displaydata.b_pos.bytes[1]  = frame.data[0];
                panel_displaydata.b_pos.bytes[2]  = frame.data[3];
                panel_displaydata.b_pos.bytes[3]  = frame.data[2];
                panel_displaydata.c_pos.bytes[0]  = frame.data[5];
                panel_displaydata.c_pos.bytes[1]  = frame.data[4];
                panel_displaydata.c_pos.bytes[2]  = frame.data[7];
                panel_displaydata.c_pos.bytes[3]  = frame.data[6];
                break;

            case CANBUS_PANEL_MPOS_4:
                panel_displaydata.u_pos.bytes[0]  = frame.data[1];
                panel_displaydata.u_pos.bytes[1]  = frame.data[0];
                panel_displaydata.u_pos.bytes[2]  = frame.data[3];
                panel_displaydata.u_pos.bytes[3]  = frame.data[2];
                panel_displaydata.v_pos.bytes[0]  = frame.data[5];
                panel_displaydata.v_pos.bytes[1]  = frame.data[4];
                panel_displaydata.v_pos.bytes[2]  = frame.data[7];
                panel_displaydata.v_pos.bytes[3]  = frame.data[6];
                break;

            default:
                break;
        }

        k_mutex_unlock(&paneldata_mutex);

        printf("Data received (filter matched): %u\n",
               sys_be16_to_cpu(UNALIGNED_GET((uint16_t *)&frame.data)));
    }
}


int canbus_init(void)
{
    int ret;
    k_tid_t rx_tid, tx_tid;

    if (!device_is_ready(can_dev)) {
        printf("CAN: Device NOT ready.\n");
        return 0;
    } else {
        printf("CAN: Device %s IS ready.\n", can_dev->name);
    }

#ifdef CONFIG_LOOPBACK_MODE
    ret = can_set_mode(can_dev, CAN_MODE_LOOPBACK);
    if (ret != 0) {
        printf("Error setting CAN mode [%d]\n", ret);
        return 0;
    }
#endif
    ret = can_start(can_dev);
    if (ret != 0) {
        printf("Error starting CAN controller [%d]\n", ret);
        return 0;
    } else {
        printf("CAN controller started successfully\n");
    }

    printf("Starting tx thread...\n");
    tx_tid = k_thread_create(&tx_thread_data, tx_thread_stack,
                 K_THREAD_STACK_SIZEOF(tx_thread_stack),
                 tx_thread, NULL, NULL, NULL,
                 TX_THREAD_PRIORITY, 0, K_NO_WAIT);
    if (!tx_tid) {
        printf("ERROR spawning tx thread\n");
    }


    printf("Starting rx thread...\n");
    rx_tid = k_thread_create(&rx_thread_data, rx_thread_stack,
                 K_THREAD_STACK_SIZEOF(rx_thread_stack),
                 rx_thread, NULL, NULL, NULL,
                 RX_THREAD_PRIORITY, 0, K_NO_WAIT);
    if (!rx_tid) {
        printf("ERROR spawning rx thread\n");
    }

    return 0;
}

#endif // PANEL_CANBUS