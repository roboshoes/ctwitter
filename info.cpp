/*
 * Author: David Strauß, Mathias Paumgarten
 */

#include "info.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*
 * Overloads the log function to also work wich char arrays, c style
 */
void Info::log(char* value) {
    log(string(value));
}

/*
 * Logs a message into the console and also writes it into a file.
 */
void Info::log(string value) {
    cout << value << endl;

    ofstream myfile ("log.txt", ios::app);

    if (myfile.is_open()) {
        myfile << value << "\n";
        myfile.close();
    }
}
