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

#ifndef USID_OUTPUT
#define USID_OUTPUT

#include <linux/input.h>

#include <string>
#include <vector>
#include <unordered_set>
#include <map>

namespace usid
{
    struct AbsConfig
    {
        int32_t min;
        int32_t max;
        
    };
    
    class OutputConfig
    {
        public:
        
        std::string name;
        uint16_t vendor;
        uint16_t product;
        
        std::unordered_set<uint32_t> cfg;
        std::unordered_set<uint32_t> axes;
        std::map<uint32_t,AbsConfig> axis_cfg;
        std::unordered_set<uint32_t> keys;
        
        OutputConfig(std::string name, uint16_t vendor=0x00, uint16_t product=0x00);

        void add_absolute(uint32_t axis, int32_t min, int32_t max);
        void add_relative(uint32_t axis);
        void add_buttons(std::vector<uint32_t> buttons);
        void add_keys(std::vector<uint32_t> keys);
        
    };
    
    class Output
    {
        public:
        
        Output();
        virtual ~Output();
        
        virtual void push(uint16_t type,uint16_t code,int32_t value) = 0;
        virtual void sync() = 0;
    };
}

#endif
