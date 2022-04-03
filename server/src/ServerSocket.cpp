//
// Created by Xiaodong Liu on 3/24/22.
//
#include "ServerSocket.h"

ServerSocket::ServerSocket(vector<char> _port, int _backlog) : fd(-1), backlog(_backlog), port(_port) {}


int ServerSocket::getFd() {
    return fd;
}

int ServerSocket::getBacklog() {
    return backlog;
}

void ServerSocket::setSocket() {
    memset(&hints, 0, sizeof(hints));
    string error_message = "Error: Unable to create a server socket!";
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    setZeroInTheEnd(port);
    if (getaddrinfo(hostname.data(), port.data(), &hints, &servinfo) != 0) {
        throw MyException("Error: Unable to create a server socket!");
    }
    fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (fd == -1) {
        throw MyException("Error: Unable to create a server socket!");
    }
}

void ServerSocket::toSetSockOpt() {
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
        throw MyException("Error: Fail to set socket option!");
    }
}

void ServerSocket::toBind() {
    if (bind(fd, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
        throw MyException("Error: Fail to bind");
    }
}

void ServerSocket::toListen() {
    if (listen(fd, backlog) != 0) {
        throw MyException("Error: Fail to listen");
    }
    cout << "listening on port " << vectorToStr(this->port) << endl;
}

void ServerSocket::setUp() {
    setSocket();
    toSetSockOpt();
    toBind();
    toListen();
}
int ServerSocket::toAccept() {
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    if ((new_socket = accept(fd, (struct sockaddr *) &address,
                             (socklen_t * ) & addrlen)) < 0) {
        throw MyException("Error: Fail to accept new socket");
    }
    return new_socket;
}

vector<char> ServerSocket::toReceive(int clientFd) {
    vector<char> buffer(MAXBUFFERLEN);
    int bufferlen = recv(clientFd, buffer.data(), buffer.size(), 0);
    if (bufferlen < 0) {
        throw MyException("Error: fail to receive client request");
    } else if (bufferlen == 0) {
        return vector < char > {};
    }
    standardizeVector(buffer);
//    printVector(buffer);
    return buffer;
}

vector<char> ServerSocket::toReceiveXML(int clientFd) {
    vector<char> buffer(MAXBUFFERLEN);
    int bufferLen = recv(clientFd, buffer.data(), buffer.size(), 0);
    if (bufferLen < 0) {
        throw MyException("Error: fail to receive client request");
    } else if (bufferLen == 0) {
        return vector < char > {};
    }
    standardizeVector(buffer);
    size_t targetLen = Buffer::getWholeBufferLength(buffer); //not take '\0' into account
    while(buffer.size() < targetLen){
        int oldSize = buffer.size();
        buffer.resize(oldSize+MAXBUFFERLEN);
        bufferLen = recv(clientFd, &(buffer.data()[oldSize]), MAXBUFFERLEN, 0);
        if (bufferLen < 0) {
            throw MyException("Error: fail to receive client request");
        }
        standardizeVector(buffer);
    }
    return buffer;
}

void ServerSocket::toResponse(int clientFd, vector<char> response) {
    int status = 0;
    setZeroInTheEnd(response);
    status = send(clientFd, response.data(), response.size(), 0);
    if(status == -1){
        throw MyException("Fail to send response to the client!");
    }
}