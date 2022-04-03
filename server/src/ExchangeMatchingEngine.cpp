//
// Created by Xiaodong Liu on 3/21/22.
//

#include "ExchangeMatchingEngine.h"

size_t ExchangeMatchingEngine::threadpoolsize = 5;

ExchangeMatchingEngine::ExchangeMatchingEngine() : myData(make_shared<MyData>()),
                                                   serverSocket(make_unique<ServerSocket>(
                                                           strToVector(PORT), BACKLOG)) {
}

void ExchangeMatchingEngine::run() {
    cout << "Engine running" << endl;
    myData->createTables();
    myData->cleanAllData();
    string startID = myData->getMaxOrderID();
//    cout << "startID: " << startID << endl;
    myData->setUniqueID(startID);

    serverSocket->setUp();

    //thread pool.
    ctpl::thread_pool p(threadpoolsize);
    while (true) {
        //accept request from client
        int clientFd = serverSocket->toAccept();
        p.push(handleNewRequest, clientFd, this->myData);
        //handleNewRequest(clientFd);
    }
}

void ExchangeMatchingEngine::handleNewRequest(int id, int clientFd, shared_ptr <MyData> myData) {
    vector<char> buffer;
    while (true) {
        try {
            buffer = ServerSocket::toReceiveXML(clientFd);
            if (buffer.empty()) {
                return;
            }
            buffer = Buffer::removeHeader(buffer);
            XMLParser xmlParser(buffer, myData);
            xmlParser.parse();
            ServerSocket::toResponse(clientFd, Buffer::addHeader(strToVector(xmlParser.resToStr())));
        } catch (MyException &exp) {
            std::cout << exp.what();
            break;
        }
    }
}

int main(int argc, char *argv[]) {
//    const auto processor_count = std::thread::hardware_concurrency();
//    cout << "processor_count: " << processor_count << endl;
    if (argc > 2) {
        ExchangeMatchingEngine::threadpoolsize = constCharToInt(argv[2]);
        cout << "The size of thread pool is: " << ExchangeMatchingEngine::threadpoolsize << endl;
    }
    if (argc > 1) {
        MyData::host = argv[1];
        cout << "database host: " << MyData::host << endl;
    }
    try {
        ExchangeMatchingEngine exchangeMatchingEngine;
        exchangeMatchingEngine.run();
    } catch (const std::exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}