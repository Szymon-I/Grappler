import serial
import sys
device_port = str(sys.argv[1])
speed = int(sys.argv[2])
ser = serial.Serial(device_port,speed)
while True:
    s = input()
    ser.write(s.encode())