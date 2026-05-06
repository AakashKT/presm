# General structure of a PRESM configuration
The PRESM codebase works with configurations defined in JSON files. A typical PRESM config must follow the following template and must contain the following keys:
```
{
    "driver": {
        "name": "DRIVER_NAME" 
    },

    "device": {
        "name": "DEVICE_NAME",
        "type": "DEVICE_TYPE"
    }
}
```
This minimal config allows you to build PRESM for the specified device/driver combination. Please note, PRESM will look for a ```DRIVER_NAME``` folder under ```drivers/``` and similarly a ```DEVICE_NAME``` folder under ```devices/```.

## RTL testbench configuration
This type of configuration defines testbenches and their sources for RTL code. Defining an RTL testbench config requires the following definitions in ```device```, along with a list of testbenches:
```
{
    "driver": {
        "name": "DRIVER_NAME" 
    },

    "device": {
        "name": "DEVICE_NAME",
        "type": "rtl_testbench"

        "rtl_testbench": {
            "simulator": "icarus",
            "language": "verilog"
        }
    },

    "rtl_testbench": [
        {
            "name": "TEST_NAME",
            "rtl_src": "SOURCE",
            "rtl_top_level": "TOP_MODULE_NAME",
            "testbench_src": "PYTHON_TESTBENCH",
            "enable": true
        }
    ]
}
```

PRESM will look for ```rtl_src``` file in ```device/DEVICE_NAME/rtl``` directory - make sure you put all your RTL files there! 

Testbenches are written using ```cocotb``` in python, and similar to the above, PRESM looks for those python files under ```rtl_testbench/DEVICE_NAME/``` directory.

A RTL testbench configuration file is consumed by the ```scripts/rtl_testbench.py``` script. 

Finally, PRESM ships with a testbench file in ```configs/presm_rtl_testbench.json``` that defines testbenches for PRESM provided RTL modules.

## Device configuration
