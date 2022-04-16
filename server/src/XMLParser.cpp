//
// Created by Xiaodong Liu on 3/24/22.
//

#include "XMLParser.h"

XMLParser::XMLParser(vector<char> _xmlBuffer, shared_ptr <MyData> _myData) : myData(_myData) {
    _xmlBuffer.emplace_back('\0');
    pugi::xml_parse_result result = this->reqDoc.load_string(_xmlBuffer.data());
    if (result) {
        //cout << "XML [" << vectorToStr(_xmlBuffer) << "] parsed without errors." << endl;
    } else {
        throw MyException("Fail to parse the xml request.");
    }
    //printReq();
    resNode = resDoc.append_child(RESULTS_NODE);
}

void XMLParser::parse() {
    if (reqDoc.child("create")) {
        create();
    } else if (reqDoc.child("transactions")) {
        transaction();
    } else {
        throw MyException("Fail to parse the xml request.");
    }
    //printRes();
}

void XMLParser::createAccount(pugi::xml_node accountNode) {
    pugi::xml_attribute attrId = accountNode.attribute(ID_ATTR);
    pugi::xml_attribute attrBalance = accountNode.attribute(BALANCE_ATTR);
    const char *id = attrId.value();
    const char *balance = attrBalance.value();
    string msg = myData->createAccount(string(id), string(balance));
    if (msg.empty()) {
//        cout << "Successfully created account" << endl;
        pugi::xml_node createdNode = resNode.append_child(CREATED_NODE);
        createdNode.append_attribute(ID_ATTR).set_value(id);
    } else {
        pugi::xml_node errNode = resNode.append_child(ERROR_NODE);
        errNode.append_attribute(ID_ATTR).set_value(id);
        errNode.text().set(msg.c_str());
    }
}

void XMLParser::createPosition(const char *symName, pugi::xml_node symChildNode) {

    pugi::xml_attribute attrId = symChildNode.attribute(ID_ATTR);
    const char *accountId = attrId.value();
    const char *amount = symChildNode.child_value();
    string msg = myData->createPosition(string(accountId), string(symName), string(amount));
    if (msg.empty()) {
//        cout << "Successfully added symbols to account" << endl;
        pugi::xml_node createdNode = resNode.append_child(CREATED_NODE);
        createdNode.append_attribute(SYM_ATTR).set_value(symName);
        createdNode.append_attribute(ID_ATTR).set_value(accountId);
    } else {
        pugi::xml_node errNode = resNode.append_child(ERROR_NODE);
        errNode.append_attribute(SYM_ATTR).set_value(symName);
        errNode.append_attribute(ID_ATTR).set_value(accountId);
        errNode.text().set(msg.c_str());
    }
}

void XMLParser::create() {
    pugi::xml_node createNode = reqDoc.child(CREATE_NODE);
    for (pugi::xml_node child: createNode) {
//        cout << child.name() << endl;
        if (strcmp(child.name(), ACCOUNT_NODE) == 0) {
            createAccount(child);
        } else if (strcmp(child.name(), SYMBOL_NODE) == 0) {
            const char *symName = child.attribute(SYM_ATTR).value();
            for (pugi::xml_node symbolChild: child) {
                createPosition(symName, symbolChild);
            }
        }
    }
}

