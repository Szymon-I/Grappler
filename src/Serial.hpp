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

    // print incoming serial data (without parsing)
    void print_input()
    {
        for (int i = 0; i < bytes_received; i++)
        {
            printf("%c", buff[i]);
        }
        putchar('\n');
    }

public:
    // initialize serial communication
    char init(std::string serial_port, int speed)
    {
        this->serial_port = serial_port;
        this->speed = speed;
        this->input_monitor = input_monitor;
        char errorOpening = serial.openDevice(serial_port.c_str(), speed);
        if (errorOpening != 1)
        {
            printf("Could not connect to %s\n", serial_port.c_str());
        }
        else
        {
            printf("Successful connection to %s\n", serial_port.c_str());
        }
        return errorOpening;
    }
    // set monitor listener
    void set_monitor(bool x)
    {
        this->input_monitor = x;
    }

    // close connection at object destruction
    ~Serial()
    {
        this->serial.closeDevice();
    }

    // handle incoming uart data
    void handler()
    {
        bytes_received = serial.available();
        if (bytes_received)
        {
            serial.readString(buff, '\n', bytes_received);
            this->data_rdy = true;
            if (input_monitor)
            {
                print_input();
            }
        }
    }
    // flag for cheking if there is incoming data in seral buffer
    bool is_ready()
    {
        return this->data_rdy;
    }
    // return string of recieved data
    std::string pass_message()
    {
        std::string str_data((const char *)buff, bytes_received);
        this->data_rdy = false;
        serial.flushReceiver();
        return str_data;
    }
};