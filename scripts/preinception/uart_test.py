import serial, sys, time

s = serial.Serial(f'/dev/ttyUSB1', 115200)

data = bytes([1, 0, 1, 0, 0, 0, 0, 0])
s.write(data)
print(s.read(8))
time.sleep(1)

data = bytes([2, 0, 2, 0, 0, 0, 0, 1])
s.write(data)
print(s.read(8))
time.sleep(1)

data = bytes([1, 1, 0, 0, 0, 0, 0, 2])
s.write(data)
print(s.read(8))
time.sleep(1)

data = bytes([3, 0, 2, 1, 0, 0, 0, 2])
s.write(data)
print(s.read(8))
time.sleep(1)

data = bytes([2, 1, 0, 0, 0, 0, 0, 3])
s.write(data)
print(s.read(8))
time.sleep(1)

data = bytes([4, 0, 2, 2, 0, 0, 0, 3])
s.write(data)
print(s.read(8))
print(s.read(8))
time.sleep(1)

data = bytes([3, 1, 0, 0, 0, 0, 0, 0])
s.write(data)