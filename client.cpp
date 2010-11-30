/*
 * Author: David Strauß, Mathias Paumgarten
 */

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

/* 
 * Initialize the tweets list which stores all tweets the client handles as long as its running.
 */
Client::Client(char* _ip, unsigned short _port):ip(_ip), port(_port) {
    tweetList = new list<string>;
}

Client::~Client() {
    delete tweetList;
}

/*
 * Connect to the socket. Therefore a server must be up and running.
 */
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

/*
 * When connection to server is established user is asked to login with a username.
 * As long as the the client doesn't accept to login with an answer of 1 the user will be
 * asked again to login with a other username.
 */
void Client::start() {

    char name[USERNAME_LENGTH];
    bool userAccepted = false;

    char answer[4];
    memset(answer, 0, sizeof(answer));

    while (!userAccepted) {

        cout << "Enter username: ";
        cin >> name;
        cout << endl << endl;

        sendLogin(name);

        int response = recv(socketFileDescriptor, answer, sizeof(answer), 0);
        if (response < 0) {
            cout << "Error on loggin. Programm was canceled" << endl;
            exit(0);
        }

        if (answer[0] == '1') {    // Server sends '1' as OK for login.
            userAccepted = true;
        } else {                   // Server sends '0' when Error occured
            cout << "This user is already logged in." << endl << endl;
        }
    }

    system("cls");

    isAlive = true;
    printTweets = true;

    showTweetScreen();
}

/*
 * This function starts thread which collects all tweets that are sent from the server.
 * Additionally it checks for any input from the keyboard to launch menu is the user
 * desires so.
 */
void Client::showTweetScreen() {
   
    // Introductions that are set into the tweetslist, so that tweetsscreen always shoes them on the top
    tweetList->push_back("====================================");
    tweetList->push_back("==  ALL YOUR TWEETS ON ONE SIGHT  ==");
    tweetList->push_back("====================================");
    tweetList->push_back(" ");
    tweetList->push_back("(Hit any key to go into the the menu mode for tweeting or following)\n\n");

    // Parameters that are needed in the thread that collects all server sent tweets.
    collectorParams* tweetSelectorParams;
    tweetSelectorParams = (collectorParams*) malloc(sizeof(collectorParams));

    tweetSelectorParams->printTweets = &printTweets;
    tweetSelectorParams->socket = socketFileDescriptor;
    tweetSelectorParams->tweetLength = TWEET_LENGTH;
    tweetSelectorParams->isAlive = &isAlive;
    tweetSelectorParams->tweetList = tweetList;

    // Creating and starting the thread that collects the tweets.
    pthread_t tweetRecieveID;
    pthread_create(&tweetRecieveID, NULL, &collectTweets, tweetSelectorParams);

    // This loop constantly checks for User input. If User hits a button on the keyboard it
    // will start a thread with the menu.
    while(isAlive) {
        
        if (_kbhit() && printTweets) {

            threadParams* params;
            params = (threadParams *) malloc(sizeof(threadParams));

            params->printTweets = &printTweets;
            params->socket = socketFileDescriptor;
            params->tweetList = tweetList;

            pthread_t receiveID;
            pthread_create(&receiveID, NULL, &launchUserMenu, params);

            printTweets = false;
        }
    }
}


/*
 * This function collects all tweets that are sent to the client.
 */
void* collectTweets(void *args) {

    pthread_detach(pthread_self());

    collectorParams *params;
    params = (collectorParams*) args;

    // Print all tweets that are stored in the tweet list, befor server was sending messages 
    // (In our cause, this is always the header)
    list<string>::iterator i = params->tweetList->begin();
    if (!params->tweetList->empty()) {
        while (i != params->tweetList->end()) {
        
            cout << (*i) << endl;

            i++;
        }
    }

    // This loop keeps checking for new messages. 
    while(*((bool*)params->isAlive)) {

        char* buffer = new char[Client::TWEET_LENGTH + Client::USERNAME_LENGTH + 10];
        memset(buffer, 0, sizeof(char) * (Client::TWEET_LENGTH + Client::USERNAME_LENGTH + 10));

        int response = recv(params->socket, buffer, params->tweetLength, 0);

        if (response < 0) {

            closesocket(params->socket);
            cout << "=======================\nError on recieving. Socket was closed." << endl;

            *((bool*)params->isAlive) = false;
            delete [] buffer;
            break;
        }
        
        // Push new messages into the list of tweets
        params->tweetList->push_back(string(buffer).append("\n"));

        // If the menu is currenty in use by the user, the application cannot print new 
        // tweets.
        if (*((bool*)params->printTweets)) {
            cout << buffer << "\n" << endl;  
        } 

        delete [] buffer;
    }

    return NULL;
}

/*
 * Functions launches an instance of ClientWorker which is used to show the menu
 * and send requests to the server such as tweet or so.
 */
void* launchUserMenu(void *args) {

    system("cls");

    pthread_detach(pthread_self());

    threadParams *params;
    params = (threadParams*) args;

    // Client worker actually handles the printing of the menu
    // and the user input.
    ClientWorker myWorker(params->socket);
    myWorker.showMenu();

    // After user has finished the request the menu is cleared from the screen
    // and all tweets are printed, including the tweets that were sent meanwhile the
    // user was in the menu.
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

/*
 * This function handles the send request to login
 */
void Client::sendLogin(char* name) {

    string buffer;
    buffer.append("n");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}

/*
 * Needed to use sockets on windows.
 */
void Client::initializeWindowsSockets() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
        throw "Couldn't initialize Windows Sockets (WSAStartup failed)";
    }
}