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

#include <cmd.hpp>

#include <iostream>
#include <string>
#include <map>

using namespace usid;
using namespace edupals;
using namespace edupals::cmd;

using namespace std;

static void list_drivers()
{
    for (string driver : DriverFactory::drivers()) {
        cout<<"\t"<<driver<<endl;
    }
}

static void list_outputs()
{
    for (string output : OutputFactory::outputs()) {
        cout<<"\t"<<output<<endl;
    }
}

static void show_help()
{
    cout<<"usid-server COMMAND"<<endl;
    cout<<"commands:"<<endl;
    cout<<"\tlist-drivers"<<endl;
    cout<<"\tlist-outputs"<<endl;
    cout<<"\trun DRIVER OUTPUT"<<endl;
    cout<<"\noptions"<<endl;
    cout<<"\t-i/--input property=value"<<endl;
    cout<<"\t-o/--output property=value"<<endl;
}

int main (int argc,char* argv[])
{

    clog<<"Edupals userspace input driver"<<endl;
    clog<<endl;
    
    cmd::ArgumentParser parser;
    
    parser.add_option(Option('i',"input",ArgumentType::Required));
    parser.add_option(Option('o',"output",ArgumentType::Required));
    
    ParseResult result = parser.parse(argc,argv);
    
    if (result.args.size()==1) {
        show_help();
        return -1;
    }
    
    string command = result.args[1];
        
    if (command == "list-drivers") {
        list_drivers();
        return 0;
    }

    if (command == "list-outputs") {
        list_outputs();
        return 0;
    }
        
    if (command == "run") {
        if (result.args.size() < 4) {
            show_help();
            return -1;
        }
        
        map<string,string> input_properties;
        map<string,string> output_properties;
        
        for (Option& option : result.options) {
            if (option.short_name == 'i' or option.short_name == 'o') {
                
                if (option.value.size() < 3) {
                    continue;
                }
                
                string property;
                string value;
                
                size_t equal = option.value.find('=');
                if (equal == std::string::npos) {
                    continue;
                }
                
                property = option.value.substr(0,equal);
                value = option.value.substr(equal+1);
                
                if (option.short_name == 'i') {
                    input_properties[property] = value;
                }
                else {
                    output_properties[property] = value;
                }
                
            }
        }
        
        //TODO: check if exists
        Output* output = OutputFactory::create(result.args[3], output_properties);
        Driver* driver = DriverFactory::create(result.args[2], output, input_properties);
        
        
        clog<<"running..."<<endl;
    }
    
    return 0;
}

