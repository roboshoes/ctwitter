#include <sys/types.h>
#include "server.h"
#include <pthread.h>
#include <iostream>
#include <cerrno>
#include <string>
#include <sstream>
#include "client.h"
#include "info.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Server::Server(char* _ip, unsigned short _port):ip(_ip), port(_port) {

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(port);
    this->serverAddress.sin_addr.s_addr = inet_addr(ip);

    data = new Data();
}

Server::~Server() {
    delete data;
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
    Data *data;
    char* message;
};

void *handleIncomeMessage(void *params) {

    threadParameter* args;
    args = (threadParameter*) params;

    int socket = args->clientAccept;

    char order = args->message[0];

    string message(args->message);
    message.erase(0, 1);

    string forwardMessage;
    string logInfo;
    stringstream stream;

    set<int> followersList;
    set<int>::iterator m;
    list<string>::iterator u;

    list<string>* tweets;

    switch (order) {

        case 'n':

            if (args->data->isClient(message)) {

                if (args->data->isOnline(message)) {

                    send(args->clientAccept, "0", sizeof(char), 0);

                    stream << "LOGIN          Error. User " << message << " is alread logged in";
                
                    Info::log(stream.str());

                } else {
                    send(args->clientAccept, "1", sizeof(char), 0);

                    stream << "LOGIN          User " << message << " came back. Now as client " << args->clientAccept;
                    Info::log(stream.str());

                    args->data->clientOnline(args->clientAccept, true);

                    tweets = args->data->getTweetsForClient(args->clientAccept);

                    u = tweets->begin();
                    while (u != tweets->end()) {
                        send(args->clientAccept, (*u).c_str(), sizeof(char) * (*u).length(), 0);
                        u++;
                    }

                    stream << "REFRESH        Sending all related tweets to client " << args->clientAccept;
                    Info::log(stream.str());
                }

            } else {
                args->data->addClient(message, args->clientAccept);
                send(args->clientAccept, "1", sizeof(char), 0);

                stream << "LOGIN          client " << args->clientAccept << " logged in as " << message;

                Info::log(stream.str());
            }

        break;

        case 't':
           
            stream << "TWEET          client " << args->clientAccept << " tweets: " << message;
            Info::log(stream.str());

            forwardMessage.append(args->data->getNameByDescriptor(args->clientAccept));
            forwardMessage.append(": ");
            forwardMessage.append(message);

            followersList = args->data->getFollowers(args->clientAccept);
            m = followersList.begin();

            while (m != followersList.end()) {
                send((*m), forwardMessage.c_str(), Client::TWEET_LENGTH, 0); 

                stream.str("");
                stream << "SEND TWEET     " << "forward tweet to " << (*m);
                Info::log(stream.str());

                m++;
            }

        break;

        case 'f':
            stream << "FOLLOW         client " << args->clientAccept << " now follows " << message;
            Info::log(stream.str());

            args->data->addFollower(args->clientAccept, args->data->getDescriptorByName(message));
        break;

        case 'u':
            stream << "UNFOLLOW       client " << args->clientAccept << " stopped following " << message;
            Info::log(stream.str());

            args->data->removeFollower(args->clientAccept, args->data->getDescriptorByName(message));
        break;

        default:
            
            stream << "UNKNOWKN TOKEN recieved order '" << order << "' with message: " << message;
            Info::log(stream.str());

        break;

    }

    std::free(args);

    return 0;
}

void Server::selectSockets() {

    int selectResult;

    fd_set currentReadSet;
    fd_set masterReadSet;

    timeval timeInterval;
            timeInterval.tv_sec = 5;
            timeInterval.tv_usec = 0;

    FD_ZERO(&masterReadSet);
    FD_SET(serverSocketFileDescriptor, &masterReadSet);

    sizeOfClient = sizeof(clientAddress);

    while (isAlive) {

        stringstream stream;

        memcpy(&currentReadSet, &masterReadSet, sizeof(masterReadSet));

        selectResult = select(highestSocketFileDescriptor + 1, &currentReadSet, NULL, NULL, NULL);

        if (selectResult < 0) {
            cout << "Error with select!\nProgram is canceled" << endl;
            closesocket(serverSocketFileDescriptor);
            exit(1);
        }

        if (selectResult > 0) {

            for (int i = 4; i < highestSocketFileDescriptor + 1; i++) {

                if (FD_ISSET(i, &currentReadSet)) {
                   
                    if (i != serverSocketFileDescriptor) {

                        threadParameter* params;
                        params = (threadParameter *) malloc(sizeof(threadParameter));
                        
                        char buffer[Client::TWEET_LENGTH + 1];
                        memset(buffer, 0, sizeof(buffer));

                        int response = recv(i, buffer, sizeof(buffer), 0);

                        if (response == -1) {
                            closesocket(i);
                            FD_CLR(i, &masterReadSet);

                            data->removeClient(i);

                            stream << "DISCONNECT     client " << i << " left";
                            Info::log(stream.str());

                            continue;
                        }

                        params->clientAccept = i;
                        params->data = data;
                        params->message = buffer;

                        pthread_t receiveID;
                        pthread_create(&receiveID, NULL, &handleIncomeMessage, params);

                    } else if (i == serverSocketFileDescriptor) {

                        threadParameter* params;
                        params = (threadParameter *) malloc(sizeof(threadParameter));
                
                        params->readFileDescriptorSet = &masterReadSet;
                        params->clientAccept = accept(serverSocketFileDescriptor, (sockaddr *)&clientAddress, &sizeOfClient);
                        params->data = data;
                        
                        FD_SET(params->clientAccept, &masterReadSet);

                        stream << "CONNECTION     client connected on: " << params->clientAccept;
                        Info::log(stream.str());

                        if (params->clientAccept > highestSocketFileDescriptor) {
                            highestSocketFileDescriptor = params->clientAccept;
                        }
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
