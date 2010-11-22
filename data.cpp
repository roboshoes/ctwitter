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

    }
}

bool Data::isClient(string name) {
    
    list<client>::iterator i = users->begin();
    while (i != users->end()) {
        
        if ((*i).name == name) {
            return true;
        }

    }
    
    return false;
}