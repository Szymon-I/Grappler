import serial
device_port = '/dev/pts/4'
speed = 115200
ser = serial.Serial(device_port,speed)
while True:
    s = input()
    ser.write(s.encode())