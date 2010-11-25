#include <iostream>
#include <cstdlib>
#include <ios>
#include <istream>
#include <limits>
#include <conio.h>
#include <pthread.h>
#include <list>
#include <string>
#include "client.h"
#include "clientworker.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Client::Client(char* _ip, unsigned short _port):ip(_ip), port(_port) {
    tweetList = new list<string>;
}

Client::~Client() {
    delete tweetList;
}

void Client::connectSocket() {

    initializeWindowsSockets();

    socketFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFileDescriptor < 0) {
		throw "Couldn't open Socket";
	}

    serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);

	int error = connect(socketFileDescriptor, (sockaddr*) &serverAddress, sizeof(serverAddress));
	if (error < 0) {
		throw "Couldn't connect to server.";
	}

}

void Client::start() {

    char name[USERNAME_LENGTH];

    cout << "Enter username: ";
    cin >> name;
    cout << endl << endl;

    sendLogin(name);

    isAlive = true;
    printTweets = true;

    showTweetScreen();
}

void Client::close() {
    closesocket(socketFileDescriptor);
	WSACleanup();
}

/*
 * Functions and data structures needed vor the threads.
 */
void* launchMemberFunction(void *args);
void* collectTweets(void *args);

struct threadParams {
    bool* printTweets;
    int socket;
    list<string>* tweetList;
};

struct collectorParams {
    bool* printTweets;
    bool* isAlive;
    int socket;
    int tweetLength;
    list<string>* tweetList;
};
/**/

void Client::showTweetScreen() {
    
    collectorParams* tweetSelectorParams;
    tweetSelectorParams = (collectorParams*) malloc(sizeof(collectorParams));

    tweetSelectorParams->printTweets = &printTweets;
    tweetSelectorParams->socket = socketFileDescriptor;
    tweetSelectorParams->tweetLength = TWEET_LENGTH;
    tweetSelectorParams->isAlive = &isAlive;
    tweetSelectorParams->tweetList = tweetList;

    pthread_t tweetRecieveID;
    pthread_create(&tweetRecieveID, NULL, &collectTweets, tweetSelectorParams);

    while(isAlive) {
        
        if (_kbhit() && printTweets) {

            threadParams* params;
            params = (threadParams *) malloc(sizeof(threadParams));

            params->printTweets = &printTweets;
            params->socket = socketFileDescriptor;
            params->tweetList = tweetList;

            pthread_t receiveID;
            pthread_create(&receiveID, NULL, &launchMemberFunction, params);

            printTweets = false;
        }
    }
}

void* collectTweets(void *args) {

    pthread_detach(pthread_self());

    collectorParams *params;
    params = (collectorParams*) args;

    bool setMissedTweets = true;

    while(*((bool*)params->isAlive)) {

        char* buffer = new char[Client::TWEET_LENGTH + Client::USERNAME_LENGTH + 10];
        memset(buffer, 0, sizeof(char) * (Client::TWEET_LENGTH + Client::USERNAME_LENGTH + 10));

        int response = recv(params->socket, buffer, params->tweetLength, 0);

        if (response < 0) {
            cout << "Error on recieving" << endl;
        }
        
        params->tweetList->push_back(string(buffer));

        if (*((bool*)params->printTweets)) {
            cout << buffer << endl;  
        } 

        delete [] buffer;
    }

    return NULL;
}

void* launchMemberFunction(void *args) {

    system("cls");

    pthread_detach(pthread_self());

    threadParams *params;
    params = (threadParams*) args;

    ClientWorker myWorker(params->socket);
    myWorker.showMenu();

    
    system("cls");
    list<string>::iterator i = params->tweetList->begin();
    if (!params->tweetList->empty()) {
        while (i != params->tweetList->end()) {
        
            cout << (*i) << endl;

            i++;
        }
    }

    *((bool*)params->printTweets) = true;

    free(params);

    return NULL;
}


void Client::sendLogin(char* name) {

    string buffer;
    buffer.append("n");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}


void Client::initializeWindowsSockets() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
        throw "Couldn't initialize Windows Sockets (WSAStartup failed)";
    }
}