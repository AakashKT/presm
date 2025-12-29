#include "app.h"

void run_vector_add()
{
    int N = 100;
    std::vector<int> a(N), b(N), c(N);

    CUdevice device;
    CUcontext context;
    CUdeviceptr d_a, d_b, d_c;

    // initialize host arrays
    for (int i = 0; i < N; ++i) {
        a[i] = N - i;
        b[i] = i * i;
    }

    // initialize
    printf("- Initializing...\n");
    initCUDA(&device, &context);

    CUmodule module = loadModule(context, "objects/ptx/vector_add/vector_add.ptx");
    CUfunction function = loadFunctionFromModule(context, module, "vector_add");

    // allocate memory
    checkCudaErrors( cuMemAlloc(&d_a, sizeof(int) * N) );
    checkCudaErrors( cuMemAlloc(&d_b, sizeof(int) * N) );
    checkCudaErrors( cuMemAlloc(&d_c, sizeof(int) * N) );

    // copy arrays to device
    checkCudaErrors( cuMemcpyHtoD(d_a, a.data(), sizeof(int) * N) );
    checkCudaErrors( cuMemcpyHtoD(d_b, b.data(), sizeof(int) * N) );

    // run
    printf("# Running the kernel...\n");

    void *args[3] = { &d_a, &d_b, &d_c };

    // grid for kernel: <<<N, 1>>>
    checkCudaErrors( cuLaunchKernel(function, N, 1, 1,  // Nx1x1 blocks
                                    1, 1, 1,            // 1x1x1 threads
                                    0, 0, args, 0) );

    printf("# Kernel complete.\n");

    // copy results to host and report
    checkCudaErrors( cuMemcpyDtoH(c.data(), d_c, sizeof(int) * N) );
    for (int i = 0; i < N; ++i) {
        printf("%d\n", c[i]);
    }
    printf("*** All checks complete.\n");


    // finish
    printf("- Finalizing...\n");
    
    checkCudaErrors( cuMemFree(d_a) );
    checkCudaErrors( cuMemFree(d_b) );
    checkCudaErrors( cuMemFree(d_c) );
}