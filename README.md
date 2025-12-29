# PRESM - <b>PRE S</b>ilicon <b>M</b>odelling framework

PRESM is a modular framework to help in research, development and testing of hardware accelerators (think GPUs, NPUs). The core of PRESM is written in C/C++.

PRESM is currently under development. 

The general goals of PRESM are:
- Provide a modular and extendable framework to model the function of a hardware accelerator.
    - Example: Can extend PRESM core classes to model a NVIDIA GPU treating the ISA as PTX
- Enable any app, written for the target ISA of the hardware accelerator to run without any source code modifications.
    - Example: The modeled NVIDIA GPU from the above example should be able to run <b>ANY</b> app written in CUDA and gather the output, without any modifications to its source.
- Enable RTL verification of the hardware accelerator
    - Example: RTL can be written for the modeled NVIDIA GPU from the above example. PRESM should provide the ability to verify functional correctness of this RTL
- Enable any app to run on on RTL of the hardware accelerator
    - Example: Continuing the above example, PRESM should enable any CUDA app to run on the RTL, and gather its output.

Some notes about the methodology:
- Functional Model: The intent of the C++ code is to provide only a functional verification of the hardware accelerator. It will not give any cycle-level measurements.
- Bit-accurate model: RTL verification, atleast for now, is done by loading the RTL on a FPGA and running the test on it (either directly or through memory dumps). The can be considered a </i>bit-accurate</i> simulation, since it works on the bit-level.
    - This model will also provide cycle-level stats, but will not be "cycle-accurate" since the FPGA clocks and mem clocks are bound to be different to actual hardware
- Cycle-accurate model: Not sure how this can be accomplished yet! This is outside the scope of the first release.

## Goals for the first release (v1.0)

For the first release we would like to achieve the following.
- We would like to have a set of common core classes that only need to be extended to implement a new hardware accelerator.
- We also want plumbing RTL modules as part of PRESM, that can be wrapped around user-written core logic RTL modules.

To make sure that we aren't writing abstractions in isolation, the core components will be developed while writing our own GPU (Graphics Processing Unit) model. Specifically:
- This GPU model targets the <a href="https://docs.nvidia.com/cuda/parallel-thread-execution/#syntax">PTX</a> ISA.
- The interface with this accelerator will be the <a href="https://docs.nvidia.com/cuda/archive/12.8.0/cuda-driver-api/index.html">CUDA API</a>, version 12.8

Finally, once we can have a complete GPU model that passes all functional tests and RTL verification, we hope to tape it out with <a href="https://tinytapeout.com/">Tiny Tapeout</a>, and verify and measure performance of actual hardware.

This makes our first release end-to-end: from modelling to tape-out.

## v1.0 Progress
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