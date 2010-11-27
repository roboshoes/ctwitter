#ifndef DATA_H
#define DATA_H

#include <string>
#include <list>
#include <set>
#include <iostream>

using namespace std;

struct client {
    string name;
    int descriptor;
    set<int> following;
    set<int> followed;
    bool online;
};

struct tweetInfo {
    string name;
    string message;
};

class Data {

  public: 

    Data();
    ~Data();

    void addClient(string name, int descriptor);
    void removeClient(int descriptor);

    void addFollower(int clientDescriptor, int personToFollow);
    void removeFollower(int clientDescriptor, int followedPerson);

    set<int> getFollowers(int clientDescriptor);

    bool isClient(string name);
    bool isOnline(string name);
    void clientOnline(int descriptor, bool value);

    int getDescriptorByName(string name);
    string getNameByDescriptor(int descriptor);

    void addTweet(string name, string tweet);
    list<string>* getTweetsByPublisher(string name);
    list<string>* getTweetsForClient(int descriptor);
    
  private:

    list<client> *users;
    list<tweetInfo> *tweets;
};


#endif