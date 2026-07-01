#include "../../include/network/ClientSession.hpp"

namespace novax::network
{

ClientSession::ClientSession(int socket)
    : socket_(socket)
{
}

ClientSession::~ClientSession()
{
    if (workerThread_.joinable())
    {
        workerThread_.join();
    }
}

void ClientSession::start()
{
    workerThread_ = std::thread(
        &ClientSession::sessionLoop,
        this
    );
}

void ClientSession::sessionLoop()
{
}

}