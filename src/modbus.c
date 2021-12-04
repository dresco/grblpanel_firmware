#include <main.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

static uint16_t holding_reg[8];
//static uint16_t input_reg[8];

static int input_reg_rd(uint16_t addr, uint16_t *reg)
{
    // if (addr >= ARRAY_SIZE(input_reg)) {
    //     return -ENOTSUP;
    // }

    // *reg = input_reg[addr];

    LOG_INF("Input register read, addr %u", addr);

    switch(addr) {
        case IREG_DEBUG_1:
        case IREG_DEBUG_2:
            break;

        case IREG_ENCODER_1:
            *reg = quadrature_get_value(1);
            break;

        case IREG_ENCODER_2:
            *reg = quadrature_get_value(2);
            break;

        case 104:
        case 105:
            break;

        case IREG_KEYPAD_1:
            *reg = keypad_get_value(1);
            break;

        case IREG_KEYPAD_2:
            *reg = keypad_get_value(2);
            break;

        case 108:
        case 109:
        case 110:
        case 111:
        case 112:
        case 113:
        case 114:
        case 115:
            break;

        default:
            LOG_ERR("Input register read, unssupported addr %u", addr);
            return -ENOTSUP;
    }

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
    // if (addr >= ARRAY_SIZE(holding_reg)) {
    //     return -ENOTSUP;
    // }

    // holding_reg[addr] = reg;

    LOG_INF("Holding register write, addr %u, value %u", addr, reg);

    switch(addr) {
        case HREG_GRBL_STATE:
            panel_displaydata.grbl_state = reg;
            break;

        case 101:
            break;

        case HREG_SPINDLE_SPEED:
            panel_displaydata.spindle_speed = reg;
            break;

        case HREG_SPINDLE_POWER:
            panel_displaydata.spindle_power = reg;
            break;

        case 104:
            break;

        // todo: figure out byte ordering...
        case HREG_XPOS_LO:
            panel_displaydata.x_pos.bytes[0] = reg & 0xFF; //low byte
            panel_displaydata.x_pos.bytes[1] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_XPOS_HI:
            panel_displaydata.x_pos.bytes[2] = reg & 0xFF; //low byte
            panel_displaydata.x_pos.bytes[3] = (reg >> 8) & 0xFF; //high byte

            LOG_INF ("x_pos float: %f", panel_displaydata.x_pos.value);
            LOG_INF ("x_pos array: %hhX:%hhX:%hhX:%hhX", panel_displaydata.x_pos.bytes[0], panel_displaydata.x_pos.bytes[1],
                                                         panel_displaydata.x_pos.bytes[2], panel_displaydata.x_pos.bytes[3]);
            //LOG_INF ("x_pos %u", panel_displaydata.x_pos.bytes[3]);
            break;

        case 107:
        case 108:
        case 109:
        case 110:
            break;

        default:
            return -ENOTSUP;
    }

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
        .unit_id = UNIT_ID,
    },
    .serial = {
        .baud = BAUD_RATE,
        .parity = UART_CFG_PARITY_NONE,
    },
};

int modbus_init(void)
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
