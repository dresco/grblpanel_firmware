#include <zephyr.h>
#include <adp5589.h>
#include <quadrature.h>
#include <modbus/modbus.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(panel, LOG_LEVEL_INF);

static uint16_t holding_reg[8];
static uint16_t input_reg[8];

static int input_reg_rd(uint16_t addr, uint16_t *reg)
{
    if (addr >= ARRAY_SIZE(input_reg)) {
        return -ENOTSUP;
    }

    *reg = input_reg[addr];

    LOG_INF("Input register read, addr %u", addr);

    return 0;
}

static int holding_reg_rd(uint16_t addr, uint16_t *reg)
{
    if (addr >= ARRAY_SIZE(holding_reg)) {
        return -ENOTSUP;
    }

    *reg = holding_reg[addr];

    LOG_INF("Holding register read, addr %u", addr);

    return 0;
}

static int holding_reg_wr(uint16_t addr, uint16_t reg)
{
    if (addr >= ARRAY_SIZE(holding_reg)) {
        return -ENOTSUP;
    }

    holding_reg[addr] = reg;

    LOG_INF("Holding register write, addr %u", addr);

    return 0;
}

static struct modbus_user_callbacks mbs_cbs = {
    .input_reg_rd   = input_reg_rd,
    .holding_reg_rd = holding_reg_rd,
    .holding_reg_wr = holding_reg_wr
};

const static struct modbus_iface_param server_param = {
    .mode = MODBUS_MODE_RTU,
    .server = {
        .user_cb = &mbs_cbs,
        .unit_id = 10,
    },
    .serial = {
        .baud = 38400,
        .parity = UART_CFG_PARITY_NONE,
    },
};

static int init_modbus_server(void)
{
    const char iface_name[] = {DT_PROP(DT_INST(0, zephyr_modbus_serial), label)};
    int iface;

    iface = modbus_iface_get_by_name(iface_name);

    if (iface < 0) {
        LOG_ERR("Failed to get iface index for %s", iface_name);
        return iface;
    }

    return modbus_init_server(iface, server_param);
}

void main(void)
{
    const struct device *i2c_dev;
    static uint8_t key_data[16]  = {0};
    static uint32_t prev_encoder;

    LOG_INF("Hello World...! %s", CONFIG_BOARD);

    if (adp5589_init(&i2c_dev) != 0) {
        LOG_ERR("Error initialising ADP5589 device.");
    } else {
        LOG_INF("ADP5589 initialised successfully.");
    }

    quadrature_init();

    if (init_modbus_server()) {
        LOG_ERR("Modbus RTU server initialization failed");
    }

    while (1) {

        //
        // Keypad testing -- todo: abstract more of this away into wrapper functions
        //
        uint8_t event_count =  adp5589_get_event_count(i2c_dev);

        if (event_count) {
            //LOG_INF("Event count %u, reading data..", event_count);

            adp5589_get_register_values(i2c_dev, ADP5589_ADR_FIFO10, key_data, event_count);
            for (int i = 0; i < event_count; i++) {
                if (key_data[i] & 0x80)
                    LOG_INF("Key 0x%02X pressed..", (key_data[i] & 0x7F));
                else
                    LOG_INF("Key 0x%02X released..", (key_data[i] & 0x7F));
            }
        }

        //
        // Encoder testing
        //
        uint32_t encoder = quadrature_get_value();
        if (encoder != prev_encoder)
            LOG_INF("Encoder value is: %d", encoder);
        prev_encoder = encoder;

        k_msleep(100);

    }
}
