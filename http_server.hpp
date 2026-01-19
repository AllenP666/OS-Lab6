#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#include "database.hpp"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

class HttpServer {
public:
    HttpServer(int port) : port(port) {}

    void start(Database& db) {
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        bind(server_fd, (struct sockaddr*)&address, sizeof(address));
        listen(server_fd, 3);

        while (true) {
            int new_socket = accept(server_fd, nullptr, nullptr);
            if (new_socket < 0) continue;

            char buffer[2048] = {0};
#ifdef _WIN32
            recv(new_socket, buffer, 2048, 0);
#else
            read(new_socket, buffer, 2048);
#endif
            std::string request(buffer);
            std::string response;
            std::string header = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n";

            if (request.find("GET /api/current") != std::string::npos) {
                std::string content = db.get_current_json();
                response = header + "Content-Type: application/json\r\n\r\n" + content;
            } else if (request.find("GET /api/history") != std::string::npos) {
                int seconds = 3600;
                size_t pos = request.find("seconds=");
                if (pos != std::string::npos) {
                    try {
                        seconds = std::stoi(request.substr(pos + 8));
                    } catch (...) {}
                }
                std::string content = db.get_history_json(seconds);
                response = header + "Content-Type: application/json\r\n\r\n" + content;
            } else {
                std::ifstream f("index.html");
                if (f.good()) {
                    std::stringstream ss;
                    ss << f.rdbuf();
                    response = header + "Content-Type: text/html\r\n\r\n" + ss.str();
                } else {
                    response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
                }
            }

#ifdef _WIN32
            send(new_socket, response.c_str(), response.size(), 0);
            closesocket(new_socket);
#else
            write(new_socket, response.c_str(), response.size());
            close(new_socket);
#endif
        }
    }

private:
    int port;
};