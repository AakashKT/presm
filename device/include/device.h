#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class PRESMDevice {
public:
    PRESMDevice();

    std::string get_name() { return name; };

private:
    std::string name;
};

#endif