/*
 * Author: David Strauß, Mathias Paumgarten
 */


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
    set<string> following;
    set<string> followed;
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

    void addFollower(string clientName, string personToFollow);
    void removeFollower(string clientName, string followedPerson);

    set<string> getFollowers(string clientName);

    bool isClient(string name);
    bool isOnline(string name);
    void setClientOnline(string name, bool value);
    void setClientDescriptor(string clientName, int descriptor);

    int getDescriptorByName(string name);
    string getNameByDescriptor(int descriptor);

    void addTweet(string name, string tweet);
    list<string>* getTweetsByPublisher(string name);
    list<string>* getTweetsForClient(string name);
    
  private:

    list<client> *users;
    list<tweetInfo> *tweets;
};


#endif