#ifndef SERIAL_H
#define SERIAL_H

#include "serial_device.h"

class SerialImpl : public SerialDevice {
public:
    SerialImpl();

    void handle_incoming_byte(char data) override;

private:

};

#endif