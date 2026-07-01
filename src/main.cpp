#include "../include/network/TcpServer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    std::cout << "Program started\n";

    novax::network::TcpServer server;

    std::cout << "Starting server...\n";

    if (!server.start(8080))
    {
        std::cout << "Server failed to start!\n";
        return 1;
    }

    std::cout << "Server started successfully.\n";

    std::this_thread::sleep_for(std::chrono::minutes(5));

    std::cout << "Stopping server...\n";

    server.stop();

    std::cout << "Done.\n";

    return 0;
}