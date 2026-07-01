#pragma once

#include <thread>

namespace novax::network
{

class ClientSession
{
public:

    explicit ClientSession(int socket);

    ~ClientSession();

    void start();

private:

    void sessionLoop();

private:

    int socket_;

    std::thread workerThread_;
};

}