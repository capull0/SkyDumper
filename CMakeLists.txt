cmake_minimum_required(VERSION 3.5)

project(SkyDumper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(libusb-1.0 REQUIRED)

include_directories(include ${LIBUSB_1_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp skylander.cpp include/skylander.h portalio.cpp include/portalio.h include/hidapi.h)

if (UNIX AND NOT APPLE)
    set(SOURCE_FILES ${SOURCE_FILES} hid_linux.c)
    set(SL_LIBS ${LIBUSB_1_LIBRARIES} pthread)
elseif(MINGW)
    set(SOURCE_FILES ${SOURCE_FILES} hid_win.c)
    set(SL_LIBS ${LIBUSB_1_LIBRARIES} pthread)
elseif(APPLE)
    set(SOURCE_FILES ${SOURCE_FILES} hid.c)
    set(SL_LIBS ${LIBUSB_1_LIBRARIES} "-framework CoreFoundation" "-framework IOKit")
else(UNIX)
    message(FATAL_ERROR "Not supported platform")
endif(UNIX AND NOT APPLE)

add_executable(SkyDumper ${SOURCE_FILES})
target_link_libraries(SkyDumper ${SL_LIBS})
