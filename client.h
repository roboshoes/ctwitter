#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <list>
#include <string>
#include <iostream>

using namespace std;

class Client {

public:

    Client(char* ip, unsigned short port);
    ~Client();

    void connectSocket();
    void start();
    void close();

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