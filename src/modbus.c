#include <main.h>

#if PANEL_MODBUS

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(panel, LOG_LEVEL_INF);

static uint16_t holding_reg[8];
//static uint16_t input_reg[8];

static int input_reg_rd(uint16_t addr, uint16_t *reg)
{
    // if (addr >= ARRAY_SIZE(input_reg)) {
    //     return -ENOTSUP;
    // }

    // *reg = input_reg[addr];

    // LOG_INF("Input register read, addr %u", addr);

    switch(addr) {
        case IREG_DEBUG_1:
        case IREG_DEBUG_2:
            break;

#if PANEL_ENCODER
        case IREG_ENCODER_1:
            *reg = quadrature_get_value(1);
            break;

        case IREG_ENCODER_2:
            *reg = quadrature_get_value(2);
            break;

        case IREG_ENCODER_3:
            *reg = quadrature_get_value(3);
            break;

        case IREG_ENCODER_4:
            *reg = quadrature_get_value(4);
            break;
#endif // PANEL_ENCODER

#if PANEL_KEYPAD
        case IREG_KEYPAD_1:
            *reg = keypad_get_value(1);
            break;

        case IREG_KEYPAD_2:
            *reg = keypad_get_value(2);
            break;

        case IREG_KEYPAD_3:
            *reg = keypad_get_value(3);
            break;

        case IREG_KEYPAD_4:
            *reg = keypad_get_value(4);
            break;
#endif // PANEL_KEYPAD

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

#if PANEL_KEYPAD
    keypad_reset_flags(); // allow queued events to be processed
#endif

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

    // LOG_INF("Holding register write, addr %u, value %u", addr, reg);

    // Thread syncronisation
    k_mutex_lock(&paneldata_mutex, K_FOREVER);

    switch(addr) {
        case HREG_GRBL_STATE:
            panel_displaydata.grbl_state = reg;
            break;

        case 101:
            break;

        case HREG_SPINDLE_SPEED:
            panel_displaydata.spindle_speed = reg;
            break;

        case HREG_SPINDLE_LOAD:
            panel_displaydata.spindle_load = reg;
            break;

        case HREG_OVERRIDES_1:
            panel_displaydata.spindle_override = reg & 0xFF; // low byte
            panel_displaydata.wcs = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_OVERRIDES_2:
            panel_displaydata.feed_override = reg & 0xFF; //low byte
            panel_displaydata.rapid_override = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_MPG_MODE:
            panel_displaydata.jog_mode= reg & 0xFF; // low byte
            panel_displaydata.mpg_mode = (reg >> 8) & 0xFF; //high byte
            break;

        // todo: figure out byte ordering...
        case HREG_XPOS_LO:
            panel_displaydata.x_pos.bytes[0] = reg & 0xFF; //low byte
            panel_displaydata.x_pos.bytes[1] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_XPOS_HI:
            panel_displaydata.x_pos.bytes[2] = reg & 0xFF; //low byte
            panel_displaydata.x_pos.bytes[3] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_YPOS_LO:
            panel_displaydata.y_pos.bytes[0] = reg & 0xFF; //low byte
            panel_displaydata.y_pos.bytes[1] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_YPOS_HI:
            panel_displaydata.y_pos.bytes[2] = reg & 0xFF; //low byte
            panel_displaydata.y_pos.bytes[3] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_ZPOS_LO:
            panel_displaydata.z_pos.bytes[0] = reg & 0xFF; //low byte
            panel_displaydata.z_pos.bytes[1] = (reg >> 8) & 0xFF; //high byte
            break;

        case HREG_ZPOS_HI:
            panel_displaydata.z_pos.bytes[2] = reg & 0xFF; //low byte
            panel_displaydata.z_pos.bytes[3] = (reg >> 8) & 0xFF; //high byte
            break;

        default:
            k_mutex_unlock(&paneldata_mutex);
            return -ENOTSUP;
    }

    k_mutex_unlock(&paneldata_mutex);
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
#if (DT_NODE_EXISTS(MODBUS_NODE))

    const char iface_name[] = {DEVICE_DT_NAME(MODBUS_NODE)};
    int iface;

    iface = modbus_iface_get_by_name(iface_name);

    if (iface < 0) {
        LOG_ERR("Failed to get iface index for %s", iface_name);
        return iface;
    }

    return modbus_init_server(iface, server_param);

#else
    LOG_ERR("Modbus node does not exist in device tree");
    return 0;
#endif
}

#endif // PANEL_MODBUS
