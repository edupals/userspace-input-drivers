
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )

add_executable(usid-server Driver.cpp DriverFactory.cpp UsbRawDriver.cpp main.cpp)
