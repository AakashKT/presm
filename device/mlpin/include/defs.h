#ifndef DEFS_H
#define DEFS_H

union RequestHeader {
    struct __attribute__((packed)) Fields {
        uint8_t id;
        uint8_t cmd;
        uint8_t size_bytes;
    } fields;

    uint8_t byte[sizeof(Fields)];
};

union ResponseHeader {
    struct __attribute__((packed)) Fields {
        uint8_t id;
    } fields;

    uint8_t byte[sizeof(Fields)];
};

union DeviceInfoRequest {
    struct __attribute__((packed)) Fields {
        RequestHeader header;
    } fields;

    uint8_t byte[sizeof(Fields)];

    DeviceInfoRequest() {
        fields.header.fields.id = 0;
        fields.header.fields.cmd = 0;
        fields.header.fields.size_bytes = 0;
    }
};

union DeviceInfoResponse {
    struct __attribute__((packed)) Fields {
        ResponseHeader header;
        uint32_t signature;
        uint64_t mem_size_in_bytes;
    } fields;

    uint8_t byte[sizeof(Fields)];
};


#endif