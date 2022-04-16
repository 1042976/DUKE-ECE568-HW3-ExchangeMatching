//
// Created by Xiaodong Liu on 3/24/22.
//

#ifndef ERSS_HWK4_XMLPARSER_H
#define ERSS_HWK4_XMLPARSER_H

#include <vector>
#include <unistd.h>
#include "pugixml.hpp"
#include "MyData.h"
#include "Order.h"
using namespace std;

class XMLParser {
private:
    shared_ptr <MyData> myData;
    pugi::xml_document reqDoc;
    pugi::xml_document resDoc;
    pugi::xml_node resNode;

//create-----------------------
    void createAccount(pugi::xml_node accountNode);

    void createPosition(const char *symName, pugi::xml_node symChildNode);

    void create();

//-----------------------------
//transaction------------------
    void order(const char *accountId, pugi::xml_node orderNode);

    void orderError(const char *msg, pugi::xml_node orderNode);

    void query(const char *accountId, pugi::xml_node queryNode);

    void queryError(const char *msg, pugi::xml_node queryNode);

    void cancel(const char *accountId, pugi::xml_node cancelNode);

    void cancelError(const char *msg, pugi::xml_node cancelNode);

    void transaction();

//-----------------------------
    void printRes();

    void printReq();

public:
    XMLParser(vector<char> _xmlBuffer, shared_ptr <MyData> _myData);

    void parse();

    string resToStr();

    string reqToStr();
};

#endif //ERSS_HWK4_XMLPARSER_H
