#include "functional_impl.h"

Device* get_device()
{
    return new FunctionalImpl();
}

FunctionalImpl::FunctionalImpl()
    : Device(),
      hw_clock("master"),
      hw_interface(hw_clock)
{
    this->log->log_info("[FunctionalImpl] 'Preinception Device' constructor called");
    this->device_memory = new HostResidentMemory(HOST_RESIDENT_MEM_SIZE);

    this->device_receive_thread = std::thread(
        [&](FunctionalImpl* impl) {
            while(true) {
                auto msg = impl->hw_interface.get_message_from_device();

                if(msg.data != nullptr && msg.ack == false) {
                    DevicePayload* pld = (DevicePayload*) msg.data.get();

                    if(pld->cmd() == 0)
                        impl->process_device_request(*pld);
                    else
                        impl->received_payloads.push_back(*pld);
                }
            }
        },
        this
    );

#if DEVICE_DEBUG
    this->stats_log = new Logger();
    this->stats_log->init("device_cp_stats", true);
    this->stats_log->log_plain("PKT_ID,INSTR,CLK_CYCLES,DERIVED_RUNTIME_MSEC");
#endif

}

FunctionalImpl::~FunctionalImpl()
{
    this->device_receive_thread.detach();

#if DEVICE_DEBUG
    delete this->stats_log;
#endif
}

void FunctionalImpl::device_initialize()
{
    bool found = false;

    DevicePayload tx;
    tx.id(1);
    tx.type((uint32_t)TYPE::REQUEST);
    tx.cmd((uint32_t)CMD::HANDSHAKE);
    tx.sub_cmd((uint32_t)HANDSHAKE::OP);
    tx.fields32.body = 0;
    this->send_device_payload(&tx);

#if DEVICE_DEBUG
        this->last_request_pkt.pop_front();
#endif

    auto begin_time = std::chrono::high_resolution_clock::now();
    DevicePayload rx;
    while(true) {
        if(this->receive_device_payload(&rx)) {
            if(rx.id() == 1 && rx.type() == static_cast<uint32_t>(TYPE::RESPONSE)
                && rx.fields.body_1 == 2 && rx.fields.body_2 == 1) {
                this->log->log_info("[FunctionalImpl] Found device.");
                found = true;
            }
            else {
                this->log->log_info("[FunctionalImpl] Received packet does not match handshake signature");
                this->log->log_info(rx.print());
            }
        }

        auto time_diff = std::chrono::high_resolution_clock::now() - begin_time;
        if(found || std::chrono::duration_cast<std::chrono::microseconds>(time_diff).count() >= 2e6)
            break;
    }

    if(!found)
        this->log->log_error_and_exit("[FunctionalImpl] Could not find device.");
}

uint32_t FunctionalImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

void FunctionalImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}

char* FunctionalImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void FunctionalImpl::send_device_payload(void* payload)
{
    auto payload_ = std::make_shared<DevicePayload>();
    payload_->copy(*((DevicePayload*)payload));

    this->hw_interface.push_message(PKT_TO_DEVICE, payload_);

    this->log->log_info("[FunctionalImpl] Sent device payload ->");
    this->log->log_info(payload_->print());

#if DEVICE_DEBUG
    if(payload_->type() == (uint32_t)TYPE::REQUEST)
        this->last_request_pkt.push_back(*payload_);
    else if(payload_->type() == (uint32_t)TYPE::RESPONSE)
        this->last_response_pkt.push_back(*payload_);
#endif
}

