#include "../../include/network/TcpServer.hpp"
#include <iostream>

namespace novax::network
{

TcpServer::TcpServer() = default;

TcpServer::~TcpServer()
{
    stop();
}

bool TcpServer::start(int port)
{
    serverSocket_ = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSocket_ == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(
            serverSocket_,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) == SOCKET_ERROR)
    {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
        return false;
    }

    if (listen(serverSocket_, SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
        return false;
    }

    running_ = true;

    acceptThread_ = std::thread(
        &TcpServer::acceptLoop,
        this
    );

    return true;
}

void TcpServer::stop()
{
    running_ = false;

    if (serverSocket_ != INVALID_SOCKET)
    {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }

    if (acceptThread_.joinable())
    {
        acceptThread_.join();
    }
}

void TcpServer::acceptLoop()
{
    while (running_)
    {
        sockaddr_in clientAddress{};
        int clientLength = sizeof(clientAddress);

        Socket clientSocket =
            accept(
                serverSocket_,
                reinterpret_cast<sockaddr*>(&clientAddress),
                &clientLength
            );
            std::cout << "Client connected!\n";
        if (clientSocket == INVALID_SOCKET)
        {
            continue;
        }

        closesocket(clientSocket);
    }
}

} // namespace novax::network