#include "app.h"

void run_sanity()
{
    CUresult res = cuInit(0);
    std::cout << res << std::endl;

    int temp = 0;
    cuDeviceGetCount(&temp);

    std::cout << temp << std::endl;
}