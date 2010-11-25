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
};

class Data {

public: 
    Data();
    ~Data();

    void addClient(string name, int descriptor);
    void removeClient(string name);
    void addFollower(int clientDescriptor, int personToFollow);
    void removeFollower(int clientDescriptor, int followedPerson);

    set<int> getFollowers(int clientDescriptor);

    bool isClient(string name);

    int getDescriptorByName(string name);
    string getNameByDescriptor(int descriptor);
    
private:

    list<client> *users;
};


#endif