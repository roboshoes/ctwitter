#include <sys/types.h>
#include "server.h"
#include <pthread.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Server::Server(char* ip, unsigned short port) {

    this->port = port;
    this->ip = ip;

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(port);
    this->serverAddress.sin_addr.s_addr = inet_addr(ip);
}

void Server::start() {
    initializeWindowsSockets();

    openSocket();
    bindSocket();
    listenSocket();

    highestSocketFileDescriptor = serverSocketFileDescriptor;

    isAlive = true;
}

struct threadParameter {
    int clientAccept;
    fd_set *readFileDescriptorSet;
};

void *handleNewClient(void *params) {

    threadParameter* args;
    args = (threadParameter*) params;

    int connectionFileDescriptor;
    connectionFileDescriptor = args->clientAccept;

    pthread_detach(pthread_self());

    FD_SET(connectionFileDescriptor, args->readFileDescriptorSet);

    //closesocket(connectionFileDescriptor);

    send(args->clientAccept, "Welcome to Twitter", 18, 0);

    free(args);

    return 0;
}

void *receiveMessage(void *i) {

    cout << "recv" << endl;
   
    int* socket;
    socket = (int*) i;

    char buffer[140];
    memset(buffer, 0, sizeof(buffer));

    recv(*socket, buffer, 140, 0);

    cout << "Message from client: " << *socket << "  " << buffer << endl;

    send(*socket, "Socket", 6, 0);

    free(i);

    return 0;
}

void Server::selectSockets() {

    int selectResult;

    fd_set readFileDescriptorSet;

    timeval timeInterval;
            timeInterval.tv_sec = 5;
            timeInterval.tv_usec = 0;

    FD_ZERO(&readFileDescriptorSet);
    FD_SET(serverSocketFileDescriptor, &readFileDescriptorSet);

    sizeOfClient = sizeof(clientAddress);

    while (isAlive) {
        
        selectResult = select(highestSocketFileDescriptor + 1, &readFileDescriptorSet, NULL, NULL, NULL);

        cout << "selectResult: " << selectResult << endl;

        if (selectResult > 0) {

            /*
            if (FD_ISSET(serverSocketFileDescriptor, &readFileDescriptorSet)) {

                threadParameter* params;
                params = (threadParameter *) malloc(sizeof(threadParameter));
                
                params->readFileDescriptorSet = &readFileDescriptorSet;
                params->clientAccept = accept(serverSocketFileDescriptor, (sockaddr *)&clientAddress, &sizeOfClient);
                
                cout << "client connected on: " << params->clientAccept << endl;

                if (params->clientAccept > highestSocketFileDescriptor) {
                    highestSocketFileDescriptor = params->clientAccept;
                }

                pthread_t acceptID;
                pthread_create(&acceptID, NULL, &handleNewClient, params);
            }

            cout << highestSocketFileDescriptor << endl;

            */

            for (int i = 4; i < highestSocketFileDescriptor + 1; i++) {

                if (FD_ISSET(i, &readFileDescriptorSet)) {
                   
                    if (i == serverSocketFileDescriptor) {
                        threadParameter* params;
                        params = (threadParameter *) malloc(sizeof(threadParameter));
                
                        params->readFileDescriptorSet = &readFileDescriptorSet;
                        params->clientAccept = accept(serverSocketFileDescriptor, (sockaddr *)&clientAddress, &sizeOfClient);
                
                        cout << "client connected on: " << params->clientAccept << endl;

                        if (params->clientAccept > highestSocketFileDescriptor) {
                            highestSocketFileDescriptor = params->clientAccept;
                        }

                        pthread_t acceptID;
                        pthread_create(&acceptID, NULL, &handleNewClient, params);

                    } else {
                        
                        cout << "found inbound socket at: " << i << endl;

                        int *descriptor;
                        descriptor = (int*) malloc(sizeof(int*));
                        *descriptor = i; 

                        pthread_t receiveID;
                        pthread_create(&receiveID, NULL, &receiveMessage, descriptor); 

                    }
                }
            }
        }
    }
}

void Server::openSocket() {
    serverSocketFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocketFileDescriptor < 0) {
        throw "Couldn't open socket";
    }
}

void Server::bindSocket() {
    int bindResult = bind(serverSocketFileDescriptor, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindResult < 0) {
        throw "Couldn't bind socket";
    }
}

void Server::listenSocket() {
    int listenResult = listen(serverSocketFileDescriptor, 3);
    if (listenResult < 0) {
        throw "Couldn't listen to Socket";
    }
}

void Server::initializeWindowsSockets() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
        throw "Couldn't initialize Windows Sockets (WSAStartup failed)";
    }
}
