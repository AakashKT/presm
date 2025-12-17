# Exposing FPGA attached to Windows on WSL
Assuming the FPGA is connected via USB, follow these steps.

Install [USBPID](https://github.com/dorssel/usbipd-win/releases) on windows.

Open powershell as administrator, and run the following:
```
>>> usbipd list
Connected:
BUSID  VID:PID    DEVICE                                                        STATE
1-1    0403:6010  USB Serial Converter A, USB Serial Converter B                Shared
1-2    25a7:fa61  USB Input Device                                              Not shared
1-5    04f3:0c90  ELAN WBF Fingerprint Sensor                                   Not shared
1-7    13d3:5463  USB2.0 HD UVC WebCam, Camera DFU Device                       Not shared
1-10   8087:0033  Intel(R) Wireless Bluetooth(R)                                Not shared
```

Select the appropriate BUSID from the above (for me it's ```1-1```) and bind it:
```
>>> usbipd bind --busid 1-1
```

Finally, expose to WSL:
```
>>> usbipd attach --wsl --busid 1-1
```

You can check if the device was successfully exposed by running the following in WSL:
```
>>> lsusb
```