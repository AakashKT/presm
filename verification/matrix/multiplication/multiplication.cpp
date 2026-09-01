#include "matrix.h"
#include "matrix_app.h"

void mul_device_run()
{
    mInit();

    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(-80);
        MIntDeviceMemory c(10);

        mMultiply(a, b, c);
        mSync();

        std::cout << a.getValue() << " * " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(-64);
        MIntDeviceMemory b(80);
        MIntDeviceMemory c(10);

        mMultiply(a, b, c);
        mSync();

        std::cout << a.getValue() << " * " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(-80);
        MIntDeviceMemory c(10);

        mMultiply(a, b, c);
        mSync();

        std::cout << a.getValue() << " * " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(80);
        MIntDeviceMemory c(10);

        mMultiply(a, b, c);
        mSync();

        std::cout << a.getValue() << " * " << b.getValue() << " = " << c.getValue() << std::endl;
    }

    mFree();
}

void mul_host_run()
{
    {
        int a = -64;
        int b = -80;
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }

    {
        int a = -64;
        int b = 80;
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }

    {
        int a = 64;
        int b = -80;
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }

    {
        int a = 64;
        int b = 80;
        int c = 10;

        c = a * b;

        std::cout << std::to_string(a) << " * " << std::to_string(b) << " = " << std::to_string(c) << std::endl;
    }
}

void run_multiplication(std::vector<std::string> args)
{
    if(args[0] == "device")
        mul_device_run();
    else
        mul_host_run();
}