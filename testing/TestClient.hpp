#ifndef __TEST_CLIENT_HPP__
#define __TEST_CLIENT_HPP__

#include <vector>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "util.hpp"

class TestClient {
public:
    TestClient();
    bool connectToServer(const char* hostname, const char* port);
    bool sendMsg(const std::string& payload);
    void sendAndRecv(const std::string& payload);
    void close();

    std::vector<char> recvSomeMsg();
    std::vector<char> recvMsg();

private:
    int socket_fd;
    boost::asio::io_context ctx;
    boost::asio::ip::tcp::socket sock;
    bool connected;
};

#endif
