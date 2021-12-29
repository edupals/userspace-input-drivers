/*
    User space input drivers

    Copyright (C) 2021  Enrique Medina Gremaldos <quiqueiii@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "serialmulticlassdriver.hpp"
#include "driverfactory.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace usid;

using namespace std;

class SerialMulticlassFactory: public DriverFactory
{
    public:
        
    SerialMulticlassFactory() : DriverFactory("edupals.serial.multiclass") 
    {
    }
    
    Driver* create(string device) override
    {
        return new SerialMulticlassDriver(device);
    }
    
};

static SerialMulticlassFactory factory;

SerialMulticlassDriver::SerialMulticlassDriver(string device): Driver(device)
{
    struct termios options;
    
    // old copy-pasted code, needs a review
    serial_fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    fcntl(serial_fd, F_SETFL, FNDELAY); //set non-blocking reads
    
    tcgetattr(serial_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    cfmakeraw(&options);
    tcsetattr(serial_fd, TCSANOW, &options);
    
    // device needs one second before start i/o
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // send init cmd
    uint8_t data = Command::Init;
    write(serial_fd,&data,1);
}

SerialMulticlassDriver::~SerialMulticlassDriver()
{
}

void SerialMulticlassDriver::run()
{
    uint8_t buffer[16];
    uint8_t ptr=0;
    uint8_t checksum;
    int status;
    
    while (true) {
        status = read(serial_fd,&buffer[ptr],1);
        
        if (status>0) {
            
            // package is filled
            if (ptr==7) {
                
                if (buffer[0] == Command::Welcome) {
                    // do what?
                }
                
                if(buffer[0] == Command::Packet and buffer[1] == Command::Packet) {
                    
                    checksum = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5] ^ buffer[6];
                    
                    if (checksum == buffer[7]) {
                        uint16_t x = (buffer[3]<<6) | buffer[4];
                        uint16_t y = (buffer[5]<<6) | buffer[6];
                        uint8_t button = buffer[2];
                    }
                }
                
                ptr=0;
            }
            else {
                ptr++;
            }
        }
        else {
            //TODO: send keep alive if there is a 1 second gap
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