void FunctionalImpl::process_device_request(DevicePayload& payload)
{
    if(payload.sub_cmd() == 0) {
        this->log->log_info("[FunctionalImpl] Device requested read, payload ->");
        this->log->log_info(payload.print());

        char* mem_val = this->read_from_device_memory(payload.fields32.body, 4);

        DevicePayload mem_response;
        mem_response.id(payload.id());
        mem_response.type((uint32_t)TYPE::RESPONSE);
        mem_response.cmd(payload.cmd());
        mem_response.sub_cmd(payload.sub_cmd());
        mem_response.fields.body_1 = mem_val[0];
        mem_response.fields.body_2 = mem_val[1];
        mem_response.fields.body_3 = mem_val[2];
        mem_response.fields.body_4 = mem_val[3];
        this->send_device_payload(&mem_response);
    }
    else if(payload.sub_cmd() == 1) {
        if(this->mem_write_state == ADDR_RECV) {
            this->log->log_info("[FunctionalImpl] Device requested write to address, payload ->");
            this->log->log_info(payload.print());

            this->mem_write_addr_scratch = payload.fields32.body;
            this->mem_write_state = VAL_RECV;
        }
        else if(this->mem_write_state == VAL_RECV) {
            this->log->log_info("[FunctionalImpl] Device requested write value to above address, payload ->");
            this->log->log_info(payload.print());

            char data[4] = { payload.fields.body_1, payload.fields.body_2, payload.fields.body_3, payload.fields.body_4 };
            this->write_to_device_memory(this->mem_write_addr_scratch, 4, data);

            DevicePayload mem_response;
            mem_response.id(payload.id());
            mem_response.type((uint32_t)TYPE::RESPONSE);
            mem_response.cmd(payload.cmd());
            mem_response.sub_cmd(payload.sub_cmd());
            mem_response.fields32.body = 0;
            this->send_device_payload(&mem_response);

            this->mem_write_state = ADDR_RECV;
        }
    }

#if DEVICE_DEBUG
    else if(payload.sub_cmd() == 15) {
        std::string cmd = "UNKNOWN";
        uint32_t clk_count = payload.fields32.body;
        
        if(payload.type() == (uint32_t)TYPE::REQUEST) {
            DevicePayload last_pkt = *this->last_request_pkt.pop_front();

            uint32_t num_pkts = 1;
            float sec_per_payload = sizeof(DevicePayload) * 8 / float(BAUD_RATE);
            clk_count += uint32_t(sec_per_payload * num_pkts * DEVICE_CLK_HZ);
            
            if(last_pkt.cmd() == (uint32_t)CMD::ADD)
                cmd = "ADD_ACK";
            else if(last_pkt.cmd() == (uint32_t)CMD::MULP2)
                cmd = "MULP2_ACK";
            else if(last_pkt.cmd() == (uint32_t)CMD::DIVP2)
                cmd = "DIVP2_ACK";
        }
        else if(payload.type() == (uint32_t)TYPE::RESPONSE) {
            DevicePayload last_pkt = *this->last_response_pkt.pop_front();
            
            uint32_t num_pkts = 0;
            
            if(last_pkt.cmd() == 0) {
                if(last_pkt.sub_cmd() == 0) {
                    cmd = "MEM_FETCH";
                    num_pkts = 2;
                }
                else if(last_pkt.sub_cmd() == 1) {
                    cmd = "MEM_WRITE";
                    num_pkts = 3;
                }
            }

            float sec_per_payload = sizeof(DevicePayload) * 8 / float(BAUD_RATE);
            clk_count += uint32_t(sec_per_payload * num_pkts * DEVICE_CLK_HZ);
        }

        this->stats_log->log_plain(
            std::to_string(payload.id()) + "," +
            cmd + "," +
            std::to_string(clk_count) + "," +
            std::to_string(clk_count / float(DEVICE_CLK_HZ) * 1e6)
        );
    }
#endif

}

bool FunctionalImpl::receive_device_payload(void *payload)
{
    auto payload_opt = this->received_payloads.pop_front();

    if(payload_opt == std::nullopt)
        return false;
    
    ((DevicePayload*)payload)->copy(*payload_opt);
    
    this->log->log_info("[FunctionalImpl] Received device payload ->");
    this->log->log_info(((DevicePayload*)payload)->print());

    return true;
}