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

#ifndef USID_DRIVER_FACTORY
#define USID_DRIVER_FACTORY

#include "driver.hpp"

#include <string>
#include <map>
#include <vector>
#include <functional>

namespace usid
{
    class DriverFactory
    {
        public:

        DriverFactory(std::string name,std::function<Driver*(Output*, std::map<std::string,std::string>)> creator);

        static Driver* create(std::string name, Output* output, std::map<std::string,std::string> properties);

        static std::vector<std::string> drivers();

        protected:
        std::function<Driver*(Output*, std::map<std::string,std::string>)> creator;

        static std::map<std::string,DriverFactory*> factories;
        
    };
}

#endif
