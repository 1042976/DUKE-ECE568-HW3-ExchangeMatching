//
// Created by Xiaodong Liu on 3/23/22.
//

#ifndef ERSS_HWK4_MYDATA_H
#define ERSS_HWK4_MYDATA_H

#include <pqxx/pqxx>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>
#include <fstream>
#include <mutex>
#include <ctime>
#include <unordered_map>
#include "pugixml.hpp"
#include "MyException.h"
#include "Order.h"
#include "MyTimer.h"
#include "UniqueID.h"
using namespace std;
using namespace pqxx;
extern string PORT;
extern int BACKLOG;
extern size_t FIRSTBUFFERLEN;
extern string DATABASE;

extern const char *CREATE_NODE;
extern const char *ACCOUNT_NODE;
extern const char *ID_ATTR;
extern const char *BALANCE_ATTR;
extern const char *SYMBOL_NODE;
extern const char *SYM_ATTR;

extern const char *TRANSACTIONS_NODE;
extern const char *ORDER_NODE;
extern const char *QUERY_NODE;
extern const char *CANCEL_NODE;
extern const char *AMOUNT_ATTR;
extern const char *LIMIT_ATTR;


extern const char *RESULTS_NODE;
extern const char *CREATED_NODE;
extern const char *ERROR_NODE;
extern const char *OPENED_NODE;
extern const char *STATUS_NODE;
extern const char *OPEN_NODE;
extern const char *CANCELED_NODE;
extern const char *EXECUTED_NODE;
extern const char *SHARES_ATTR;
extern const char *PRICE_ATTR;
extern const char *TIME_ATTR;

vector<char> strToVector(string str);

string vectorToStr(vector<char> arr);

void standardizeVector(vector<char> &buffer);

void setZeroInTheEnd(vector<char> &buffer);

int findChar(const vector<char> &A, char target);

int findChar(const vector<char> &A, char target, size_t left);

int findAnyCharInSubStr(const vector<char> &A, vector<char> target);

string addApostrophe(string str);

string addInt(string str, int a);

string getCurrentTime();

void printVector(vector<char> arr);

double moneyToDouble(string money);

bool isAlphanumeric(string str);

bool isNumeric(string str);

int constCharToInt(const char *str);

//===============================================================================
class MyData {
private:
    shared_ptr <connection> C;
    UniqueID uniqueID;

    void createTableAccount();

    void createTablePosition();

    void createTableOrder();

    void createTableExecuted();

    void createTableCanceled();

    //helper function
    string accountExistError(string accountId);

    string deductFromAccount(string accountId, int shares, double limit);

    string deductFromPosition(string accountId, string sym, int shares);

    pqxx::result getPosition(string accountId, string sym);

    pqxx::result getOrder(string orderId);

    double getBalance(string accountId);

    int getAmountOfSymbol(string accountID, string symbol);

    pqxx::result getCanceledOrder(string orderId);

    pqxx::result getExecutedOrder(string orderId);

    //order execution
    void execOrder(string orderID);

    pqxx::result getSellOrder(string orderID, string accountID, string symbol, double limitPrice);

    pqxx::result getBuyOrder(string orderID, string accountID, string symbol, double limitPrice);

    bool execBuyToSellOrder(string buyOrderID, string sellOrderID);

    bool execSellToBuyOrder(string sellOrderID, string buyOrderID);

    void addBalance(string accountID, double a);

    void addAmountOfSymbol(string accountID, string symbol, int a);

    void updateOpenShares(string orderID, int u);

    void insertExecuted(string orderID, int shares, double price);

public:
    std::mutex mtx;
    static std::string host;

    MyData();

    ~MyData();

    void createTables();

    void cleanAllData();

    string createAccount(string id, string balance);

    string createPosition(string accountId, string sym, string amount);

    pair <string, string> order(string accountId, string sym, string shares, string limit);

    Order query(string accountId, string orderId);

    Order cancel(string accountId, string orderId);

    //helper function
    string accountNonExistError(string accountId);

    string getMaxOrderID();

    void setUniqueID(string targetID);
};


#endif //ERSS_HWK4_MYDATA_H
