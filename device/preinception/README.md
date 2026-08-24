# Device: Preinception

## Seirial device architecture
Configs: serial_tangnano20k.json

```
+------------------------------------------------+
| Host Driver + Device Memory (class SerialImpl) |
+------------------------------------------------+
        |           (Host)          ^
--------|---------------------------|-------------
        |           (Device)        |
        Y                           |
+---------------------+    +---------------------+
| UART RX             |    | UART TX             |
+---------------------+    +---------------------+
  |                          ^
  |      ____________________|
  Y      |
+----------------+
| Command        |
| Processor (CP) |
+----------------+
```

## General RTL conventions
- Output is always register in a module
- A wire is connected to a module's output
- Correspondingly, input is always a wire in a module
- Input to a module from the outside should be a register