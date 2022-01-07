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

#include "output.hpp"

#include <cstring>

using namespace usid;

using namespace std;

OutputConfig::OutputConfig(string name, uint16_t vendor, uint16_t product) : name(name), vendor(vendor), product(product) 
{
    //std::memset(&dev, 0, sizeof(struct uinput_user_dev));
}

void OutputConfig::add_absolute(uint32_t axis, int32_t min, int32_t max)
{
    cfg.insert(EV_ABS);
    
    axes.insert(axis);
    
    AbsConfig cfg;
    
    cfg.min = min;
    cfg.max = max;
    
    axis_cfg[axis]=cfg;
    
}

void OutputConfig::add_relative(uint32_t axis)
{
    cfg.insert(EV_REL);
    axes.insert(axis);
}

void OutputConfig::add_buttons(vector<uint32_t> buttons)
{
    cfg.insert(EV_KEY);
    keys.insert(buttons.begin(),buttons.end());
}

void OutputConfig::add_keys(vector<uint32_t> keys)
{
    cfg.insert(EV_KEY);
    this->keys.insert(keys.begin(),keys.end());
}

void Output::set_config(OutputConfig* config)
{
    this->config = config;
}
