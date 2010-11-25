#include "data.h"
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

Data::Data() {
    users = new list<client>;
}

Data::~Data() {
    delete users;
}

void Data::addClient(string name, int descriptor) {
    client currentClient;

    currentClient.name = name;
    currentClient.descriptor = descriptor;

    users->push_back(currentClient);
}

void Data::removeClient(string name) {

    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            users->erase(i++);
        }

        i++;

    }
}

bool Data::isClient(string name) {
    
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            return true;
        }

        i++;

    }
    
    return false;
}

int Data::getDescriptorByName(string name) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            return (*i).descriptor;
        }

        i++;

    }
    
    return -1;
}

string Data::getNameByDescriptor(int descriptor) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).descriptor == descriptor) {
            return (*i).name;
        }

        i++;

    }
    
    return "UNKOWN USER";
}

void Data::addFollower(int clientDescriptor, int personToFollow) {

    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).descriptor == clientDescriptor) {
            (*i).following.insert(personToFollow);
        }

        if ((*i).descriptor == personToFollow) {
            (*i).followed.insert(clientDescriptor);
        }

        i++;

    }
}

void Data::removeFollower(int clientDescriptor, int followedPerson) {
    
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).descriptor == clientDescriptor) {
            (*i).following.erase(followedPerson);
        }

        if ((*i).descriptor == followedPerson) {
            (*i).followed.erase(clientDescriptor);
        }

        i++;

    }

}

set<int> Data::getFollowers(int clientDescriptor) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).descriptor == clientDescriptor) {
            return (*i).followed;
        }

        i++;

    } 

    set<int> emptySet;
    return emptySet;
}