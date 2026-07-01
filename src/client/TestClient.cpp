#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in server{};

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server.sin_addr
    );

    if (connect(
            sock,
            reinterpret_cast<sockaddr*>(&server),
            sizeof(server)
        ) == SOCKET_ERROR)
    {
        std::cout << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to Novak Exchange\n";
    std::cout << "Type EXIT to quit\n\n";

    std::string command;

    while (true)
    {
        std::cout << "> ";

        std::getline(std::cin, command);

        if (command == "EXIT")
        {
            break;
        }

        command += "\n";
send(
    sock,
    command.c_str(),
    static_cast<int>(command.size()),
    0
);

char buffer[1024];

int bytesReceived = recv(
    sock,
    buffer,
    sizeof(buffer) - 1,
    0
);

if (bytesReceived > 0)
{
    buffer[bytesReceived] = '\0';

    std::cout << buffer;
}
else
{
    std::cout << "Server disconnected\n";
    break;
}
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}