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

#include "DriverFactory.hpp"

using namespace usid;

using namespace std;

/* static factory map */
map<string,DriverFactory*> DriverFactory::factories;

DriverFactory::DriverFactory(string name) : name(name)
{
    DriverFactory::factories[name] = this;
}

DriverFactory::~DriverFactory()
{
    map<string,DriverFactory*>::iterator it = DriverFactory::factories.find(name);
    
    if (it != DriverFactory::factories.end()) {
        DriverFactory::factories.erase(it);
    }
}

vector<string> DriverFactory::drivers()
{
    vector<string> ret;
    
    for (auto q : DriverFactory::factories) {
        ret.push_back(q.first);
    }
    
    return ret;
}
