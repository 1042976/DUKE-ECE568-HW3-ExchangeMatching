//
// Created by Xiaodong Liu on 3/29/22.
//

#ifndef ERSS_HWK4_CLIENTSOCKET_H
#define ERSS_HWK4_CLIENTSOCKET_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <memory>
#include <utility>
#include <vector>
#include "string"
#include "cstring"
#include "MyException.h"
#include "Common.h"
#include "Buffer.h"

using namespace std;

class ClientSocket {
private:
    int serverFd;
    struct addrinfo hints, *servinfo;
    std::vector<char> hostname, port;

public:
    ClientSocket(vector<char> _hostname, vector<char> _port);

    int getServerFd();

    vector<char> getHostName();

    vector<char> getPort();

    void setSocket();

    void toConnect();

    void toSend(vector<char> allContent);

    pair<int, vector<char>> toReceive();

    pair<int, vector<char>> toReceiveXML();
};

#endif //ERSS_HWK4_CLIENTSOCKET_H
