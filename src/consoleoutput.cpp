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

#include "consoleoutput.hpp"
#include "outputfactory.hpp"

#include <iostream>

using namespace usid;

using namespace std;

static Output* create(map<string,string> properties)
{
    return new ConsoleOutput(properties);
}

static OutputFactory factory("edupals.output.console",create);

ConsoleOutput::ConsoleOutput(map<string,string> properties) : Output(properties)
{
}

ConsoleOutput::~ConsoleOutput()
{
}

void ConsoleOutput::push(uint16_t type,uint16_t code,int32_t value)
{
    cout<<"axis "<<std::dec<<type<<":"<<code<<":"<<value<<endl;
}

void ConsoleOutput::sync()
{
    clog<<"sync\n"<<endl;
}
