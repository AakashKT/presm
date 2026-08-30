#include "matrix.h"
#include "matrix_app.h"

void device_run()
{
    mInit();

    // Unsigned integer
    {
        MUIntDeviceMemory a(64);
        MUIntDeviceMemory b(80);
        MUIntDeviceMemory c(10);

        MCommandInfo addCmd = mAdd(a, b, c);
        mSync(addCmd);

        std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    // Integer
    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(-80);
        MIntDeviceMemory c(10);

        MCommandInfo addCmd = mAdd(a, b, c);
        mSync(addCmd);

        std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(80);
        MIntDeviceMemory c(10);

        MCommandInfo addCmd = mAdd(a, b, c);
        mSync(addCmd);

        std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    mFree();
}

void host_run()
{
    // Unsigned integer
    {
        uint32_t a = 64;
        uint32_t b = 80;
        uint32_t c = 10;

        c = a + b;

        std::cout << std::to_string(a) << " + " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }

    // Integer
    {
        int a = -64;
        int b = -80;
        int c = 10;

        c = a + b;

        std::cout << std::to_string(a) << " + " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }

    {
        int a = -64;
        int b = 80;
        int c = 10;

        c = a + b;

        std::cout << std::to_string(a) << " + " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }
}

void run_sanity(std::vector<std::string> args)
{
    if(args[0] == "device")
        device_run();
    else
        host_run();
}