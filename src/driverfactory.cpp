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

#include "driverfactory.hpp"

using namespace usid;

using namespace std;

map<string,DriverFactory*> DriverFactory::factories;

DriverFactory::DriverFactory(string name,std::function<Driver*()> creator) : creator(creator)
{
    DriverFactory::factories[name] = this;
}

Driver* DriverFactory::create(string name)
{
    DriverFactory* target = DriverFactory::factories[name];

    return target->creator();
}

vector<string> DriverFactory::drivers()
{
    vector<string> ret;

    for (auto q: DriverFactory::factories) {
        ret.push_back(q.first);
    }

    return ret;
}
