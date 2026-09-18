#ifndef HW_CP_H
#define HW_CP_H

#include "hw_module.h"
#include "hw_clock.h"
#include "defs.h"
#include "config_preinception.h"

enum class CP_STATE {
    IDLE = 0,
    DECODE,

    HANDSHAKE,

    MEM_FETCH,
    MEM_FETCH_WAIT,

    MEM_WRITE_ADDR,
    MEM_WRITE_VAL_PREP,
    MEM_WRITE_VAL,
    MEM_WRITE_WAIT,

    FETCH_OP1,
    FETCH_OP1_END,
    FETCH_OP2,
    FETCH_OP2_END,

    ADD,
    MULP2,
    DIVP2,

    CMD_END,
    CMD_END_WAIT,

#if DEVICE_DEBUG
    DEBUG_WRITE_REQUEST,
    DEBUG_WRITE_RESPONSE,
    DEBUG_WRITE_WAIT
#endif

};

class HwCp : public HwModule<DevicePayload> {
public:
    HwCp(HwModule* hw_interface_module, HwClock& clk);
    
    std::string module_name() override;
    void execute(uint32_t block_idx) override;

private:
    Logger* log;
    HwModule* hw_interface_module;

    uint8_t pkt_id, pkt_type, pkt_cmd, pkt_sub_cmd;
    uint32_t pkt_body;

    uint32_t mem_fetch_addr, mem_write_addr, mem_val;
    uint32_t op1, op2;

    uint8_t tx_cmd_id = 0;

    CP_STATE state = CP_STATE::IDLE;
    CP_STATE mem_op_restore_state = CP_STATE::IDLE;
    CP_STATE wait_restore_state = CP_STATE::IDLE;

#if DEVICE_DEBUG
    uint32_t cp_cycle_count = 0;
#endif

};

#endif