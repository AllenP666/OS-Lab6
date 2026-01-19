cmake_minimum_required(VERSION 3.10)
project(WeatherNetworkServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(weather_server 
    main.cpp 
    sqlite3.c
)
configure_file(index.html index.html COPYONLY)

if(WIN32)
    target_link_libraries(weather_server ws2_32)
else()
    target_link_libraries(weather_server pthread dl)
endif()

add_executable(sensor_simulator simulator.cpp)

find_package(Qt5 COMPONENTS Widgets Network Charts QUIET)
if (Qt5_FOUND)
    message(STATUS "Qt5 found, building GUI Client...")
    add_subdirectory(gui_client)
else()
    find_package(Qt6 COMPONENTS Widgets Network Charts QUIET)
    if (Qt6_FOUND)
        message(STATUS "Qt6 found, building GUI Client...")
        add_subdirectory(gui_client)
    else()
        message(WARNING "Qt not found! GUI Client will not be built. Install Qt5 or Qt6.")
    endif()
endif()