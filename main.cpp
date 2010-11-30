/*
 * Author: David Strauß, Mathias Paumgarten
 */

#include <iostream>
#include "server.h"
#include "client.h"
#include "info.h"

using namespace std;

/*
 * Starting Point of Program. Depending on user input it will either start a client or a server node.
 * Not possible to launch two servers on same IP and Port.
 */
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
        cout << "Server is running and ready for input. " << endl << endl;

        cout << "==================== ACTIVITY ====================" << endl << endl;

        myServer.selectSockets();

    } else if (choice == 'c') {

        cout << "Starting client ... ";

        Client myClient("127.0.0.1", 1337);

        myClient.connectSocket();

        cout << "DONE" << endl << endl;

        myClient.start();
    }

}