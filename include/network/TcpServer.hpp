#pragma once

#include <atomic>
#include <thread>

#include "Socket.hpp"

namespace novax::network
{

class TcpServer
{
public:

    TcpServer();

    ~TcpServer();

    bool start(int port);

    void stop();

private:

    void acceptLoop();

private:

    Socket serverSocket_{INVALID_SOCKET};

    std::atomic<bool> running_{false};

    std::thread acceptThread_;
};

}