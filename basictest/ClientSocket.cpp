//
// Created by Xiaodong Liu on 3/29/22.
//
#include "ClientSocket.h"

int ClientSocket::getServerFd() {
    return serverFd;
}

vector<char> ClientSocket::getHostName() {
    return hostname;
}

vector<char> ClientSocket::getPort() {
    return port;
}

ClientSocket::ClientSocket(vector<char> _hostname, vector<char> _port) : hostname(_hostname), port(_port) {}

void ClientSocket::setSocket() {
    int status = 0;
    memset(&hints, 0, sizeof(hints));
    string error_message = "Error: Unable to create a client socket!";
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    setZeroInTheEnd(hostname);
    setZeroInTheEnd(port);
    status = getaddrinfo(hostname.data(), port.data(), &hints, &servinfo);
    if (status != 0) {
        throw MyException(error_message);
    }
    serverFd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (serverFd == -1) {
        throw MyException(error_message);
    }
}

void ClientSocket::toConnect() {
    int status = 0;
    status = connect(serverFd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status == -1) {
        throw MyException("Error: Fail to connect to the original server!");
    }
    freeaddrinfo(servinfo);
}

void ClientSocket::toSend(vector<char> allContent) {
    //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
    int status = 0;
    status = send(serverFd, allContent.data(), allContent.size(), 0);
    if (status == -1) {
        throw MyException("Error: Fail to send request content to the original server!");
    }
}

//pair<int, vector<char> > ClientSocket::toReceive() {
//    int curLen = 0;
//    vector<char> buffer(MAXBUFFERLEN, '\0');
//    curLen = recv(serverFd, buffer.data(), buffer.size(), 0);
//    if (curLen == -1) {
//        throw MyException("Error: Fail to receive response from the original server!");
//    }
//    standardizeVector(buffer);
//    return make_pair(curLen, buffer);
//}

pair<int, vector<char>> ClientSocket::toReceiveXML() {
    vector<char> buffer(FIRSTBUFFERLEN);
    int bufferLen = recv(serverFd, buffer.data(), buffer.size(), 0);
    if (bufferLen == -1) {
        throw MyException("Error: Fail to receive response from the original server!");
    }
    int targetLen = Buffer::getWholeBufferLength(buffer); //not take '\0' into account
    buffer.resize(targetLen);
    int curLen = bufferLen;
    cout << "--------------------------------" << endl;
    cout << "curLen: " << curLen << endl;
    cout << "targetLen: " << targetLen << endl;
    cout << "--------------------------------" << endl;
    while (curLen < targetLen) {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "curLen: " << curLen << endl;
        cout << "targetLen: " << targetLen << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        bufferLen = recv(serverFd, &(buffer.data()[curLen]), targetLen-curLen, 0);
        if(bufferLen < 0){
            throw MyException("Error: fail to receive client request");
        }
        curLen += bufferLen;
        cout << " while (curLen < targetLen) " << endl;
    }
    return make_pair(buffer.size(), buffer);
}