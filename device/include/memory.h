#ifndef MEMORY_H
#define MEMORY_H

class DeviceMemory {
public:
    DeviceMemory(unsigned long long size_in_bytes);

    unsigned long long get_size() { return size; };

private:
    unsigned long long size; 
};

#endif