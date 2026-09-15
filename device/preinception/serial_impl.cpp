#include "serial_impl.h"

Device* get_device()
{
    return new SerialImpl();
}

SerialImpl::SerialImpl()
    : SerialDevice()
{
    this->device_memory = new HostResidentMemory(HOST_RESIDENT_MEM_SIZE);

#if DEVICE_DEBUG
    this->stats_log = new Logger();
    this->stats_log->init("device_cp_stats", true);
    this->stats_log->log_plain("PKT_ID,INSTR,CLK_CYCLES,DERIVED_RUNTIME_MSEC");
#endif

}

SerialImpl::~SerialImpl()
{
    this->log->log_info("[SerialImpl] Destructor called");
#if DEVICE_DEBUG
    delete this->stats_log;
#endif  
}

void SerialImpl::send_device_payload(void* payload)
{
    DevicePayload* sc = (DevicePayload*) payload;

    auto bytes_written = write(this->port_fd, (char*)sc->packet, 6);
    if (bytes_written != 6)
        this->log->log_error_and_exit("[SerialImpl] Failed to send device payload");

    this->log->log_info("[SerialImpl] Sent device payload ->");
    this->log->log_info(sc->print());

#if DEVICE_DEBUG
    if(sc->type() == (uint32_t)TYPE::REQUEST)
        this->last_request_pkt.push_back(*sc);
    else if(sc->type() == (uint32_t)TYPE::RESPONSE)
        this->last_response_pkt.push_back(*sc);
#endif

}

bool SerialImpl::receive_device_payload(void *payload)
{
    auto payload_opt = this->received_payloads.pop_front();

    if(payload_opt == std::nullopt)
        return false;
    
    ((DevicePayload*)payload)->copy(*payload_opt);
    
    this->log->log_info("[SerialImpl] Received device payload ->");
    this->log->log_info(((DevicePayload*)payload)->print());

    return true;
}

uint32_t SerialImpl::allocate_device_memory(uint32_t size_in_bytes)
{
    return this->device_memory->allocate(size_in_bytes);
}

void SerialImpl::write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data)
{
    this->device_memory->write(address, size_in_bytes, data);
}

char* SerialImpl::read_from_device_memory(uint32_t address, uint32_t size_in_bytes)
{
    return this->device_memory->read(address, size_in_bytes);
}

void SerialImpl::process_device_request(DevicePayload& payload)
{
    if(payload.sub_cmd() == 0) {
        this->log->log_info("[SerialImpl] Device requested read, payload ->");
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
        if(this->device_packet_recv_state == ADDR_RECV) {
            this->log->log_info("[SerialImpl] Device requested write to address, payload ->");
            this->log->log_info(payload.print());

            this->mem_write_addr_scratch = payload.fields32.body;
            this->device_packet_recv_state = VAL_RECV;
        }
        else if(this->device_packet_recv_state == VAL_RECV) {
            this->log->log_info("[SerialImpl] Device requested write value to above address, payload ->");
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

            this->device_packet_recv_state = ADDR_RECV;
        }
    }

#if DEVICE_DEBUG
    else if(payload.sub_cmd() == 15) {
        std::string cmd = "UNKNOWN";
        
        if(payload.type() == (uint32_t)TYPE::REQUEST) {
            DevicePayload last_pkt = *this->last_request_pkt.pop_front();
            
            if(last_pkt.cmd() == (uint32_t)CMD::ADD)
                cmd = "ADD_ACK";
            else if(last_pkt.cmd() == (uint32_t)CMD::MULP2)
                cmd = "MULP2_ACK";
            else if(last_pkt.cmd() == (uint32_t)CMD::DIVP2)
                cmd = "DIVP2_ACK";
        }
        else if(payload.type() == (uint32_t)TYPE::RESPONSE) {
            DevicePayload last_pkt = *this->last_response_pkt.pop_front();

            if(last_pkt.cmd() == 0) {
                if(last_pkt.sub_cmd() == 0)
                    cmd = "MEM_FETCH";
                else if(last_pkt.sub_cmd() == 1)
                    cmd = "MEM_WRITE";
            }
        }

        this->stats_log->log_plain(
            std::to_string(payload.id()) + "," +
            cmd + "," +
            std::to_string(payload.fields32.body) + "," +
            std::to_string(payload.fields32.body / float(DEVICE_CLK_HZ) * 1e6)
        );
    }
#endif

}

void SerialImpl::serial_read_process(char data)
{
    this->scratch.packet[this->scratch_ptr++] = data;

    if(this->scratch_ptr == 6) {
        if(this->scratch.cmd() == 0) 
            this->process_device_request(this->scratch);
        else
            this->received_payloads.push_back(this->scratch);

        this->scratch_ptr = 0;
    }
}

void SerialImpl::device_find()
{
    std::string port_string_base = "/dev/ttyUSB";
    bool found = false;

    for(int i=0; i<5; i++) {
        std::string port_string = port_string_base + std::to_string(i);

        bool opened = this->open_serial_port(port_string);
        if(!opened) {
            this->log->log_info("[SerialImpl] Failed to open serial port '" + port_string + "'.");
            continue;
        }

        this->configure_serial_port(BAUD_RATE);
        tcflush(this->port_fd, TCIOFLUSH);

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

        DevicePayload rx;
        auto begin_time = std::chrono::high_resolution_clock::now();
        uint8_t rx_ptr = 0;
        char temp;
        while(true) {
            ssize_t bytes_read = read(this->port_fd, &rx.packet[rx_ptr], 1);

            if(bytes_read == 1) {
                rx_ptr += 1;

                if(rx_ptr == 6) {
                    this->log->log_info("[SerialImpl] Received device payload ->");
                    this->log->log_info(rx.print());
                    if(rx.id() == 1 && rx.type() == static_cast<uint32_t>(TYPE::RESPONSE)
                        && rx.fields.body_1 == 2 && rx.fields.body_2 == 1) {
                        this->log->log_info("[SerialImpl] Found device in serial port '" + port_string + "'.");
                        found = true;
                    }

                    break;
                }
            }

            auto time_diff = std::chrono::high_resolution_clock::now() - begin_time;
            if(found || std::chrono::duration_cast<std::chrono::microseconds>(time_diff).count() >= 2e6)
                break;
        }

        if(found)
            break;
        else {
            tcflush(this->port_fd, TCIOFLUSH);
            close(this->port_fd);
        }
    }

    if(!found)
        this->log->log_error_and_exit("[SerialImpl] Could not find device over serial port.");
}