#ifndef CLIENTWORKER_H
#define CLIENTWORKER_H

class ClientWorker {

public:

    ClientWorker(int socket);
    
    void showMenu();
 
private:

    int socketFileDescriptor;

    void printMenu();

    void sendTweet(char* tweet);
    void sendFollow(char* name);
    void sendUnfollow(char* name);
    void refreshTweets();

    static const int USERNAME_LENGTH = 25;
    static const int TWEET_LENGTH = 140;

};

#endif