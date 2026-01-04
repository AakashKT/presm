#include "c_accel.h"
#include "sanity_kernel.h"

void run_sanity(std::vector<std::string> args)
{
    int output;
    for(int i=0; i<1000; i++) {
        if(args[0] == "host")
            output = fma_func(rand() % 100000, rand() % 100000, rand() % 100000);
        else
            std::cout << "Device functions not implemented yet" << std::endl;
        
        std::cout << output << std::endl;
    }
}