#include "info.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void Info::log(char* value) {
    log(string(value));
}

void Info::log(string value) {
    cout << value << endl;

    ofstream myfile ("log.txt", ios::app);

    if (myfile.is_open()) {
        myfile << value << "\n";
        myfile.close();
    }
}
