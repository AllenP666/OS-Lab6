#!/bin/bash

kill_processes() {
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        # Windows (MSYS2)
        taskkill //F //IM weather_server.exe > /dev/null 2>&1
        taskkill //F //IM sensor_simulator.exe > /dev/null 2>&1
    else
        # Linux
        pkill -f weather_server
        pkill -f sensor_simulator
    fi
}

kill_processes

echo "Starting Simulator..."
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    ./build/sensor_simulator.exe & 
else
    ./build/sensor_simulator & 
fi

echo "Starting Server..."
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    ./build/weather_server.exe virtual_com & 
else
    ./build/weather_server virtual_com &
fi

echo "Waiting for server..."
sleep 2

echo "Starting GUI..."
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    ./build/gui_client/weather_gui.exe
else
    ./build/gui_client/weather_gui
fi

echo "Stopping background processes..."
kill_processes
