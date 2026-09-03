#include "matrix.h"
#include "matrix_app.h"

void multiple_device_run()
{
    mInit();

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(4);

        MIntDeviceMemory add(10);
        mAdd(a, b, add);

        MIntDeviceMemory mul(10);
        mMulPow2(a, b, mul);

        MIntDeviceMemory div(10);
        mDivPow2(a, b, div);

        mSync();

        std::cout << add.getValue() << std::endl;
        std::cout << mul.getValue() << std::endl;
        std::cout << div.getValue() << std::endl;
    }

    {
        MIntDeviceMemory a(64);
        MIntDeviceMemory b(4);

        MIntDeviceMemory add1(10);
        mAdd(a, b, add1);

        MIntDeviceMemory mul1(10);
        mMulPow2(a, b, mul1);

        MIntDeviceMemory div1(10);
        mDivPow2(a, b, div1);


        
        MIntDeviceMemory c(4325);
        MIntDeviceMemory d(6);

        MIntDeviceMemory add2(10);
        mAdd(c, d, add2);

        MIntDeviceMemory mul2(10);
        mMulPow2(c, d, mul2);

        MIntDeviceMemory div2(10);
        mDivPow2(c, d, div2);

        

        MIntDeviceMemory e(324);
        MIntDeviceMemory f(9);

        MIntDeviceMemory add3(10);
        mAdd(e, f, add3);

        MIntDeviceMemory mul3(10);
        mMulPow2(e, f, mul3);

        MIntDeviceMemory div3(10);
        mDivPow2(e, f, div3);



        MIntDeviceMemory g(665);
        MIntDeviceMemory h(3);

        MIntDeviceMemory add4(10);
        mAdd(g, h, add4);

        MIntDeviceMemory mul4(10);
        mMulPow2(g, h, mul4);

        MIntDeviceMemory div4(10);
        mDivPow2(g, h, div4);


        mSync();

        std::cout << add1.getValue() << std::endl;
        std::cout << mul1.getValue() << std::endl;
        std::cout << div1.getValue() << std::endl;

        std::cout << add2.getValue() << std::endl;
        std::cout << mul2.getValue() << std::endl;
        std::cout << div2.getValue() << std::endl;

        std::cout << add3.getValue() << std::endl;
        std::cout << mul3.getValue() << std::endl;
        std::cout << div3.getValue() << std::endl;

        std::cout << add4.getValue() << std::endl;
        std::cout << mul4.getValue() << std::endl;
        std::cout << div4.getValue() << std::endl;
    }

    mFree();
}

void multiple_host_run()
{
    {
        int a = 64;
        int b = 4;

        int add = a + b;
        int mul = a * std::pow(2, b);
        int div = a / std::pow(2, b);

        std::cout << add  << std::endl;
        std::cout << mul  << std::endl;
        std::cout << div  << std::endl;
    }

    {
        int a = 64;
        int b = 4;

        int add1 = a + b;
        int mul1 = a * std::pow(2, b);
        int div1 = a / std::pow(2, b);
        
        a = 4325;
        b = 6;
        int add2 = a + b;
        int mul2 = a * std::pow(2, b);
        int div2 = a / std::pow(2, b);

        a = 324;
        b = 9;
        int add3 = a + b;
        int mul3 = a * std::pow(2, b);
        int div3 = a / std::pow(2, b);
        
        a = 665;
        b = 3;
        int add4 = a + b;
        int mul4 = a * std::pow(2, b);
        int div4 = a / std::pow(2, b);

        std::cout << add1  << std::endl;
        std::cout << mul1  << std::endl;
        std::cout << div1  << std::endl;

        std::cout << add2  << std::endl;
        std::cout << mul2  << std::endl;
        std::cout << div2  << std::endl;

        std::cout << add3  << std::endl;
        std::cout << mul3  << std::endl;
        std::cout << div3  << std::endl;

        std::cout << add4  << std::endl;
        std::cout << mul4  << std::endl;
        std::cout << div4 << std::endl;
    }
}

void run_multiple(std::vector<std::string> args)
{
    if(args[0] == "device")
        multiple_device_run();
    else
        multiple_host_run();
}