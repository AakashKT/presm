#include "matrix.h"
#include "matrix_app.h"

void mulp2_device_run()
{
    mInit();

    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(4);
        MIntDeviceMemory c(10);

        mMulPow2(a, b, c);
        mSync();

        std::cout << a.getValue() << " * (2^" << b.getValue() << ") = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(4);
        MIntDeviceMemory c(10);

        mMulPow2(a, b, c);
        mSync();

        std::cout << a.getValue() << " * (2^" << b.getValue() << ") = " << c.getValue() << std::endl;
    }

    mFree();
}

void mulp2_host_run()
{
    {
        int n = 4;
        int a = -64;
        int b = std::pow(2, n);
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * (2^" << n << ") = " << std::to_string(c) << std::endl;
    }

    {
        int n = 4;
        int a = 64;
        int b = std::pow(2, n);
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * (2^" << n << ") = " << std::to_string(c) << std::endl;
    }
}

void run_mulp2(std::vector<std::string> args)
{
    if(args[0] == "device")
        mulp2_device_run();
    else
        mulp2_host_run();
}