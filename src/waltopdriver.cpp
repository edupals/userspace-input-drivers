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

#include "waltopdriver.hpp"
#include "driverfactory.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <exception>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace usid;

using namespace std;

static Driver* create(Output* output, map<string,string> properties)
{
    return new WaltopDriver(output, properties);
}

static DriverFactory factory("edupals.driver.waltop",create);

WaltopDriver::WaltopDriver(Output* output, map<string,string> properties) : Driver(output,properties)
{
    
    if (properties.find("device") == properties.end()) {
        throw runtime_error("Missing device property");
    }
    
    device = properties["device"];
    
    int status;
    
    if (libusb_init(&usb_context) < 0) {
        throw runtime_error("Failed to init libusb");
    }
    
    usb_fd = open(device.c_str(),O_RDWR);
    
    status = libusb_wrap_sys_device(usb_context, usb_fd, &usb_handle);
    
    clog<<"fd:"<<usb_fd<<endl;
    clog<<"status:"<<status<<endl;
    
    if (status!=0) {
        throw runtime_error("Failed to open device:"+std::to_string(status));
    }
    
    if (libusb_kernel_driver_active(usb_handle, 0) == 1) {
        libusb_detach_kernel_driver(usb_handle, 0);
    }
    
    libusb_claim_interface(usb_handle, 0);
    
    OutputConfig* config = new OutputConfig("usid waltop driver",0x172f,0x0037);
    
    config->add_absolute(ABS_X,0,12288);
    config->add_absolute(ABS_Y,0,9612);
    config->add_absolute(ABS_Z,0,1024);
    
    config->add_buttons({BTN_LEFT,BTN_RIGHT});

    output->start(config);
}

WaltopDriver::~WaltopDriver()
{
    libusb_exit(usb_context);
}

void WaltopDriver::run()
{
    
    clog<<"Running Waltop driver on "<<device<<endl;
    
    map<int,string> errcodes;
    
    errcodes[LIBUSB_ERROR_TIMEOUT] = "timeout";
    errcodes[LIBUSB_ERROR_PIPE] = "pipe";
    errcodes[LIBUSB_ERROR_OVERFLOW] = "overflow";
    errcodes[LIBUSB_ERROR_NO_DEVICE] = "no device";
    errcodes[LIBUSB_ERROR_BUSY] = "busy";
    errcodes[LIBUSB_ERROR_INVALID_PARAM] = "invalid param";
    
    int status;
    uint8_t buffer[64];
    int length;
    
    buffer[0] = 2;
    buffer[1] = 1;

    status = libusb_control_transfer(usb_handle,
                            LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
                            0x09/*HID set_report*/,
                            (3/*HID feature*/ << 8) | buffer[0],
                            0,
                            buffer, 2,
                            1000/*timeout millis*/);
    
    if (status<0) {
        clog<<"set feature report error:"<<status<<" reason:"<<errcodes[status]<<endl;
    }

    while (true) {
        status = libusb_interrupt_transfer(usb_handle,(2 | LIBUSB_ENDPOINT_IN),buffer,64,&length,1000);
        
        if (status!=0 and status!=LIBUSB_ERROR_TIMEOUT) {
            clog<<"read error:"<<status<<" reason:"<<errcodes[status]<<endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        
        if (buffer[0] == 16) {
            int mx,my,mz;

            int tip = buffer[1] & 0x01;
            int barrel = (buffer[1] & 0x02)>>1;

            mx = (int)(buffer[2]+(buffer[3]<<8));
            my = (int)(buffer[4]+(buffer[5]<<8));
            mz = (int)(buffer[6]+(buffer[7]<<8));
            
            output->push(EV_ABS, ABS_X, mx);
            output->push(EV_ABS, ABS_Y, my);
            output->push(EV_ABS, ABS_Z, mz);
            
            output->push(EV_KEY, BTN_LEFT, tip);
            output->push(EV_KEY, BTN_RIGHT, barrel);

            output->sync();
        }
        
        
    }
}
