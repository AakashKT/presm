# Preinception

<b>Preinception</b> is a simple compute accelerator. Simple since it only supports four ```int32``` operations:
- Add
- Subtract
- Multiply by powers of 2: ```a * 2^b```
- Divide by powers of 2: ```a / 2^b```

Why call it a "compute accelerator" and not "ALU"? Three reasons:
- A host PC can interact with it via a USB connection and a [C++ API](https://github.com/AakashKT/presm/blob/main/driver/matrix/include/matrix.h). This is the driver (named matrix), and is how commands are sent to the device.
- It is meant to be a supplement to the main host CPU - in the same way GPUs are. Hence the term "accelerator", although it is unlikely that operations are faster than the host CPU!
- The values for the above operations are fetched from the host PC memory. Furthermore, the result of these operations is also written to the host PC memory. The coordination from memory fetch and memory write is done by the [C++ API](https://github.com/AakashKT/presm/blob/main/driver/matrix/include/matrix.h).

The name "Preinception" is so since its a proof-of-concept release using the [PRESM](https://github.com/AakashKT/presm) pre-silicon modelling & testing framework. Its not the "inception", but rather a stepping stone towards it.

The PRESM configuration files are located in ```hw_configs/rpeincpetion/```.

Three configurations are possible:
- Functional (```functional.json```): A purely functional implementation, meant to test the driver (C++ API) and define the hardware architecture.
- FPGA (```serial_fpga_tangnano20k.json```): For end-to-end driver & RTL validation, using the Tang Nano 20k FPGA connected via USB. 
- Tapeout Chip (```serial_tapeout_tt_ihp26b.json```): Release interface with a tapeout chip, connected via USB.

## Architecture
```
+------------------------------------------------+
|               driver (C++ API)                 | 
+------------------------------------------------+
        |           (Host)          ^
        |                           |    
--------|------[ 48-bit packet ]----|-------------
        |                           |
        Y           (Device)        |
+------------------------------------------------+
|                   UARTPacket                   |
|     +-------------+           +-------------+  |
|     |   UARTRx    |           |   UARTTx    |  |
|     +-------------+           +-------------+  |
|                                                |
+------------------------------------------------+
        |                           ^
        |                           |
        Y                           |
+------------------------------------------------+
|                   Command                      |
|                  Processor                     |
|                     (CP)                       |
+------------------------------------------------+
```

#### Host-Device packet structure (48-bit packet)
```
+-----------------------------------------------+-------------------+
|            16-bit header                      |   32-bit body     |
++----------------------------------------------+-------------------+
|| 4-bit     | 4-bit   | 4-bit   | 4-bit       ||   32-bits data   ||
|| Unique ID | Type    | Command | Sub-command ||                  ||
++--------------------------------------------------+---------------+
```

## Testing with FPGA
Clone [PRESM](https://github.com/AakashKT/presm) and navigate to the cloned directory.

Build the FPGA config for Preinception:
```python
python scripts/build.py --config hw_configs/preinception/serial_fpga_tangnano20k.json --get_extern_tools
```
The ```--get_extern_tools``` flag can be ignored for the next run - it is only required the first time to setup the FPGA tools.

Next, connect a Tang Nano 20k to your PC with a USB cable. You may also need to run the following command:
```
sudo modprobe ftdi_sio
```

Finally, run verification apps:
```python
python scripts/verify.py --config hw_configs/preinception/serial_fpga_tangnano20k.json
```
You should see an output like so:
```bash
++ Copying files from /home/aakashkt/presm/build/verification/ to verify_runs/00491
++ Copying file /home/aakashkt/presm/build/driver/matrix/libmatrix_serial.so to verify_runs/00491
++ Copying file hw_configs/preinception/serial_fpga_tangnano20k.json to verify_runs/00491/hw_config.json
++ ======================
++ PRESM Execution Begin
++ ======================
++ Changing working directory to: /home/aakashkt/presm/verify_runs/00491
++ Executing: ./verification sanity device
++ Environment:
{'LD_PRELOAD': 'libmatrix_serial.so'}
++ Changing working directory to: /home/aakashkt/presm
++ ======================
++ PRESM Execution End
++ ======================

++ Changing working directory to: /home/aakashkt/presm/verify_runs/00491
++ Executing: ./verification sanity host
++ Changing working directory to: /home/aakashkt/presm

++ Verification of "sanity" succeeded.

++ ======================
++ PRESM Execution Begin
++ ======================
++ Changing working directory to: /home/aakashkt/presm/verify_runs/00491
++ Executing: ./verification addition device
++ Environment:
{'LD_PRELOAD': 'libmatrix_serial.so'}
++ Changing working directory to: /home/aakashkt/presm
++ ======================
++ PRESM Execution End
++ ======================

++ Changing working directory to: /home/aakashkt/presm/verify_runs/00491
++ Executing: ./verification addition host
++ Changing working directory to: /home/aakashkt/presm

++ Verification of "addition" succeeded.

.....
.....
```

Ensure all verifications succeed. 

## Testing the tapeout (Tiny Tapeout with Demo Board)
Clone [PRESM](https://github.com/AakashKT/presm) and navigate to the cloned directory.

Build the Tiny Tapeout config for Preinception:
```python
python scripts/build.py --config hw_configs/preinception/serial_tapeout_tt_ihp26b.json
```

Next, we need to connect the chip to the PC. You may also need to run the following command:
```
sudo modprobe ftdi_sio
```

The chip can be connected in two ways:
- Connect the demo board to the PC with USB
- With an exteral UART module attached to the demo board, and the UART module connected to the PC with USB

As of now, not sure which one will work, so give both a go when chips become available.

### Direct Demo Board
This is straightforward. Testing is done similarly to the above section by running verification apps.
```python
python scripts/verify.py --config hw_configs/preinception/serial_tapeout_tt_ihp26b.json
```
Again, check that all apps succeed verification.

### External UART module
On the demo board:
- Connect output pin 6 ```uo[6]``` (```PIN_HI``` in datasheet) to input pin 0 ```ui[0]``` (```SEL_RX_TX``` in datasheet).
  - This changes the UART RX and TX pins from the default to two other free pins
- Next, connect the external UART module
  - Connect TX to input pin 7 ```ui[7]``` (```RX_secondary``` in datasheet)
  - Connect RX to input pin 7 ```uo[7]``` (```TX_secondary``` in datasheet)

Now we can run the verification apps like above.

## Using the chip with your own code
Make sure you have built Preinception with the tapeout configuration.
```python
python scripts/build.py --config hw_configs/preinception/serial_tapeout_tt_ihp26b.json
```

The build creates a package of the driver in ```packages/```, which looks like:
```bash
├── packages/
│   ├── matrix/
│   |   ├── include/
|   |   |   ├── matrix.h # Driver API header
|   |   ├── lib/
|   |   |   ├── libmatrix_serial.so # Driver library
```

Lets compile a simple C++ code that uses the driver to add two numbers on preinception.
```c++
#include "include/matrix.h"
#include <iostream>

int main()
{
    mInit(); // Initialize the driver
    
    // Allocate memory for device, and initialize it
    MIntDeviceMemory a(-64); 
    MIntDeviceMemory b(-80);
    MIntDeviceMemory c(10);

    mAdd(a, b, c); // Call add on 'a' and 'b', store in 'c'
    mSync(); // Wait for device to finish

    std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;

    mFree(); // Free driver resources
}
```

Compilation needs to link with the driver library:
```bash
g++ main.cpp -L./lib/ -lmatrix_serial -o main -Wl,-rpath,./lib
```

Running the program:
```bash
$ ./main
-64 + -80 = -144
```

### Failure cases
- The driver currently assumes that the device shows up in one of these: ```/dev/ttyUSB0``` --> ```/dev/ttyUSB4```
  - Essentially, it searches for and does a handshake protocol for devices from index 0 to 4. 
  - If the device shows up in another name or after index 4, the driver will quit with an error.