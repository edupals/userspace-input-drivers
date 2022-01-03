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
#include "driver.hpp"
#include "outputfactory.hpp"
#include "output.hpp"

#include <iostream>
#include <string>

using namespace usid;

using namespace std;

void list_drivers()
{
    for (string driver : DriverFactory::drivers()) {
        cout<<"\t"<<driver<<endl;
    }

}

void list_outputs()
{
    for (string output : OutputFactory::outputs()) {
        cout<<"\t"<<output<<endl;
    }
}

int main (int argc,char* argv[])
{

    clog<<"Edupals userspace input driver"<<endl;
    clog<<endl;
    
    if (argc>1) {
        string cmd = argv[1];
        
        if (cmd == "list-drivers") {
            list_drivers();
            return 0;
        }

        if (cmd == "list-outputs") {
            list_outputs();
            return 0;
        }
        
        if (cmd == "run") {
            if (argc<4) {
                cerr<<"Usage: run DRIVER DEVICE_PATH"<<endl;
                return -1;
            }
            
            Driver* driver = DriverFactory::create(argv[3]);
            
            driver->run();
        }
    }
    
    return 0;
}

