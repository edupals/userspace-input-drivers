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

using namespace usid;

using namespace std;

class Factory: public DriverFactory
{
    public:
        
    Factory() : DriverFactory("edupals.usbraw") 
    {
    }
    
    Driver* create(string device) override
    {
        return new UsbRawDriver(device);
    }
    
};

Factory factory;

UsbRawDriver::UsbRawDriver(string device) : Driver()
{
}

void UsbRawDriver::run()
{
}
