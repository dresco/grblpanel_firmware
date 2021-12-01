#include <zephyr.h>
#include <modbus.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

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

int init_modbus_server(void)
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
