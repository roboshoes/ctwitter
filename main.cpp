#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

int main () {
    
    char choice;

    cout << "Welcome to Twitter in your console!" << endl << endl;
    cout << "Do you wish to be;\n [c] Client \n [s] Server" << endl << endl << ">> ";

    cin >> choice;

    cout << endl << endl;

    if (choice == 's') {

        cout << "Starting server ... ";

        Server myServer("127.0.0.1", 1337);

        myServer.start();

        cout << "DONE" << endl << endl;
        cout << "Server is running and ready for input. " << endl;

        myServer.selectSockets();

    } else if (choice == 'c') {

        cout << "Starting client ... ";

        Client myClient("127.0.0.1", 1337);

        myClient.connectSocket();

        cout << "DONE" << endl;

        myClient.start();
    }

}