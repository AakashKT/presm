#include "hw_cp.h"

HwCp::HwCp(HwModule* hw_interface_module)
    : HwModule<DevicePayload>(1),
      hw_interface_module(hw_interface_module)
{
    this->log = new Logger();
    this->log->init("hw_cp");

    this->log->log_info("[HwCp] Constructor called");
}

std::string HwCp::module_name()
{
    return "HW_MODULE_CP";
}

void HwCp::execute(uint32_t block_idx)
{
#if DEVICE_DEBUG
    this->cp_cycle_count += 1;
#endif

    if(this->state == CP_STATE::IDLE) {
        auto msg = this->get_message(PKT_TO_CP);
        if(msg.data != nullptr && msg.ack == false) {
            auto payload = (DevicePayload*) msg.data.get();

            this->pkt_id = payload->id();
            this->pkt_type = payload->type();
            this->pkt_cmd = payload->cmd();
            this->pkt_sub_cmd = payload->sub_cmd();
            this->pkt_body = payload->fields32.body;

#if DEVICE_DEBUG
            this->cp_cycle_count = 0;
#endif

            this->state = CP_STATE::DECODE;
        }
        else {
            this->state = CP_STATE::IDLE;
        }
    }

    else if(this->state == CP_STATE::DECODE) {
        if(this->pkt_cmd == 1)
            this->state = CP_STATE::HANDSHAKE;
        else if(this->pkt_cmd == 2) {
            if(this->pkt_sub_cmd == 0)
                this->state = CP_STATE::FETCH_OP1;
            else if(this->pkt_sub_cmd == 1)
                this->state = CP_STATE::FETCH_OP2;
            else if(this->pkt_sub_cmd == 2)
                this->state = CP_STATE::ADD;
        }
        else if(this->pkt_cmd == 3) {
            if(this->pkt_sub_cmd == 0)
                this->state = CP_STATE::FETCH_OP1;
            else if(this->pkt_sub_cmd == 1)
                this->state = CP_STATE::FETCH_OP2;
            else if(this->pkt_sub_cmd == 2)
                this->state = CP_STATE::MULP2;
        }
        else if(this->pkt_cmd == 4) {
            if(this->pkt_sub_cmd == 0)
                this->state = CP_STATE::FETCH_OP1;
            else if(this->pkt_sub_cmd == 1)
                this->state = CP_STATE::FETCH_OP2;
            else if(this->pkt_sub_cmd == 2)
                this->state = CP_STATE::DIVP2;
        }
    }

    else if(this->state == CP_STATE::HANDSHAKE) {
        this->log->log_info("[HwCp] Handshake Requested");

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->pkt_id);
        tx->type((uint32_t)TYPE::RESPONSE);
        tx->cmd(this->pkt_cmd);
        tx->sub_cmd(this->pkt_sub_cmd);
        tx->fields.body_1 = 2;
        tx->fields.body_2 = 1;
        tx->fields.body_3 = 0;
        tx->fields.body_4 = 0;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->state = CP_STATE::IDLE;
    }

#if DEVICE_DEBUG
    else if(this->state == CP_STATE::DEBUG_WRITE_REQUEST) {
        this->log->log_info("[HwCp] Writing debug payload (request).");

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->pkt_id);
        tx->type(0);
        tx->cmd(0);
        tx->sub_cmd(15);
        tx->fields32.body = this->cp_cycle_count;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->cp_cycle_count = 0;

        this->state = CP_STATE::DEBUG_WRITE_WAIT;
    }
    else if(this->state == CP_STATE::DEBUG_WRITE_RESPONSE) {
        this->log->log_info("[HwCp] Writing debug payload (response).");

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->pkt_id);
        tx->type(1);
        tx->cmd(0);
        tx->sub_cmd(15);
        tx->fields32.body = this->cp_cycle_count;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->cp_cycle_count = 0;

        this->state = CP_STATE::DEBUG_WRITE_WAIT;
    }
    else if(this->state == CP_STATE::DEBUG_WRITE_WAIT) {
        if(this->hw_interface_module->get_message_ack(PKT_FROM_CP)) {
            this->state = this->wait_restore_state;
        }
    }
