#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>

class Server {

public:

    Server(char* ip, unsigned short port);
    
    void start();
    void selectSockets();

private:

    bool isAlive;
    char *ip;
    unsigned short port;

    sockaddr_in serverAddress;
    sockaddr_in clientAddress;

    int serverSocketFileDescriptor;
    int sizeOfClient;
    int highestSocketFileDescriptor;

    void initializeWindowsSockets();
    void openSocket();
    void bindSocket();
    void listenSocket();
};

#endif