void XMLParser::order(const char *accountId, pugi::xml_node orderNode) {
    pugi::xml_attribute symAttr = orderNode.attribute(SYM_ATTR);
    pugi::xml_attribute amountAttr = orderNode.attribute(AMOUNT_ATTR);
    pugi::xml_attribute limitAttr = orderNode.attribute(LIMIT_ATTR);
    const char *sym = symAttr.value();
    const char *amount = amountAttr.value();
    const char *limit = limitAttr.value();
    auto msg_orderid = myData->order(string(accountId), string(sym), string(amount), string(limit));
    if (msg_orderid.first.empty()) {
//        cout << "successfully execute an order" << endl;
        pugi::xml_node openedNode = resNode.append_child(OPENED_NODE);
        openedNode.append_attribute(SYM_ATTR).set_value(sym);
        openedNode.append_attribute(AMOUNT_ATTR).set_value(amount);
        openedNode.append_attribute(LIMIT_ATTR).set_value(limit);
        openedNode.append_attribute(ID_ATTR).set_value(msg_orderid.second.c_str());
    } else {
        pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
        errorNode.append_attribute(SYM_ATTR).set_value(sym);
        errorNode.append_attribute(AMOUNT_ATTR).set_value(amount);
        errorNode.append_attribute(LIMIT_ATTR).set_value(limit);
        errorNode.text().set(msg_orderid.first.c_str());
    }
}

void XMLParser::query(const char *accountId, pugi::xml_node queryNode) {
    pugi::xml_attribute idAttr = queryNode.attribute(ID_ATTR);
    const char *orderId = idAttr.value();
    Order order1 = myData->query(string(accountId), string(orderId));
    if (order1.msg.empty()) {
//        cout << "successfully execute a query transaction" << endl;
        pugi::xml_node statusNode = resNode.append_child(STATUS_NODE);
        statusNode.append_attribute(ID_ATTR).set_value(orderId);
        if (!order1.openShares.empty()) {
            pugi::xml_node openNode = statusNode.append_child(OPEN_NODE);
            openNode.append_attribute(SHARES_ATTR).set_value(order1.openShares.c_str());
        } else if (!order1.canceledShares.first.empty()) {
            pugi::xml_node canceledNode = statusNode.append_child(CANCELED_NODE);
            canceledNode.append_attribute(SHARES_ATTR).set_value(order1.canceledShares.first.c_str());
            canceledNode.append_attribute(TIME_ATTR).set_value(order1.canceledShares.second.c_str());
        }
        if (!order1.executedShares.empty()) {
            auto it = order1.executedShares.begin();
            while (it != order1.executedShares.end()) {
                pugi::xml_node executedNode = statusNode.append_child(EXECUTED_NODE);
                executedNode.append_attribute(SHARES_ATTR).set_value((*it)[2].c_str());
                executedNode.append_attribute(PRICE_ATTR).set_value((*it)[1].c_str());
                executedNode.append_attribute(TIME_ATTR).set_value((*it)[0].c_str());
                ++it;
            }
        }
    } else {
        pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
        errorNode.append_attribute(ID_ATTR).set_value(orderId);
        errorNode.text().set(order1.msg.c_str());
    }
}

void XMLParser::cancel(const char *accountId, pugi::xml_node cancelNode) {
    pugi::xml_attribute idAttr = cancelNode.attribute(ID_ATTR);
    const char *orderId = idAttr.value();
    Order order1 = myData->cancel(string(accountId), string(orderId));
    if (order1.msg.empty()) {
        pugi::xml_node canceledNode = resNode.append_child(CANCELED_NODE);
        canceledNode.append_attribute(ID_ATTR).set_value(orderId);
        pugi::xml_node childCanceledNode = canceledNode.append_child(CANCELED_NODE);
        childCanceledNode.append_attribute(SHARES_ATTR).set_value(order1.canceledShares.first.c_str());
        childCanceledNode.append_attribute(TIME_ATTR).set_value(order1.canceledShares.second.c_str());
        if (!order1.executedShares.empty()) {
            auto it = order1.executedShares.begin();
            while (it != order1.executedShares.end()) {
                pugi::xml_node executedNode = canceledNode.append_child(EXECUTED_NODE);
                executedNode.append_attribute(SHARES_ATTR).set_value((*it)[2].c_str());
                executedNode.append_attribute(PRICE_ATTR).set_value((*it)[1].c_str());
                executedNode.append_attribute(TIME_ATTR).set_value((*it)[0].c_str());
                ++it;
            }
        }
    } else {
        pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
        errorNode.append_attribute(ID_ATTR).set_value(orderId);
        errorNode.text().set(order1.msg.c_str());
    }
}

