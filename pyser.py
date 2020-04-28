import serial
import sys
device_port = str(sys.argv[1])
speed = int(sys.argv[2])

while True:
    with serial.Serial(device_port,speed) as ser:
        s = input()
        ser.write(s.encode())