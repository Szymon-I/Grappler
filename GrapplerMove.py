import serial
import sys
import time
import math
import numpy as np


class GrapplerMove:
    normalizer = 1000.0
    delay = 0.05

    def __init__(self):
        self.device_port = str(sys.argv[1])
        self.speed = int(sys.argv[2])
        self.buffer = ''
        self.y_pos = 0

    def show_menu(self):
        print('This program imitates grappler movement without hardware')
        print('press key to make movement:')
        print('q - exit')
        print('1 - line movement')
        print('2 - square movement')
        print('3 - wave movement')
        print('w - move forward (once)')
        print('s - move back (once)')
        print('a - move left (once)')
        print('d - move right (once)')
        print('+ - move up (once)')
        print('- - move down (once)')

    def line_movement(self, line_length=20):
        data = [
            f'0.0/{GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length)]
        data += [
            f'0.0/{-GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length)]
        self.send_data(data)

    def square_movement(self, line_length=20):
        data = [
            f'0.0/{GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length//2)]
        data += [f'{+GrapplerMove.normalizer}/0.0/{self.y_pos}' for _ in range(
            line_length//2)]
        data += [
            f'0.0/{-GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length)]
        data += [
            f'{-GrapplerMove.normalizer}/0.0/{self.y_pos}' for _ in range(line_length)]
        data += [
            f'0.0/{GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length)]
        data += [f'{GrapplerMove.normalizer}/0.0/{self.y_pos}' for _ in range(
            line_length//2)]
        data += [
            f'0.0/{-GrapplerMove.normalizer}/{self.y_pos}' for _ in range(line_length//2)]

        self.send_data(data)

    def wave_movement(self, line_length=50):
        sin_arg = np.linspace(0, 2*math.pi, line_length)
        data = [
            f'{math.sin(sin_arg[x])*GrapplerMove.normalizer}/{GrapplerMove.normalizer}/{self.y_pos}' for x in range(line_length)]
        data += [
            f'{-math.sin(sin_arg[x])*GrapplerMove.normalizer}/{-GrapplerMove.normalizer}/{self.y_pos}' for x in range(line_length)]
        self.send_data(data)

    def move_once(self, direction):
        data = None
        if direction == 'w':
            data = [f'0.0/{GrapplerMove.normalizer}/{self.y_pos}']
        elif direction == 's':
            data = [f'0.0/{-GrapplerMove.normalizer}/{self.y_pos}']
        elif direction == 'a':
            data = [f'{GrapplerMove.normalizer}/0.0/{self.y_pos}']
        elif direction == 'd':
            data = [f'{-GrapplerMove.normalizer}/0.0/{self.y_pos}']
        self.send_data(data)

    def move_vert(self, abs_value):
        self.y_pos += abs_value
        data = [f'0.0/0.0/{self.y_pos}']
        self.send_data(data)

    def send_data(self, data, monitor=False):
        with serial.Serial(self.device_port, self.speed) as ser:
            for packet in data:
                if monitor:
                    print(packet)
                ser.write(packet.encode())
                time.sleep(GrapplerMove.delay)

    def run(self):
        option = None
        while option != 'q':
            self.show_menu()
            option = input()
            if option == '1':
                self.line_movement()
            elif option == '2':
                self.square_movement()
            elif option == '3':
                self.wave_movement()
            elif option in ['w', 'a', 's', 'd']:
                self.move_once(option)
            elif option in ['+', '-']:
                value = GrapplerMove.normalizer/10
                self.move_vert(value if option == '+' else -value)


if __name__ == "__main__":
    grappler = GrapplerMove()
    grappler.run()
