#pragma once
#include "libs/serialib.h"
#include <unistd.h>
#include <stdio.h>
#include <string>

class Serial
{
private:
    // serial settings
    std::string serial_port;
    int speed;

    // object of serial library
    serialib serial;

    // char arr buffer for receiving data
    char buff[100];
    int bytes_received;

    bool data_rdy = false;
    bool input_monitor = false;

    void print_input()
    {
        for (int i = 0; i < bytes_received; i++)
        {
            printf("%c", buff[i]);
        }
        putchar('\n');
    }

public:
    char init(std::string serial_port, int speed)
    {
        this->serial_port = serial_port;
        this->speed = speed;
        this->input_monitor = input_monitor;
        // Connection to serial port
        char errorOpening = serial.openDevice(serial_port.c_str(), speed);
        // If connection fails, return the error code otherwise, display a success message
        if (errorOpening != 1)
        {
            printf("Could not connect to %s\n", serial_port.c_str());
            return errorOpening;
        }
        printf("Successful connection to %s\n", serial_port.c_str());
        return 0;
    }
    void set_monitor(bool x)
    {
        this->input_monitor = x;
    }

    // close connection at object destruction
    ~Serial()
    {
        this->serial.closeDevice();
    }

    // handle incoming uart data and print in terminal
    void handler()
    {
        bytes_received = serial.available();
        if (bytes_received)
        {
            serial.readString(buff, '\n', bytes_received);
            // send data to grappler
            this->data_rdy = true;

            if (input_monitor)
            {
                print_input();
            }
        }
    }
    bool is_ready()
    {
        return this->data_rdy;
    }

    std::string pass_message()
    {
        std::string str_data((const char *)buff, bytes_received);
        this->data_rdy = false;
        serial.flushReceiver();
        return str_data;
    }
};