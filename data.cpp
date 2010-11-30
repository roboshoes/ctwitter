/*
 * Author: David Strauß, Mathias Paumgarten
 */

#include "data.h"
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

/*
 * This class is used to store all data that is used in and sent to the server.
 */

Data::Data() {
    users = new list<client>;      // All users. Online and offline.
    tweets = new list<tweetInfo>;  // All tweets including message and who wrote it.
}

Data::~Data() {
    delete users;
    delete tweets;
}

/*
 * Adds a new Tweet into the data structure.
 */
void Data::addTweet(string name, string tweet) {
    tweetInfo newTweet;

    newTweet.message = tweet;
    newTweet.name = name;

    tweets->push_back(newTweet);
}

/*
 * Returns all tweets that were sent by a certain user.
 */
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


/*
 * Returns all subscribed tweets for client according to the users he follows.
 */
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


// Adds a new client into the data structure.
void Data::addClient(string name, int descriptor) {
    client currentClient;

    currentClient.name = name;
    currentClient.descriptor = descriptor;
    currentClient.online = true;

    users->push_back(currentClient);
}


// Removes a client from the data structure.
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

// Checks if a client is already stored in the data structure.
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


// Checks if a client is currently connected to the server.
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


// Sets a extisting client's connection status.
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


// Sets the descriptor of a given client.
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

// Returns the descriptor to a client.
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


// Returns the name according to a descriptor.
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

// Adds a new follower for a client.
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


// Removes a follower from a client.
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

// Returns all followers from a client.
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