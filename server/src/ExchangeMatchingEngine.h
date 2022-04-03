//
// Created by Xiaodong Liu on 3/21/22.
//

#ifndef ERSS_HWK4_EXCHANGEMATCHINGENGINE_H
#define ERSS_HWK4_EXCHANGEMATCHINGENGINE_H

#include <pqxx/pqxx>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include <thread>
#include "ctpl_stl.h"
#include "MyException.h"
#include "MyData.h"
#include "ServerSocket.h"
#include "XMLParser.h"
#include "UniqueID.h"

using namespace std;

class ExchangeMatchingEngine {
private:
    shared_ptr <MyData> myData;
    unique_ptr <ServerSocket> serverSocket;
public:
    ExchangeMatchingEngine();

    void run();

    static void handleNewRequest(int id, int clientFd, shared_ptr <MyData> myData);

    static size_t threadpoolsize;
};


#endif //ERSS_HWK4_EXCHANGEMATCHINGENGINE_H
