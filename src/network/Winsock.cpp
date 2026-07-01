#include <winsock2.h>

namespace
{

class WinsockInitializer
{
public:

    WinsockInitializer()
    {
        WSADATA data;

        WSAStartup(MAKEWORD(2, 2), &data);
    }

    ~WinsockInitializer()
    {
        WSACleanup();
    }
};

WinsockInitializer initializer;

}