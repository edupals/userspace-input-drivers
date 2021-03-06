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

#ifndef USID_WALTOP_DRIVER
#define USID_WALTOP_DRIVER

#include "driver.hpp"

#include <libusb-1.0/libusb.h>

#include <string>

namespace usid
{
    class WaltopDriver: public Driver
    {
        public:
        
        WaltopDriver(Output* output, std::map<std::string,std::string> properties);
        ~WaltopDriver();
        
        void run() override;
        
        protected:
        std::string device;
        bool debug;
        int endpoint;
        
        libusb_context* usb_context;
        libusb_device_handle* usb_handle;
        libusb_device* usb_device;
        int usb_fd;
    };
}

#endif
