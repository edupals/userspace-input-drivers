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

#ifndef USID_SERIAL_MULTICLASS
#define USID_SERIAL_MULTICLASS

#include "driver.hpp"

namespace usid
{
    enum Command : uint8_t
    {
        Packet=0xAA,
        Welcome=0xA8,
        Init=0xCA,
        KeepAlive=0xCC
    };
    
    class SerialMulticlassDriver: public Driver
    {
        public:
        
        SerialMulticlassDriver(std::string device);
        ~SerialMulticlassDriver();
        
        void run() override;
        
        protected:
        
        int serial_fd;
    };
}

#endif