void XMLParser::orderError(const char *msg, pugi::xml_node orderNode) {
    pugi::xml_attribute symAttr = orderNode.attribute(SYM_ATTR);
    pugi::xml_attribute amountAttr = orderNode.attribute(AMOUNT_ATTR);
    pugi::xml_attribute limitAttr = orderNode.attribute(LIMIT_ATTR);
    const char *sym = symAttr.value();
    const char *amount = amountAttr.value();
    const char *limit = limitAttr.value();
    pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
    errorNode.append_attribute(SYM_ATTR).set_value(sym);
    errorNode.append_attribute(AMOUNT_ATTR).set_value(amount);
    errorNode.append_attribute(LIMIT_ATTR).set_value(limit);
    errorNode.text().set(msg);
}

void XMLParser::queryError(const char *msg, pugi::xml_node queryNode) {
    pugi::xml_attribute idAttr = queryNode.attribute(ID_ATTR);
    const char *orderId = idAttr.value();
    pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
    errorNode.append_attribute(ID_ATTR).set_value(orderId);
    errorNode.text().set(msg);
}

void XMLParser::cancelError(const char *msg, pugi::xml_node cancelNode) {
    pugi::xml_attribute idAttr = cancelNode.attribute(ID_ATTR);
    const char *orderId = idAttr.value();
    pugi::xml_node errorNode = resNode.append_child(ERROR_NODE);
    errorNode.append_attribute(ID_ATTR).set_value(orderId);
    errorNode.text().set(msg);
}

void XMLParser::transaction() {
    pugi::xml_node transactionsNode = reqDoc.child(TRANSACTIONS_NODE);
    const char *accountId = transactionsNode.attribute(ID_ATTR).value();
    std::unique_lock <std::mutex> munique(myData->mtx, std::defer_lock);
    munique.lock();
    string msg = myData->accountNonExistError(string(accountId));
    munique.unlock();
    if (!msg.empty()) {
        for (pugi::xml_node child: transactionsNode) {
//            cout << child.name() << endl;
            if (strcmp(child.name(), ORDER_NODE) == 0) {
                orderError(msg.c_str(), child);
            } else if (strcmp(child.name(), QUERY_NODE) == 0) {
                queryError(msg.c_str(), child);
            } else if (strcmp(child.name(), CANCEL_NODE) == 0) {
                cancelError(msg.c_str(), child);
            } else {
                cout << "error: " << child.name() << endl;
            }
        }
    } else {
        for (pugi::xml_node child: transactionsNode) {
//            cout << child.name() << endl;
            if (strcmp(child.name(), ORDER_NODE) == 0) {
                order(accountId, child);
            } else if (strcmp(child.name(), QUERY_NODE) == 0) {
                query(accountId, child);
            } else if (strcmp(child.name(), CANCEL_NODE) == 0) {
                cancel(accountId, child);
            } else {
                cout << "error: " << child.name() << endl;
            }
        }
    }
}

void XMLParser::printRes() {
    //std::cout << "\nWrite result xml doc to stdout with indent of 4 char:" << std::endl;
    resDoc.save(std::cout, "    ");
}

void XMLParser::printReq() {
    //std::cout << "\nWrite request xml doc to stdout with indent of 4 char:" << std::endl;
    reqDoc.save(std::cout, "    ");
}

string XMLParser::reqToStr() {
    //std::cout << "\nWrite request xml doc to stringstream with indent of 4 chars:" << std::endl;
    std::stringstream ss;
    reqDoc.save(ss, "    ");
    return ss.str();
}

string XMLParser::resToStr() {
    //std::cout << "\nWrite result xml doc to stringstream with indent of 4 chars:" << std::endl;
    std::stringstream ss;
    resDoc.save(ss, "    ");
    return ss.str();
}