#endif

    else if(this->state == CP_STATE::MEM_FETCH) {
        this->log->log_info("[HwCp] Sending MEM_FETCH request");

        this->tx_cmd_id = (this->tx_cmd_id + 1) % 16;

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->tx_cmd_id);
        tx->type((uint32_t)TYPE::REQUEST);
        tx->cmd(0);
        tx->sub_cmd(0);
        tx->fields32.body = this->mem_fetch_addr;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->state = CP_STATE::MEM_FETCH_WAIT;
    }
    else if(this->state == CP_STATE::MEM_FETCH_WAIT) {
        auto msg = this->get_message(PKT_TO_CP);
        if(msg.data != nullptr && msg.ack == false) {
            auto payload = (DevicePayload*) msg.data.get();

            if(payload->id() == this->tx_cmd_id && payload->type() == 1u && payload->cmd() == 0u && payload->sub_cmd() == 0u) {
                this->mem_val = payload->fields32.body;

#if DEVICE_DEBUG
                this->wait_restore_state = this->mem_op_restore_state;
                this->state = CP_STATE::DEBUG_WRITE_RESPONSE;
#else
                this->state = this->mem_op_restore_state;
#endif

                this->log->log_info("[HwCp] Received MEM_FETCH response -> Value is " + std::bitset<32>(this->mem_val).to_string());
            }
        }
    }

    else if(this->state == CP_STATE::MEM_WRITE_ADDR) {
        this->log->log_info("[HwCp] Sending MEM_WRITE request (1)");

        this->tx_cmd_id = (this->tx_cmd_id + 1) % 16;

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->tx_cmd_id);
        tx->type((uint32_t)TYPE::REQUEST);
        tx->cmd(0);
        tx->sub_cmd(1);
        tx->fields32.body = this->mem_write_addr;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->state = CP_STATE::MEM_WRITE_VAL_PREP;
    }
    else if(this->state == CP_STATE::MEM_WRITE_VAL_PREP) {
        if(this->hw_interface_module->get_message_ack(PKT_FROM_CP)) {
            this->state = CP_STATE::MEM_WRITE_VAL;
        }
    }
    else if(this->state == CP_STATE::MEM_WRITE_VAL) {
        this->log->log_info("[HwCp] Sending MEM_WRITE request (2)");

        this->tx_cmd_id = (this->tx_cmd_id + 1) % 16;

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->tx_cmd_id);
        tx->type((uint32_t)TYPE::REQUEST);
        tx->cmd(0);
        tx->sub_cmd(1);
        tx->fields32.body = this->mem_val;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->state = CP_STATE::MEM_WRITE_WAIT;
    }
    else if(this->state == CP_STATE::MEM_WRITE_WAIT) {
        auto msg = this->get_message(PKT_TO_CP);
        if(msg.data != nullptr && msg.ack == false) {
            auto payload = (DevicePayload*) msg.data.get();

            if(payload->id() == this->tx_cmd_id && payload->type() == 1u && payload->cmd() == 0u && payload->sub_cmd() == 1u) {
                this->log->log_info("[HwCp] Received MEM_WRITE response");

#if DEVICE_DEBUG
                this->wait_restore_state = this->mem_op_restore_state;
                this->state = CP_STATE::DEBUG_WRITE_RESPONSE;
#else
                this->state = this->mem_op_restore_state;
#endif

            }
        }
    }

    else if(this->state == CP_STATE::FETCH_OP1) {
        this->log->log_info("[HwCp] Operand 1 fetch request");

        this->mem_fetch_addr = this->pkt_body;

        this->mem_op_restore_state = CP_STATE::FETCH_OP1_END;
        this->state = CP_STATE::MEM_FETCH;
    }
    else if(this->state == CP_STATE::FETCH_OP1_END) {
        this->op1 = this->mem_val;
        this->state = CP_STATE::CMD_END;
    }

    else if(this->state == CP_STATE::FETCH_OP2) {
        this->log->log_info("[HwCp] Operand 2 fetch request");

        this->mem_fetch_addr = this->pkt_body;

        this->mem_op_restore_state = CP_STATE::FETCH_OP2_END;
        this->state = CP_STATE::MEM_FETCH;
    }
    else if(this->state == CP_STATE::FETCH_OP2_END) {
        this->op2 = this->mem_val;
        this->state = CP_STATE::CMD_END;
    }

    else if(this->state == CP_STATE::ADD) {
        this->log->log_info("[HwCp] ADD command execute request");

        this->mem_write_addr = this->pkt_body;
        this->mem_val = std::bit_cast<uint32_t>(std::bit_cast<int>(this->op1)
                                                     + std::bit_cast<int>(this->op2));
        
        this->log->log_info("[HwCp] ADD -> " + std::bitset<32>(this->op1).to_string() + " + "  + std::bitset<32>(this->op2).to_string() + " = "  + std::bitset<32>(this->mem_val).to_string());
        
        this->mem_op_restore_state = CP_STATE::CMD_END;
        this->state = CP_STATE::MEM_WRITE_ADDR;
    }
    else if(this->state == CP_STATE::MULP2) {
        this->log->log_info("[HwCp] MULP2 command execute request");

        this->mem_write_addr = this->pkt_body;
        this->mem_val = std::bit_cast<uint32_t>(std::bit_cast<int>(this->op1) << this->op2);
        
        this->log->log_info("[HwCp] MULP2 -> " + std::bitset<32>(this->op1).to_string() + " * 2^"  + std::bitset<32>(this->op2).to_string() + " = "  + std::bitset<32>(this->mem_val).to_string());
        
        this->mem_op_restore_state = CP_STATE::CMD_END;
        this->state = CP_STATE::MEM_WRITE_ADDR;
    }
    else if(this->state == CP_STATE::DIVP2) {
        this->log->log_info("[HwCp] DIVp2 command execute request");

        this->mem_write_addr = this->pkt_body;
        this->mem_val = std::bit_cast<uint32_t>(std::bit_cast<int>(this->op1) >> this->op2);
        
        this->log->log_info("[HwCp] DIVP2 -> " + std::bitset<32>(this->op1).to_string() + " / 2^"  + std::bitset<32>(this->op2).to_string() + " = "  + std::bitset<32>(this->mem_val).to_string());
        
        this->mem_op_restore_state = CP_STATE::CMD_END;
        this->state = CP_STATE::MEM_WRITE_ADDR;
    }

    else if(this->state == CP_STATE::CMD_END) {
        this->log->log_info("[HwCp] Sending CMD_END response");

        auto tx = std::make_shared<DevicePayload>();
        tx->id(this->pkt_id);
        tx->type((uint32_t)TYPE::RESPONSE);
        tx->cmd(this->pkt_cmd);
        tx->sub_cmd(this->pkt_sub_cmd);
        tx->fields32.body = 0;
        this->hw_interface_module->push_message(PKT_FROM_CP, tx);

        this->state = CP_STATE::CMD_END_WAIT;
    }
    else if(this->state == CP_STATE::CMD_END_WAIT) {
        if(this->hw_interface_module->get_message_ack(PKT_FROM_CP)) {

#if DEVICE_DEBUG
            this->wait_restore_state = CP_STATE::IDLE;
            this->state = CP_STATE::DEBUG_WRITE_REQUEST;
#else
            this->state = CP_STATE::IDLE;
#endif

        }
    }

    else {
        this->log->log_error_and_exit("[HwCp] Unknown CP_STATE");
    }
}