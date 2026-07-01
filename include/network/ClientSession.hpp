#pragma once

#include <thread>

#include "Socket.hpp"
#include "../engine/MatchingEngine.hpp"
#include <string>
namespace novax::network
{

class ClientSession
{
public:

    ClientSession(
        Socket socket,
        novax::engine::MatchingEngine& engine
    );

    ~ClientSession();

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    void start();

private:

    void sessionLoop();

private:

    Socket socket_;

    novax::engine::MatchingEngine& matchingEngine_;

    std::thread workerThread_;
    void sendMessage(const std::string& message);
};

} // namespace novax::network