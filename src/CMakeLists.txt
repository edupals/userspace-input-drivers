
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )

find_package(PkgConfig)
pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)

add_executable(usid-server Driver.cpp DriverFactory.cpp UsbRawDriver.cpp SerialMulticlassDriver.cpp main.cpp)
target_link_libraries(usid-server PkgConfig::LIBUSB)
