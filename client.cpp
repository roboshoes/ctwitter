#include <iostream>
#include <cstdlib>
#include <ios>
#include <istream>
#include <limits>
#include "client.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Client::Client(char* _ip, unsigned short _port):ip(_ip), port(_port) {}

void Client::connectSocket() {

    initializeWindowsSockets();

    socketFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFileDescriptor < 0) {
		throw "Couldn't open Socket";
	}

    serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);

	int error = connect(socketFileDescriptor, (sockaddr*) &serverAddress, sizeof(serverAddress));
	if (error < 0) {
		throw "Couldn't connect to server.";
	}

}

void Client::start() {

    char name[USERNAME_LENGTH];

    cout << "Enter username: ";
    cin >> name;
    cout << endl << endl;

    sendLogin(name);

    showMenu();
}

void Client::close() {
    closesocket(socketFileDescriptor);
	WSACleanup();
}

void Client::showTweetScreen() {
    system("cls");
    
    int bla;
    cin >> bla;

    Sleep(1000);
}

void Client::showMenu() {
    system("cls");

    printMenu();

    char choice;

    cout << endl << endl << ">> ";
    cin >> choice;

    char name[USERNAME_LENGTH];

    switch (choice) {
        case 't':
            cout << "Whats going on?" << endl;
            cout << ">> ";

            char message[TWEET_LENGTH];
            cin >> message;

            sendTweet(message);
        break;

        case 'f':
            cout << "Who do you want to follow?" << endl;
            cout << ">> ";

            cin >> name;

            sendFollow(name);
        break;

        case 'u':
            cout << "Who's isn't interesting any more?" << endl;
            cout << ">> ";

            cin >> name;

            sendUnfollow(name);
        break;
    }

    cout << "Request completed" << endl;

    Sleep(500);

    showTweetScreen();
}

void Client::printMenu() {
    cout << "What to do next?" << endl << endl;

    cout << "[t] ... tweet" << endl;
    cout << "[f] ... follow" << endl;
    cout << "[u] ... unfollow" << endl;

    cout << endl << endl;
}

void Client::sendLogin(char* name) {

    string buffer;
    buffer.append("n");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}

void Client::sendTweet(char* message) {
    string buffer;
    buffer.append("t");
    buffer.append(message);

    const char* tweet;
    tweet = buffer.c_str();

    send(socketFileDescriptor, tweet, TWEET_LENGTH + 1, 0); 
}

void Client::sendFollow(char* name) {
    string buffer;
    buffer.append("f");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}

void Client::sendUnfollow(char* name) {
    string buffer;
    buffer.append("u");
    buffer.append(name);

    const char* message;
    message = buffer.c_str();

    send(socketFileDescriptor, message, USERNAME_LENGTH + 1, 0);
}

void Client::initializeWindowsSockets() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(1, 1), &wsa)) {
        throw "Couldn't initialize Windows Sockets (WSAStartup failed)";
    }
}