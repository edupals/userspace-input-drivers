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

#include "UsbRawDriver.hpp"
#include "DriverFactory.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <exception>

using namespace usid;

using namespace std;

class UsbRawFactory: public DriverFactory
{
    public:
        
    UsbRawFactory() : DriverFactory("edupals.usb.raw") 
    {
    }
    
    Driver* create(string device) override
    {
        return new UsbRawDriver(device);
    }
    
};

static UsbRawFactory factory;

UsbRawDriver::UsbRawDriver(string device) : Driver(device)
{
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
}

UsbRawDriver::~UsbRawDriver()
{
    libusb_exit(usb_context);
}

void UsbRawDriver::run()
{
    clog<<"Running USB Raw driver on "<<device<<endl;
    
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
    
    while (true) {
        status = libusb_interrupt_transfer(usb_handle,(2 | LIBUSB_ENDPOINT_IN),buffer,64,&length,1000);
        
        if (status!=0) {
            clog<<"read error:"<<status<<" reason:"<<errcodes[status]<<endl;
            continue;
        }
        clog<<"read:"<<length<<endl;
        for (int n=0;n<length;n++){
            clog<<std::hex<<(int)buffer[n]<<" ";
        }
        clog<<endl;
    }
}
