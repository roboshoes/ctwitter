#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>

class Client {

public:

    Client(char* ip, unsigned short port);
    void connectSocket();
    void start();
    void close();

private:

    char* ip;
    unsigned short port;
    int socketFileDescriptor;
    
    sockaddr_in serverAddress;

    void printMenu();
    void initializeWindowsSockets();
    void showTweetScreen();
    void showMenu();

    void sendTweet(char* tweet);
    void sendLogin(char* name);
    void sendFollow(char* name);
    void sendUnfollow(char* name);

    static const int USERNAME_LENGTH = 25;
    static const int TWEET_LENGTH = 140;
};

#endif