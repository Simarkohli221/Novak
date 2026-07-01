#include "../../include/network/ClientSession.hpp"

#include <iostream>
#include <string>

#include "../../include/parser/CommandParser.hpp"

namespace novax::network
{

ClientSession::ClientSession(
    Socket socket,
    novax::engine::MatchingEngine& engine
)
    : socket_(socket),
      matchingEngine_(engine)
{
}

ClientSession::~ClientSession()
{
    if (workerThread_.joinable())
    {
        workerThread_.join();
    }

    if (socket_ != INVALID_SOCKET)
    {
        closesocket(socket_);
    }
}

void ClientSession::start()
{
    workerThread_ = std::thread(
        &ClientSession::sessionLoop,
        this
    );
}

void ClientSession::sendMessage(const std::string& message)
{
    send(
        socket_,
        message.c_str(),
        static_cast<int>(message.size()),
        0
    );
}

void ClientSession::sessionLoop()
{
    char buffer[1024];

    while (true)
    {
        int bytesReceived = recv(
            socket_,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0)
        {
            break;
        }

        buffer[bytesReceived] = '\0';

        std::string command(buffer);

        auto request =
            novax::parser::CommandParser::parse(command);

        if (!request)
        {
            sendMessage("INVALID COMMAND\n");

            std::cout << "Invalid command\n";

            continue;
        }

if (request->type ==
    novax::engine::RequestType::CANCEL_ORDER)
{
    matchingEngine_.submitOrder(*request);

    sendMessage(
        "CANCEL REQUEST RECEIVED\n"
    );
}
else
{
    auto orderId =
        matchingEngine_.submitOrder(*request);

    sendMessage(
        "ORDER ACCEPTED ID=" +
        std::to_string(orderId) +
        "\n"
    );
}

        novax::common::Trade trade;

        while (matchingEngine_.getTrade(trade))
        {
            std::string message =
                "TRADE " +
                trade.symbol +
                " " +
                std::to_string(trade.quantity) +
                " @" +
                std::to_string(trade.price) +
                "\n";

            sendMessage(message);
        }

        std::cout << "Order submitted\n";
    }
}

} // namespace novax::network