# Device: Preinception
Configuration files are located in: 
```
configs/hw/preinception/
```

## Seirial device architecture
Configuration file: ```serial_tangnano20k.json```

### Architecture
```
+------------------------------------------------+
|                Device Interface                | 
+------------------------------------------------+
        |           (Host)          ^
--------|------[ 64-bit packet ]----|-------------
        |           (Device)        |
        Y                           |
+------------------------------------------------+
|                   UARTPacket                   |
|     +-------------+           +-------------+  |
|     |   UARTRx    |           |   UARTTx    |  |
|     +-------------+           +-------------+  |
|                                                |
+------------------------------------------------+
  |      ^
  |      |
  Y      |
+----------------+
| Command        |
| Processor (CP) |
+----------------+
```

## Host-Device packet structure
```
+---------------------------------------------------+---------------+
|            32-bit header                          | 32-bit body   |
++--------------------------------------------------+---------------+
|| 8-bit     | 8-bit   | 8-bit       | Body-size   || 32-bits data ||
|| Unique ID | Command | Sub-command | 8-bit chunks||              ||
++--------------------------------------------------+---------------+
```

### Commands & sub-commands

#### Command = 1, Sub-command = [ANY_VALUE] 
Handshake request, under which the device returns a identity packet. 

## General RTL conventions
- Output is always register in a module
- A wire is connected to a module's output
- Correspondingly, input is always a wire in a module
- Input to a module from the outside should be a register