# PRESM - <b>PRE S</b>ilicon <b>M</b>odelling framework

PRESM is a modular framework to help in research, development and testing of hardware accelerators (think GPUs, NPUs). The core of PRESM is written in C/C++.

PRESM is currently under development. This project is so that I can ultimately do hardware research for photorealistic rendering. I am learning on the go, so things might not really make sense to seasoned people.

The general goals are:
- Provide a modular and extendable framework to model the function of a hardware accelerator.
    - Example: Can extend PRESM core classes to model a NVIDIA GPU treating PTX as the ISA.
- Enable any app, written for (or can be compiled to) the target ISA of the hardware accelerator to run without any source code modifications.
    - Example: The modeled NVIDIA GPU from the above example should be able to run <i>any</i> app written in CUDA and gather the output, without needing modifications or access to the app's source.
- Enable RTL verification of the hardware accelerator
    - Example: RTL can be written for the modeled NVIDIA GPU from the above example. PRESM should provide the ability to verify functional correctness of this RTL
- Enable any app to run on on RTL of the hardware accelerator
    - Example: Continuing the above example, PRESM should enable any CUDA app to run on the RTL, and gather its output.

Some notes:
- Functional Model: The intent of the core C++ code is to provide only a functional model of the hardware accelerator. It will not give any cycle-level measurements.
- Bit-accurate model: RTL verification, atleast for now, will be done by loading the RTL on a FPGA and running verification apps on it (either directly or through memory dumps). This can be considered a </i>bit-accurate</i> simulation, since it works on the bit-level on actual hardware logic.
    - This model will also provide cycle-level stats, but will not be "cycle-accurate" since the FPGA clocks and mem clocks will be quite different than silicon. It will however be possible to get an estimate of the performance.
- Cycle-accurate model: Not sure how this can be accomplished yet! Will need this for performance comparisons / estimates, before a silicon tape-out. This is quite an important aspect of pre-silicon modelling, and will require thought.

## Goals for the first release (v1.0)

For the first release we would like to achieve the following.
- We would like to have a set of common core classes that only need to be extended to implement a new hardware accelerator.
- We also want plumbing RTL modules as part of PRESM, that can be wrapped around user-written core logic RTL modules.

To make sure that we aren't writing abstractions in isolation, the core components will be developed while writing our own GP-GPU (General Purpose Graphics Processing Unit) model. Specifically:
- This GPU model will target the <a href="https://docs.nvidia.com/cuda/parallel-thread-execution/#syntax">PTX ISA</a>.
- Host can communicate with this accelerator using the <a href="https://docs.nvidia.com/cuda/archive/12.8.0/cuda-driver-api/index.html">CUDA API</a>, version 12.8.

Finally, once we can have a complete GPU model that passes all functional tests and RTL verification, we hope to tape it out with <a href="https://tinytapeout.com/">Tiny Tapeout</a>, and verify corretness and measure performance of real hardware.

This makes our first release end-to-end: from modelling to tape-out.

### Intermediate releases (v0.3, v0.6)
v1.0 involves a lot of moving components, each with deep complexity: device implementation, device memory, cuda driver API, PTX assembler, GPU RTL for PTX. Aside from being complicated, this is extensive too, since PTX itself is a big ISA and the cuda driver API is huge.

Its easier to start with a simple accelerator. To that end, we will have two sub-releases, with two different goals.

### Release v0.6

For this release, we target an AI model inference accelerator. Just inference - so no complicated driver API, no complicated ISA and relatively simple RTL, possibly the same as release v0.3. 

So, here is the plan:
- Our new hardware accelerator will only be for inference of AI models.
    - This accelerator and it's driver will not be super efficient, but will gurantee functional correctness.
- The ISA for this accelerator will be yet again <a href="https://docs.riscv.org/reference/isa/unpriv/intro.html">RISC-V ISA</a>, maybe just a reuse of the v0.3 accelerator.
- The driver API will be the <a href="https://onnxruntime.ai/docs/">GGML API</a>.

### Release v0.3

For this release, we focus on creating general purpose compute accelerator. We will define the API to communicate with the accelerator ourselves. 
- The accelerator will target the <a href="https://docs.riscv.org/reference/isa/unpriv/intro.html">RISC-V ISA</a>.
- Computation tasks (or kernels) will be written in plain C
    - Since these kernels will be plain C programs, we can just run them as is to get ground truth computations (for verification).

### v1.0, v0.6, v0.3 Progress Tracker
- [x] Code skeleton, with C++ and RTL sources
- [x] Automate building on FPGA toolchain
- [x] Automate RTL load on Tang Nano 20K FPGA
- [x] Script to build PRESM for device-driver configuration
- [x] Script to execute standalone app on PRESM
- [x] PRESM functional verification tests skeleton
- [x] Automate running of functional verification tests
- [x] Write simple vector addition CUDA test
- [ ] Functional model that passes vector addition test
- [ ] RTL model that passes vector addition test
- [ ] Automate running of a verification test on FPGA with the loaded RTL
- [ ] Automate running of a standalone test on FPGA with the loaded RTL 