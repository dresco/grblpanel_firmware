#include <main.h>
#include <zephyr/drivers/sensor.h>

#if PANEL_ENCODER

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel_encoder, LOG_LEVEL_DBG);

const struct device *encoders[MAX_ENCODERS];

// todo: change printf/printk to use logging

void quadrature_init(void)
{
    printf("In quadrature init()\n");

    // Populate the encoder array with devicetree entries
    encoders[0] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_1);
    encoders[1] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_2);
    encoders[2] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_3);
    encoders[3] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_4);
    encoders[4] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_5);
    encoders[5] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_6);
    encoders[6] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_7);
    encoders[7] = DEVICE_DT_GET_OR_NULL(ENCODER_NODE_8);

    for (int i = 0; i < MAX_ENCODERS; i++) {
        const struct device *encoder = encoders[i];
        if (encoder) {
            //printf("encoder %d is not null, check for ready..\n", i+1);
            if (!device_is_ready(encoder)) {
                printf("encoder %d NOT ready.\n", i+1);
            } else {
                printf("encoder %d IS ready.\n", i+1);
            }

        } else {
            //printf("encoder %d IS null, not checking for ready..\n", i+1);
        }
    }

}

uint16_t quadrature_get_value(uint8_t instance)
{
    int rc;
    struct sensor_value val;

    // note: called with instance starting at 1, but indexing into array starting at 0

    if(!encoders[instance-1]) {
        //printk("encoder %d not initialised\n", instance);
        return 0;
}

    rc = sensor_sample_fetch(encoders[instance-1]);
    if (rc != 0) {
        printk("Failed to fetch sample (%d)\n", rc);
        return 0;
    }


    rc = sensor_channel_get(encoders[instance-1], SENSOR_CHAN_ROTATION, &val);
    if (rc != 0) {
        printk("Failed to get data (%d)\n", rc);
        return 0;
    } else {
        //printk("encoder %d value: %d\n", instance, val.val1);
        return val.val1;
    }

    return 0;
}

#endif // PANEL_ENCODER