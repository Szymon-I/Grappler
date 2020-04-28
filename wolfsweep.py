import serial
import sys
device_port = str(sys.argv[1])
speed = int(sys.argv[2])
xyz = '0.0/0.0/0.0'
print('Use wasd keys to move')
while True:
    with serial.Serial(device_port, speed) as ser:
        s = input()
        if s == 'a':
            xyz = '1.0/0.0/0.0'
        elif s == 'w':
            xyz = '0.0/0.0/1.0'
        elif s == 'd':
            xyz = '-1.0/0.0/0.0'
        else:
            xyz = '0.0/0.0/-1.0'

        ser.write(xyz.encode())
