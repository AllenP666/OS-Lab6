#!/bin/bash

taskkill //F //IM weather_server.exe > /dev/null 2>&1
taskkill //F //IM sensor_simulator.exe > /dev/null 2>&1

echo "Starting Simulator..."
./build/sensor_simulator.exe & 

echo "Starting Server..."
./build/weather_server.exe virtual_com & 

echo "Waiting for server..."
sleep 2

echo "Starting GUI..."
./build/gui_client/weather_gui.exe

echo "Stopping background processes..."
taskkill //F //IM weather_server.exe > /dev/null 2>&1
taskkill //F //IM sensor_simulator.exe > /dev/null 2>&1