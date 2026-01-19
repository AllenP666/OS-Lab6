#include <iostream>
#include <thread>
#include <string>
#include "serial_port.hpp"
#include "database.hpp"
#include "http_server.hpp"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    std::string default_port = "COM3";
#else
    std::string default_port = "virtual_com";
#endif
    std::string port_name = (argc > 1) ? argv[1] : default_port;

    Database db("weather.db");
    HttpServer server(8080);

    std::thread server_thread([&]() {
        server.start(db);
    });

    SerialPort sp(port_name);
    if (!sp.isOpen()) {
        std::cerr << "Failed to open port" << std::endl;
        return 1;
    }

    while (true) {
        std::string data;
        if (sp.readLine(data) && !data.empty()) {
            try {
                double temp = std::stod(data);
                db.insert_temp(temp);
                std::cout << "Logged: " << temp << std::endl;
            } catch (...) {}
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    if (server_thread.joinable()) server_thread.join();
    return 0;
}