#ifndef DATA_H
#define DATA_H

#include <string>
#include <list>
#include <iostream>

using namespace std;

struct client {
    string name;
    int descriptor;
};

class Data {

public: 
    Data();
    ~Data();

    void addClient(string name, int descriptor);
    void removeClient(string name);
    bool isClient(string name);
    
private:

    list<client> *users;
};


#endif