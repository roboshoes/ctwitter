/*
 * Author: David Strauß, Mathias Paumgarten
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <list>
#include <string>
#include <iostream>

using namespace std;

/*
 * -----------------------------------------------------
 * Functions and data structures needed vor the threads.
 */

void* launchUserMenu(void *args);
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
/* ----------------------------------------------------- */

class Client {

public:

    Client(char* ip, unsigned short port);
    ~Client();

    void connectSocket();
    void start();

    static const int USERNAME_LENGTH = 25;
    static const int TWEET_LENGTH = 140;

private:

    char* ip;
    unsigned short port;
    int socketFileDescriptor;

    bool isAlive;
    bool printTweets;

    list<string>* tweetList;
    
    sockaddr_in serverAddress;

    void initializeWindowsSockets();
    void showTweetScreen();

    void sendLogin(char* name);
};

#endif