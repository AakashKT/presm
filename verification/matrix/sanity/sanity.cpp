#include "matrix.h"
#include "matrix_app.h"

void device_run()
{
    mInit();

    MUIntDeviceMemory a(64);
    MUIntDeviceMemory b(20);
    MUIntDeviceMemory c(10);

    MCommandInfo addCmd = mAdd(a, b, c);
    mSync(addCmd);

    std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;

    mFree();
}

void host_run()
{
    uint32_t a = 64;
    uint32_t b = 20;
    uint32_t c = 10;

    c = a + b;

    std::cout << std::to_string(a) << " + " << std::to_string(b) << " = " << std::to_string(c) << std::endl; 
}

void run_sanity(std::vector<std::string> args)
{
    if(args[0] == "device")
        device_run();
    else
        host_run();
}