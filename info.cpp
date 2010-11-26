#include "info.h"
#include <iostream>
#include <string>

using namespace std;

Info* Info::instance = 0;

Info::Info(string filename = "log.txt") {
    instance = this;
}

Info* Info::getInstance() {
    if (instance == 0) 
        return new Info;

    return instance;
}

void Info::log(char* value) {
    log(string(value));
}

void Info::log(string value) {
    cout << value << endl;
}