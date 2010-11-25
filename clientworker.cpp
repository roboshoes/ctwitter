#include <iostream>
#include "clientworker.h"
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

ClientWorker::ClientWorker(int socket):socketFileDescriptor(socket) {};

void ClientWorker::showMenu() {
    system("cls");

    printMenu();

    char choice;

    cout << endl << endl << ">> ";
    cin >> choice;

    char name[USERNAME_LENGTH];

    switch (choice) {
        case 't':
            cout << "What's going on?" << endl;
            cout << ">> ";

            char message[TWEET_LENGTH];

            cin.ignore(1000, '\n');
            cin.getline(message, TWEET_LENGTH);

            sendTweet(message);
        break;

        case 'f':
            cout << "Who do you want to follow?" << endl;
            cout << ">> ";

            cin.ignore(1000, '\n');
            cin.getline(name, TWEET_LENGTH);

            sendFollow(name);
        break;

        case 'u':
            cout << "Who isn't interesting any more?" << endl;
            cout << ">> ";

            cin.ignore(1000, '\n');
            cin.getline(name, TWEET_LENGTH);

            sendUnfollow(name);
        break;
    }

    cout << "Request completed" << endl;

    Sleep(500);
}

void ClientWorker::printMenu() {
    cout << "What to do next?" << endl << endl;

    cout << "[t] ... tweet" << endl;
    cout << "[f] ... follow" << endl;
    cout << "[u] ... unfollow" << endl;

    cout << endl << endl;
}

void ClientWorker::sendTweet(char* message) {
    string buffer;
    buffer.append("t");
    buffer.append(message);

    const char* tweet;
    tweet = buffer.c_str();

    send(socketFileDescriptor, tweet, TWEET_LENGTH + 1, 0); 
}

void ClientWorker::sendFollow(char* name) {
    string buffer;
    buffer.append("f");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}

void ClientWorker::sendUnfollow(char* name) {
    string buffer;
    buffer.append("u");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}