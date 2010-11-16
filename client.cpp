#include <iostream>
#include <cstdlib>
#include <ios>
#include <istream>
#include <limits>
#include <conio.h>
#include <pthread.h>
#include "client.h"
#include "clientworker.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Client::Client(char* _ip, unsigned short _port):ip(_ip), port(_port) {}

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

void* launchMemberFunction(void *args);
void* collectTweets(void *args);

struct threadParams {
    bool* printTweets;
    int socket;
};

void Client::showTweetScreen() {
    
    bool clearScreen = true;
    int selectResult;

    fd_set currentReadSet;

    FD_ZERO(&currentReadSet);
    FD_SET(socketFileDescriptor, &currentReadSet);

    pthread_t tweetRecieveID;
    pthread_create(&tweetRecieveID, NULL, &collectTweets, NULL);

    char buffer[TWEET_LENGTH];

    while(isAlive) {
        
        if (_kbhit() && printTweets) {

            threadParams* params;
            params = (threadParams *) malloc(sizeof(threadParams));

            params->printTweets = &printTweets;
            params->socket = socketFileDescriptor;

            pthread_t receiveID;
            pthread_create(&receiveID, NULL, &launchMemberFunction, params);

            printTweets = false;
            clearScreen = true;
        }

        timeval* timeInterval = new timeval;
        
        timeInterval->tv_sec = 1;
        timeInterval->tv_usec = 0;

        selectResult = select(socketFileDescriptor, &currentReadSet, NULL, NULL, timeInterval);

        delete timeInterval;

        if (selectResult == 1) {
            recv(socketFileDescriptor, buffer, TWEET_LENGTH, 0);
        }

        if (printTweets) {

            if (clearScreen) {
                system("cls");
                clearScreen = false;
            }

            if (selectResult == -1) {
                cout << "error: " << WSAGetLastError() << endl;
            }

            if (selectResult == 1) {
                cout << "recv: " << buffer << endl;
            }
        }
    }
}

void* launchMemberFunction(void *args) {

    system("cls");

    pthread_detach(pthread_self());

    threadParams *params;
    params = (threadParams*) args;

    ClientWorker myWorker(params->socket);
    myWorker.showMenu();

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