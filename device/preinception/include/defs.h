#ifndef DEVICE_DEFS_H
#define DEVICE_DEFS_H

union DevicePayload {

    struct Fields {
        uint8_t id;
        uint8_t cmd;
        uint8_t sub_cmd;
        uint8_t num_bytes;
        uint8_t body_1;
        uint8_t body_2;
        uint8_t body_3;
        uint8_t body_4;
    } fields;

    uint8_t packet[8];
};

#endif