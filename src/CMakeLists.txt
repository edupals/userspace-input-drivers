
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )

find_package(PkgConfig)
pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)

find_package(EdupalsBase REQUIRED)

add_executable(usid-server driver.cpp driverfactory.cpp usbrawdriver.cpp serialmulticlassdriver.cpp output.cpp outputfactory.cpp consoleoutput.cpp uinputoutput.cpp main.cpp)
target_link_libraries(usid-server PkgConfig::LIBUSB Edupals::Base)
