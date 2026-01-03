#ifndef CUDA_VERIFICATION_APP_H
#define CUDA_VERIFICATION_APP_H

#include "app.h"

#include <cuda.h>
#include <builtin_types.h>

#define checkCudaErrors(err)  __checkCudaErrors (err, __FILE__, __LINE__)

inline void __checkCudaErrors( CUresult err, const char *file, const int line )
{
    if( CUDA_SUCCESS != err) {
        fprintf(stderr,
                "CUDA Driver API error = %04d from file <%s>, line %i.\n",
                err, file, line );
        exit(-1);
    }
}

void initCUDA(CUdevice *device, CUcontext *context, bool print_debug_info = false);
CUmodule loadModule(CUcontext context, std::string module_file);
CUfunction loadFunctionFromModule(CUcontext context, CUmodule module, std::string function_name);

class CudaTestHandler : public TestHandler {
public:
    CudaTestHandler() {};

    bool run_test(std::string test_name) override;
};

void run_sanity();
void run_vector_add();

#endif