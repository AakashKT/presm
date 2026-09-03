# PRESM - <b>PRE S</b>ilicon <b>M</b>odelling framework for hardware accelerators

PRESM is a modular framework to help in research, development and testing of hardware accelerators (think GPUs, NPUs). The core of PRESM is written in C/C++.

#### PRESM is currently under active development!
This project exists so that one can ultimately do hardware research for photorealistic rendering. As a side effect, this project will also enable general accelerator hardware research.

See <a href="https://github.com/AakashKT/presm/blob/main/GOALS.md">GOALS.md</a> for a in-details description on PRESM's goals and the plan for progressive releases. 

## Table of Contents  
- [Device Configurations](#device-configurations)
- [Building](#building)  
    - [Building for the first time](#building-for-the-first-time) 
    - [Rebuilding and Clean builds](#rebuilding-and-clean-builds)
- [Verification Apps](#verification-apps)
- [Running your own app on PRESM](#running-your-own-app-on-presm)
    - [Souce code of the app is available](#souce-code-of-the-app-is-available)
    - [Only executable of the app is available](#only-executable-of-the-app-is-available)

## Device Configurations


## Building
Currently PRESM only works in a Linux environment. Its primary development & testing environment is Ubuntu.

Follow the below steps to build PRESM.

First, clone this repository with ```--recursive``` flag
```
git clone --recursive https://github.com/AakashKT/presm.git
```

The ```hw_configs/``` directory contains various pre-configured devices with their drivers, including configurations of verification tests.

### Building for the first time
The following steps build the ```preinception``` configuration.

First run the following
```
python scripts/build.py --config hw_configs/preinception/functional.json --get_extern_tools
```
Note the ```--get_extern_tools``` flag, which is required when building for the first time. Subsequent builds should not include this flag.

### Rebuilding and Clean builds
To rebuild, run
```
python scripts/build.py --config hw_configs/preinception/functional.json
```

For a clean build, run
```
python scripts/build.py --config hw_configs/preinception/functional.json --clean
```

## Verification Apps
Every configured device-driver is accompanied by a verification test app suite. These tests help validate the result of running the app on PRESM v/s on host. Such validation is important to ensure that PRESM-implemented devices function correctly.

Verification apps are listed in the device configuration file like so:
```
"verification": [
    {
        "name": "sanity",
        "args_host_run": ["host"],
        "args_device_run": ["device"],
        "verify": true,
        "enabled": true
    }
]
```
The above defines a single verification app named ```sanity```.

```args_host_run``` are passed as command line arguments to the test when running on the host (not on PRESM).

```args_device_run``` are paseed as command line arguments to the test when running on PRESM.

If ```verify``` is set to ```true```, the verification app is run both on the host and on PRESM, and the outputs are compared. The app passes verification only if the output matches.

If ```verify``` is ```false```, the app will only be run on PRESM and its output printed.

Verification can be run with the following script
```
python scripts/verify.py --config hw_configs/preinception/functional.json
```

## RTL testbenches
RTL verification via simulation is important for debugging and initial design iteration.

PRESM includes scripts to run RTL testbenches (written in ```cocotb```):
```
python scripts/rtl_testbench_run.py --config hw_configs/preinception/serial.json
```

## Running your own app on PRESM

### Souce code of the app is available
If the source code is available, you need to modify it to include PRESM driver's header files and link it to PRESM driver library. These are available in post building PRESM here:
```
packages/${DRIVER_NAME}/include
packages/${DRIVER_NAME}/lib
```

An example ```CMakeLists.txt``` modification might look like:
```
target_include_directories(your_app PUBLIC packages/${DRIVER_NAME}/include)
target_link_directories(your_app PUBLIC packages/${DRIVER_NAME}/lib)
target_link_libraries(your_app PUBLIC libdriver.so)
```

Compiling the app with above modification should result in all calls made to the driver functions intercepted by PRESM.

### Only executable of the app is available
If only an executable is available, you can use the following script
```
python scripts/execute.py --config configs/riscv_compute_accelerator.json --executable /relative/path/to/executable
```

Under the hood, this script uses the ```LD_PRELOAD``` trick (available in Linux OS) to preload the PRESM library, allowing PRESM to intercept all driver calls.