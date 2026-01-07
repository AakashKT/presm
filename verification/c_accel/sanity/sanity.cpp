#include "c_accel_app.h"
#include "c_accel.h"
#include "sanity_kernel.h"

void run_sanity(std::vector<std::string> args)
{
    if(args[0] == "device") {
        caDeviceInit();
        caSetKernel("sanity_kernel.s");
    }

    int output;
    for(int i=0; i<1000; i++) {
        int a = rand() % 100000;
        int b = rand() % 100000;
        int c = rand() % 100000;

        caDeviceBuffer a_device(1 * sizeof(int));
        caCopyHtoD((void*)&a, a_device, 1 * sizeof(int));

        caDeviceBuffer b_device(1 * sizeof(int));
        caCopyHtoD((void*)&b, b_device, 1 * sizeof(int));

        caDeviceBuffer c_device(1 * sizeof(int));
        caCopyHtoD((void*)&c, c_device, 1 * sizeof(int));

        caDeviceBuffer d_device(1 * sizeof(int));

        if(args[0] == "device") {
            caSetKernelParams(0, a_device);
            caSetKernelParams(1, b_device);
            caSetKernelParams(2, c_device);
            caSetKernelParams(3, d_device);

            caKernelRun();
            caSync();

            // int* kernelOutput = (int*)caGetKernelOutput();
            // int d[1] = {0};
            // caCopyDtoH(d_device, (void*)d, 1 * sizeof(int));

            // if(d[0] == kernelOutput[0])
            //     output = kernelOutput[0];
            // else
            //     std::cout << "Failed! This should never happen" << std::endl;
        }
        else
            fsa_func(&a, &b, &c, &output);
        
        // std::cout << output << std::endl;
    }
}