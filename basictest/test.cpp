//
// Created by Xiaodong Liu on 3/24/22.
//


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "ClientSocket.h"
#include "Common.h"

int main(int argc, char *argv[]) {
    vector <string> xmls;
    string hostname = argv[1];
    for (int i = 2; i < argc; ++i) {
        xmls.emplace_back(readFile(argv[i]));
    }
    cout << hostname << endl;
    string port = "12345";
    ClientSocket clientSocket(strToVector(hostname), strToVector(port));
    try {
        clientSocket.setSocket();
        clientSocket.toConnect();
        for (int i = 2; i < argc; ++i) {
            clientSocket.toSend(strToVector(addLenAsHeader(xmls[i - 2])));
            auto response = clientSocket.toReceiveXML();
            printVector(Buffer::removeHeader(response.second));
        }
    } catch (MyException &exp) {
        cerr << exp.what() << endl;
    }
    return EXIT_SUCCESS;
}