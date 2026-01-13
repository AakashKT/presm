# PRESM - <b>PRE S</b>ilicon <b>M</b>odelling framework

PRESM is a modular framework to help in research, development and testing of hardware accelerators (think GPUs, NPUs). The core of PRESM is written in C/C++.

#### PRESM is currently under active development!
This project exists so that one can ultimately do hardware research for photorealistic rendering. As a side effect, this project will also enable general accelerator hardware research.

See <a href="https://github.com/AakashKT/presm/blob/main/GOALS.md">GOALS.md</a> for a in-details description on PRESM's goals and the plan for progressive releases. 

## Building
Currently PRESM only works in a Linux environment. Its primary development & testing environment is Ubuntu.

Follow the below steps to build PRESM.

First, clone this repository with ```--recursive``` flag
```
git clone --recursive https://github.com/AakashKT/presm.git
```

The ```configs/``` directory contains various pre-configured driver-device combinations, including configurations of verification tests.

### Building for the first time
The following steps build the ```riscv_compute_accelerator``` configuration.

First run the following
```
python scripts/build.py --config configs/riscv_compute_accelerator.json --get_extern_tools
```
Note the ```--get_extern_tools``` flag, which is required when building for the first time. Subsequent buildS can/should not include this flag.

### Rebuilding & Clean builds
To rebuild, run
```
python scripts/build.py --config configs/riscv_compute_accelerator.json
```

For a clean build, run
```
python scripts/build.py --config configs/riscv_compute_accelerator.json --clean
```