# PRESM - <b>PRE S</b>ilicon <b>M</b>odelling framework

PRESM is a modular framework to help in research, development and testing of hardware accelerators (think GPUs, NPUs), all the way to a ASIC tapeout. 

#### PRESM is currently under active development!

This project exists so that one can ultimately do hardware research for real-time physically-based rendering. As a side effect, this project will also enable general accelerator hardware research for compute and ML (called AI these days).

## News
- **PRESM [v0.3](https://github.com/AakashKT/presm/releases/tag/v0.3) is out!** This release marks a end-to-end proof-of-concept, from functional modelling to tapeout.
  - [Preinception](https://github.com/AakashKT/presm/tree/main/device/preinception) device coupled with the [Matrix](https://github.com/AakashKT/presm/tree/main/driver/matrix) driver, all developed in PRESM.
  - Submitted for [tapeout](https://github.com/AakashKT/preinception-ihp26b-tapeout) on Tiny Tapeout's [IHP26b](https://app.tinytapeout.com/shuttles/ttihp26b) shuttle (delivery expected Aug. 2027)

## Table of Contents  
- [Why is PRESM useful?](#why-is-presm-useful)
- [Building PRESM](#building-presm) 
- [Hardware Config Definition](#hardware-config-definition) 
- [Functional Modelling](#functional-modelling)
- [Functional Verification](#functional-verification)
- [Hardware Emulation and Verification](#hardware-emulation-and-verification)
- [Hardware Design Verification](#hardware-design-verification)
 
## Why is PRESM useful?
The development of a hardware accelerator has a typical journey:
- Defining the hardware architecture, it's modules with their function and host communication protocol.
- Defining & implementing the driver (which runs on the host) and communicates with the accelerator.
- Validating the functional correctness of the driver and the hardware.
- Implementing the hardware in a Hardware Description Language (HDL).
- Unit testing of the HDL modules (design verification).
- Emulating the hardware for HDL correctness, driver-device communication validation and performance estimates. 
  - The hardware emulation is usually done on a Field Programmable Gate Array (FPGA), implying that the whole stack needs to be tested against HDL running on an FPGA.
- Submit for tapeout.
  - A bunch of testing will also take place after the ASIC is received.

PRESM assists with each of the above:
- Hardware is initially defined as C++ classes which inherit PRESM base classes to define hardware modules and connections, all of which is simulated in a multi-threaded fashion. **This is [functional modelling](#functional-modelling) of the hardware architecture**.
- Driver code is implemented within PRESM, and can get the underlying device with PRESM helpers. These abstract the underlying hardware realization, which allows hardware-independent driver depelopment. 
- Verification tests written within the PRESM framework allow for quick, progressive and comprehensive testing. **This is [functional verification](#functional-verification)**.
- PRESM provides common HDL modules and a framework to easily run testbenches on user-defined HDL modules. **This is [hardware desgin verification](#hardware-design-verification)**.
- PRESM additionally provides scripts to compose HDL modules and programme on commonly available FPGAs. With the device abstraction in PRESM, the already developed driver can now communicate with the FPGA instead of the functional model. **This is [hardware emulation & verification](#hardware-emulation-and-verification)**.

## Building PRESM
Currently PRESM only works in a Linux environment. Its primary development & testing environment is Ubuntu.

Follow the below steps to build PRESM.

First, clone this repository with ```--recursive``` flag
```bash
git clone --recursive https://github.com/AakashKT/presm.git
```

The ```hw_configs/``` directory contains various pre-configured devices with their drivers, including configurations of verification tests.

### Building for the first time
First run the following
```bash
python scripts/build.py --config hw_configs/preinception/config.json --get_extern_tools
```
Note the ```--get_extern_tools``` flag, which is required when building for the first time. Subsequent builds should not include this flag.

### Rebuilding and Clean builds
To rebuild, run
```bash
python scripts/build.py --config hw_configs/preinception/config.json
```

For a clean build, run
```bash
python scripts/build.py --config hw_configs/preinception/config.json --clean
```

## Hardware Config Definition
Hardware configurations define hardware properties along with which driver to use. Configurations are defined in JSON. The following are mandatory to be present in any configuration:
- ```log_enabled```: Defines if logs are exported after a run.
- ```driver```: The name of the driver, which corresponds to a directory in ```driver/```.
- ```device```: The name of the device, which again corresponds to a directory in ```device/```.
    - ```type```: The device type, currently supported are one of ```functional``` or ```serial```. Helps in choosing different sources files to compile.
    - ```memory_size_in_bytes```: Device memory size.
- ```verification```: List of functional verification apps for the above driver. These are picked up from ```verification/[driver_name]``` directory.
```json
{
    "log_enabled": true,
    
    "driver": {
        "name": "..."
    },

    "device": {
        "name": "...",
        "type": "...",
        "memory_size_in_bytes": 1e9
    },

    "verification": [
        {
            "name": "sanity",
            "args_host_run": ["host"],
            "args_device_run": ["device"],
            "verify": true,
            "enabled": true
        }
    ]
}
```

All configurations are located in ```hw_configs/``` directory.

## Functional Modelling
A multi-threaded hardware module simulation, assisted by PRESM base classes.

To write a functional hardware module, extend the ```HwModule``` class with an appropriate type of message type. A message type is the format in which messages are sent/received from this module to others. 

As an example, here is ```HwCp``` module with a ```DevicePayload``` message type:
```c++
class HwCp : public HwModule<DevicePayload> {
public:
    HwCp();
    
    std::string module_name() override;
    void execute(uint32_t block_idx) override;
};
```
Refer to ```HwModule``` class [definition](https://github.com/AakashKT/presm/blob/main/device/include/hw_module.h) on which functions need to be overridden.

A functional hardware device that uses ```HwCp``` can inherit from the ```Device``` base class of PRESM:
```c++
class FunctionalImpl : public Device {
public:
    FunctionalImpl();
    ~FunctionalImpl();

    void device_initialize() override;
    void send_device_payload(void* payload) override;
    bool receive_device_payload(void *payload) override;
    uint32_t allocate_device_memory(uint32_t size_in_bytes) override;
    void write_to_device_memory(uint32_t address, uint32_t size_in_bytes, const char* data) override;
    char* read_from_device_memory(uint32_t address, uint32_t size_in_bytes) override;

private:
    HwCp command_processor;
}
```
Refer to ```Device``` class [definition](https://github.com/AakashKT/presm/blob/main/device/include/device.h) on which functions need to be overridden.

The configuration file for a functional hardware might simply look like so:
```json
{
    "log_enabled": true,
    
    "driver": {
        "name": "driver_name"
    },

    "device": {
        "name": "device_name",
        "type": "functional",
        "memory_size_in_bytes": 1e9
    },

    "verification": [
        ...
    ]
}
```

## Functional Verification
Functional tests are written in the ```verification/[driver_name]/``` directory. First step is to create a test handler:
```c++
class ExampleTestHandler : public TestHandler {
public:
    bool run_test(std::string test_name, std::vector<std::string> args) override;
};
```

The ```run_test()``` function can then run different tests, based on command line arguments and test name:
```c++
bool ExampleTestHandler::run_test(std::string test_name, std::vector<std::string> args)
{
    if(test_name == "sanity")
        run_sanity(args);
    else if(test_name == "addition")
        run_addition(args);
    else
        return false;
    
    return true;    
}
```

Remember to build PRESM with this config:
```python
python scripts/build.py --config hw_configs/functional.json
```

Running functional verification can be done with a PRESM script:
```python
python scripts/verify.py --config hw_configs/functional.json
```
This script and PRESM classes work together to forward the ```name``` value from the JSON to the ```test_name``` variable, along with other command line args (```args_host_run``` when running on host, ```args_device_run``` when running on device).

Running this script outputs configs, ```.so``` library, verification executable and logs (if enabled) to ```verify_runs/``` directory. This directory will contain many subdirectories, one for each run (starting from ```verify_runs/00000/```).

## Hardware Emulation and Verification
PRESM provides scripts to load the hardware HDL onto an FPGA for emulation. Currently, we assume that the FPGA is connected to the host with a serial port.

The first step is to define an appropriate config:
```json
{
    "log_enabled": true,
    
    "driver": {
        "name": "driver_name"
    },

    "device": {
        "name": "device_name",
        "type": "serial",
        "memory_size_in_bytes": 1e9,

        "rtl": {
            "top_module_src": "tangnano20k_serial.v",
            "top_module": "Tangnano20kUart"
        },

        "rtl_testbench_config": {
            "simulator": "icarus",
            "language": "verilog"
        },

        "fpga": {
            "name": "tangnano20k"
        },

        "serial_config": {
            "baud_rate": 9600
        }
    },

    "verification": [
        ...
    ],

    "rtl_testbench": [
        {
            "name": "Command Processor (CP)",
            "rtl_src": "command_processor.v",
            "rtl_top_level": "CommandProcessor",
            "testbench_src": "command_processor.py",
            "enable": false
        }
    ]
}
```
Note that the device type is ```serial```, since we assume FPGA is connected over serial port. Note also the ```rtl```, ```rtl_testbench```, ```fpga``` and ```serial_config``` keys in ```device```. Finally, any emulation config should also have ```rtl_testbench``` tests defined like above.

Remember to build PRESM with this config:
```python
python scripts/build.py --config hw_configs/serial_fpga.json
```

Given this config, the HDL can be loaded onto an FPGA with:
```python
python scripts/fpga_load.py --config hw_configs/serial_fpga.json 
```

Verification tests with this config when run, result in communication with the FPGA instead of the functional model like above.

## Hardware Design Verification
PRESM uses [cocotb](https://www.cocotb.org/) to write testbenches for HDL code. Testbenches are located in ```rtl_testbenches/[device_name]``` directory, and look like:
```python
import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.clock import Clock

@cocotb.test()
async def uart_rx(dut):
    freq = 27000000 
    baud_rate = 9600

    clk_ns = round(1e9 / freq, 2)
    bit_hold = round(clk_ns * float(freq) / baud_rate, 2)

    dut.async_reset.value = 0
    dut.extern_uart_rx.value = 1

    clk = Clock(dut.extern_clock, clk_ns, unit='ns')
    clk.start(start_high=False)

    await RisingEdge(dut.extern_clock)
    await Timer(clk_ns, unit='ns')

    dut.async_reset.value = 1
    await Timer(clk_ns, unit='ns')
    dut.async_reset.value = 0
    await Timer(clk_ns, unit='ns')
    assert dut.data.value == 0b00000000, "UART RX reset failed"
```

Testbenches can be run with the PRESM script:
```python
python scripts/rtl_testbench_run.py --config hw_configs/functional.json
```

Running this script outputs the HDL sources, testbench sources and simulator outputs to ```rtl_testbench_runs/``` directory. This directory will contain many subdirectories, one for each run (starting from ```rtl_testbench_runs/00000/```).