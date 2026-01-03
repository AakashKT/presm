#include "cuda_verification.h"

bool CudaTestHandler::run_test(std::string test_name)
{
    if(test_name == "sanity")
        run_sanity();
    else if(test_name == "vector_add")
        run_vector_add();
    else 
        return false;

    return true;
}

TestHandler* get_test_handler()
{
    return new CudaTestHandler();
}

void initCUDA(CUdevice *device, CUcontext *context, bool print_debug_info)
{
    int deviceCount = 0;
    CUresult err = cuInit(0);
    int major = 0, minor = 0;

    if (err == CUDA_SUCCESS)
        checkCudaErrors(cuDeviceGetCount(&deviceCount));

    if (deviceCount == 0) {
        if(print_debug_info)
            fprintf(stderr, "Error: no devices supporting CUDA\n");
        exit(-1);
    }

    // get first CUDA device
    checkCudaErrors(cuDeviceGet(device, 0));
    char name[100];
    cuDeviceGetName(name, 100, *device);
    if(print_debug_info)
        printf("> Using device 0: %s\n", name);

    size_t totalGlobalMem = 0;
    checkCudaErrors( cuDeviceTotalMem(&totalGlobalMem, *device) );
    if(print_debug_info)
        printf("  Total amount of global memory:   %llu bytes\n",
           (unsigned long long)totalGlobalMem);
    if(print_debug_info)
        printf("  64-bit Memory Address:           %s\n",
           (totalGlobalMem > (unsigned long long)4*1024*1024*1024L)?
           "YES" : "NO");

    err = cuCtxCreate(context, 0, *device);
    if (err != CUDA_SUCCESS) {
        if(print_debug_info)
            fprintf(stderr, "* Error initializing the CUDA context.\n");
        exit(-1);
    }
}

CUmodule loadModule(CUcontext context, std::string module_file)
{
    CUmodule module;
    CUresult err = cuModuleLoad(&module, module_file.c_str());
    if (err != CUDA_SUCCESS) {
        fprintf(stderr, "* Error loading the module %s\n", module_file.c_str());
        exit(-1);
    }

    return module;
}

CUfunction loadFunctionFromModule(CUcontext context, CUmodule module, std::string function_name)
{
    CUfunction function;
    CUresult err = cuModuleGetFunction(&function, module, function_name.c_str());

    if (err != CUDA_SUCCESS) {
        fprintf(stderr, "* Error getting kernel function %s\n", function_name.c_str());
        exit(-1);
    }

    return function;
}