#include "data.h"
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

Data::Data() {
    users = new list<client>;
    tweets = new list<tweetInfo>;
}

Data::~Data() {
    delete users;
    delete tweets;
}

void Data::addTweet(string name, string tweet) {
    tweetInfo newTweet;

    newTweet.message = tweet;
    newTweet.name = name;

    tweets->push_back(newTweet);
}

list<string>* Data::getTweetsByPublisher(string name) {
    list<string>* tweetList = new list<string>;

    list<tweetInfo>::iterator i = tweets->begin();
    while (i != tweets->end()) {
        if ((*i).name == name) {

            string newline = (*i).name;
            newline.append(": ");
            newline.append((*i).message);

            tweetList->push_back(newline);
        }

        i++;
    }

    return tweetList;
}

list<string>* Data::getTweetsForClient(string name) {
    list<string>* tweetList = new list<string>;

    list<client>::iterator i = users->begin();
    while (i != users->end()) {

        if ((*i).name == name) {

            set<string>::iterator m = (*i).following.begin();
            while (m != (*i).following.end()) {

                list<string>* currentList = getTweetsByPublisher(*m);

                list<string>::iterator b = currentList->begin();
                while (b != currentList->end()) {
                    tweetList->push_back(*b);
                    b++;
                }

                m++;

                delete currentList;
            }

            break;
        }

        i++;
    }

    return tweetList;
}

void Data::addClient(string name, int descriptor) {
    client currentClient;

    currentClient.name = name;
    currentClient.descriptor = descriptor;
    currentClient.online = true;

    users->push_back(currentClient);
}

void Data::removeClient(int descriptor) {

    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).descriptor == descriptor) {
            (*i).online = false;
            return;
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

bool Data::isOnline(string name) {

    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            if((*i).online) return true;
            else return false;
        }

        i++;

    }
    
    return false;
}

void Data::setClientOnline(string name, bool value) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            (*i).online = value;
            return;
        }

        i++;

    }
}

void Data::setClientDescriptor(string clientName, int descriptor) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == clientName) {
            (*i).descriptor = descriptor;
            return;
        }

        i++;

    }
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

void Data::addFollower(string clientName, string personToFollow) {

    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == clientName) {
            (*i).following.insert(personToFollow);
        }

        if ((*i).name == personToFollow) {
            (*i).followed.insert(clientName);
        }

        i++;

    }
}

void Data::removeFollower(string clientName, string followedPerson) {
    
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == clientName) {
            (*i).following.erase(followedPerson);
        }

        if ((*i).name == followedPerson) {
            (*i).followed.erase(clientName);
        }

        i++;

    }

}

set<string> Data::getFollowers(string clientName) {
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == clientName) {
            return (*i).followed;
        }

        i++;

    } 

    set<string> emptySet;
    return emptySet;
}