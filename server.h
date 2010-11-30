/*
 * Author: David Strauß, Mathias Paumgarten
 */


#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include "data.h"

class Server {

public:

    Server(char* ip, unsigned short port);
    ~Server();
    
    void start();
    void selectSockets();

private:

    bool isAlive;
    char *ip;
    unsigned short port;
    Data *data;

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