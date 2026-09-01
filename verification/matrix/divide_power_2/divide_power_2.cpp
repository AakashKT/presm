#include "matrix.h"
#include "matrix_app.h"

void divp2_device_run()
{
    mInit();

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(4);
        MIntDeviceMemory c(10);

        mDivPow2(a, b, c);
        mSync();

        std::cout << a.getValue() << " / (2^" << b.getValue() << ") = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(4);
        MIntDeviceMemory c(10);

        mDivPow2(a, b, c);
        mSync();

        std::cout << a.getValue() << " / (2^" << b.getValue() << ") = " << c.getValue() << std::endl;
    }

    mFree();
}

void divp2_host_run()
{
    {
        int n = 4;
        int a = 64;
        int b = std::pow(2, n);
        int c = 10;

        c = a / b;

        std::cout << std::to_string(a) << " / (2^" << std::to_string(n) << ") = " << std::to_string(c) << std::endl;
    }

    {
        int n = 4;
        int a = -64;
        int b = std::pow(2, n);
        int c = 10;

        c = a / b;

        std::cout << std::to_string(a) << " / (2^" << std::to_string(n) << ") = " << std::to_string(c) << std::endl;
    }
}

void run_divp2(std::vector<std::string> args)
{
    if(args[0] == "device")
        divp2_device_run();
    else
        divp2_host_run();
}