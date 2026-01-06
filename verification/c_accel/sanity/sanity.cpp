#include "c_accel_app.h"
#include "c_accel.h"
#include "sanity_kernel.h"

void run_sanity(std::vector<std::string> args)
{
    if(args[0] == "device") {
        caDeviceInit();
        // caSetKernel("sanity_kernel.riscv");
    }

    int output;
    for(int i=0; i<1000; i++) {
        int a = rand() % 100000;
        int b = rand() % 100000;
        int c = rand() % 100000;

        if(args[0] == "device") {
            // caSetKernelParams(0, 1 * sizeof(int), (void*)&a);
            // caSetKernelParams(1, 1 * sizeof(int), (void*)&b);
            // caSetKernelParams(2, 1 * sizeof(int), (void*)&c);

            // caKernelRun();

            // int* kernelOutput = (int*)caGetKernelOutput();
            // output = kernelOutput[0];
        }
        else
            output = fsa_func(a, b, c);
        
        std::cout << output << std::endl;
    }
}