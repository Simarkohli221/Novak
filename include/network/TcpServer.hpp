#pragma once

#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include "Socket.hpp"
#include "ClientSession.hpp"
#include "../engine/MatchingEngine.hpp"
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

    std::thread acceptThread_;

    std::atomic<bool> running_{false};
    novax::engine::MatchingEngine matchingEngine_;
    std::vector<std::unique_ptr<ClientSession>> sessions_;
};

}