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

public:
    char init(std::string serial_port, int speed)
    {
        this->serial_port = serial_port;
        this->speed = speed;
        // Connection to serial port
        char errorOpening = serial.openDevice(serial_port.c_str(), speed);
        // If connection fails, return the error code otherwise, display a success message
        if (errorOpening != 1)
            return errorOpening;
        printf("Successful connection to %s\n", serial_port.c_str());
        return 0;
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
            for (int i = 0; i < bytes_received; i++)
            {
                printf("%c", buff[i]);
            }
            putchar('\n');
            serial.flushReceiver();
        }
        
    }
};