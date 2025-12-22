#include "app.h"

void run_sanity()
{
    CUdevice device;
    CUcontext context;

    initCUDA(&device, &context, true);
}