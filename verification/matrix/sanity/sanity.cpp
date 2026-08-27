#include "matrix.h"
#include "matrix_app.h"

void device_run()
{
    mInit();

    MIntDeviceMemory a(60);
    MIntDeviceMemory b(20);
    MIntDeviceMemory c(10);

    MCommandInfo addCmd = mAdd(a, b, c);
    mSync(addCmd);

    std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;

    mFree();
}

void host_run()
{
    int a = 64;
    int b = 20;
    int c = 10;

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