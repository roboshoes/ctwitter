#ifndef INFO_H
#define INFO_H

#include <iostream>
#include <string>

using namespace std;

class Info {

public:

    static string filename;

    static void log(string value);
    static void log(char* value);

};

#endif