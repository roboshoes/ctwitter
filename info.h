#ifndef INFO_H
#define INFO_H

#include <iostream>
#include <string>

using namespace std;

class Info {

public:
    Info* getInstance();

    static void log(string value);
    static void log(char* value);

private:
    Info(string filename);

    static Info* instance;

};

#endif