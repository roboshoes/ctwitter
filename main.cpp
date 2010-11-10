#include <iostream>
#include "server.h"

using namespace std;

int main () {
    
    Server myServer("127.0.0.1", 1337);

    try {

        myServer.start();
        myServer.selectSockets();

    } catch (char* error) {
        cout << error;
    }

}