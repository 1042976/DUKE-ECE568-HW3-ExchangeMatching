//
// Created by Xiaodong Liu on 3/23/22.
//
#include "MyData.h"

string PORT = "12345";
int BACKLOG = 100;
size_t FIRSTBUFFERLEN = 1024;
string DATABASE = "exchange_matching";

//"create" xml
const char *CREATE_NODE = "create";
const char *ACCOUNT_NODE = "account";
const char *ID_ATTR = "id";
const char *BALANCE_ATTR = "balance";
const char *SYMBOL_NODE = "symbol";
const char *SYM_ATTR = "sym";

//"transaction xml"
const char *TRANSACTIONS_NODE = "transactions";
const char *ORDER_NODE = "order";
const char *QUERY_NODE = "query";
const char *CANCEL_NODE = "cancel";
const char *AMOUNT_ATTR = "amount";
const char *LIMIT_ATTR = "limit";

//"result xml"
const char *RESULTS_NODE = "results";
const char *CREATED_NODE = "created";
const char *ERROR_NODE = "error";
const char *OPENED_NODE = "opened";
const char *STATUS_NODE = "status";
const char *OPEN_NODE = "open";
const char *CANCELED_NODE = "canceled";
const char *EXECUTED_NODE = "executed";
const char *SHARES_ATTR = "shares";
const char *PRICE_ATTR = "price";
const char *TIME_ATTR = "time";


vector<char> strToVector(string str) {
    return vector<char>(str.begin(), str.end());
}

string vectorToStr(vector<char> arr) {
    return string(arr.begin(), arr.end());
}

void standardizeVector(vector<char> &buffer) {
    size_t actual_len = 0;
    while (actual_len < buffer.size() && buffer[actual_len] != '\0') {
        ++actual_len;
    }
    buffer.resize(actual_len);
}

void setZeroInTheEnd(vector<char> &buffer) {
    int firstZeroIdx = findChar(buffer, '\0');
    if (firstZeroIdx == -1) {
        buffer.emplace_back('\0');
    }
}


int findChar(const vector<char> &A, char target) {
    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i] == target) {
            return i;
        }
    }
    return -1;
}

int findChar(const vector<char> &A, char target, size_t left) {
    if (left >= A.size()) {
        return -1;
    }
    vector<char> tmp(A.begin() + left, A.end());
    int k = findChar(tmp, target);
    return k < 0 ? -1 : left + k;
}

int findAnyCharInSubStr(const vector<char> &A, vector<char> target) {
    int min_index = INT_MAX;
    for (size_t i = 0; i < target.size(); ++i) {
        int cur = findChar(A, target[i]);
        if (cur == -1) {
            continue;
        }
        min_index = min(min_index, cur);
    }
    return min_index == INT_MAX ? -1 : min_index;
}

string addApostrophe(string str) {
    return "\'" + str + "\'";
}

string addInt(string str, int a) {
    return to_string(stoi(str) + a);
}

string getCurrentTime() {
    time_t now = time(NULL);
    tm *gmtm = gmtime(&now);
    char *dt = asctime(gmtm);
    string dt_str = "";
    char *p = dt;
    while (*p != '\n') {
        dt_str.push_back(*p);
        ++p;
    }
    return dt_str;
}

double moneyToDouble(string money) {
    string res = "";
    for (const auto &x: money) {
        if (isdigit(x) || x == '.') {
            res.push_back(x);
        }
    }
    return stod(res);
}

void printVector(vector<char> arr) {
    if (arr.empty()) {
        cout << endl;
        return;
    }
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i];
    }
    cout << endl;
}

bool isAlphanumeric(string str) {

    for (const auto &c: str) {
        if (!isdigit(c) && !isalpha(c)) {
            return false;
        }
    }
    return true;
}

bool isNumeric(string str) {

    for (const auto &c: str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int constCharToInt(const char *str) {
    int res;
    stringstream s(str);
    s >> res;
    return res;
}

//===============================================================================
std::string MyData::host = "localhost";

MyData::MyData() : C(make_shared<connection>("dbname = exchange_matching user = postgres password = postgres "
                                             "host = " + MyData::host + " port = 5432")) {
    if (C->is_open()) {
        cout << "Successfully connect to the database: " << C->dbname() << endl;
    } else {
        throw MyException("Unable to open the database");
    }
}

MyData::~MyData() {
    C->disconnect();
}

void MyData::createTableAccount() {
    work W(*C);
    string sql = "CREATE TABLE IF NOT EXISTS ACCOUNT("
                 "ID        TEXT    PRIMARY KEY   NOT NULL,"
                 "BALANCE   MONEY     NOT NULL);";
    W.exec(sql);
    W.commit();
}

void MyData::createTablePosition() {
    work W(*C);
    string sql = "CREATE TABLE IF NOT EXISTS POSITION("
                 "ACCOUNT_ID    TEXT    REFERENCES ACCOUNT(ID),"
                 "SYMBOL        TEXT    NOT NULL,"
                 "AMOUNT        INT     NOT NULL);";
    W.exec(sql);
    W.commit();
}

void MyData::createTableOrder() {
    work W(*C);
    string sql = "CREATE TABLE IF NOT EXISTS ORDERS("
                 "ID                BIGINT          PRIMARY KEY     NOT NULL,"
                 "ACCOUNT_ID        TEXT            REFERENCES ACCOUNT(ID),"
                 "TRANSACTION_TYPE  CHAR            NOT NULL,"
                 "SYMBOL            TEXT            NOT NULL,"
                 "SHARES            INT             NOT NULL,"
                 "OPEN_SHARES       INT             NOT NULL,"
                 "LIMIT_PRICE       MONEY           NOT NULL,"
                 "ISCANCELED        BOOLEAN         NOT NULL);";
    W.exec(sql);
    W.commit();
}

void MyData::createTableExecuted() {
    work W(*C);
    string sql = "CREATE TABLE IF NOT EXISTS EXECUTED("
                 "ORDER_ID    BIGINT          REFERENCES ORDERS(ID),"
                 "SHARES      INT             NOT NULL,"
                 "PRICE       MONEY           NOT NULL,"
                 "TIME        BIGINT          NOT NULL);";
    W.exec(sql);
    W.commit();
}

void MyData::createTableCanceled() {
    work W(*C);
    string sql = "CREATE TABLE IF NOT EXISTS CANCELED("
                 "ORDER_ID    BIGINT       REFERENCES ORDERS(ID),"
                 "SHARES      INT          NOT NULL,"
                 "TIME        BIGINT       NOT NULL);";
    W.exec(sql);
    W.commit();
}

void MyData::createTables() {
    createTableAccount();
    createTablePosition();
    createTableOrder();
    createTableExecuted();
    createTableCanceled();
}

void MyData::cleanAllData() {
    work W(*C);
    string sqlExecuted = "DELETE FROM EXECUTED";
    string sqlCanceled = "DELETE FROM CANCELED";
    string sqlPosition = "DELETE FROM POSITION";
    string sqlOrders = "DELETE FROM ORDERS";
    string sqlAccount = "DELETE FROM ACCOUNT";
    W.exec(sqlExecuted);
    W.exec(sqlCanceled);
    W.exec(sqlPosition);
    W.exec(sqlOrders);
    W.exec(sqlAccount);
    W.exec(sqlPosition);
    W.commit();
}


string MyData::createAccount(string id, string balance) {
    //don't have to lock here
    std::lock_guard <std::mutex> lck(mtx);
    pqxx::result r;
    string msg = "";
    //first check if the account already existed
    msg = accountExistError(id);
    if (!msg.empty()) {
        return msg;
    }
    //check if the account id is numeric
    if (id.empty() || !isNumeric(id)) {
        return "The given account id is empty or contains non-digital characters";
    }
    work W(*C);
    string sql = "INSERT INTO ACCOUNT (ID, BALANCE) "
                 "VALUES(" + addApostrophe(id) + "," + balance + ");";
    W.exec(sql);
    W.commit();
    return "";
}

string MyData::createPosition(string accountId, string sym, string amount) {
    //don't have to lock here
    std::lock_guard <std::mutex> lck(mtx);
    //nontransaction N(*C);
    string sql = "";
    pqxx::result r;
    string msg;
    //first check if the account to add symbols exist
    msg = accountNonExistError(accountId);
    if (!msg.empty()) {
        return msg;
    }
    //check if symbol is alphanumeric
    if (sym.empty() || !isAlphanumeric(sym)) {
        return "The given symbol is empty or contains non-alphanumeric characters";
    }
    //check if the position already existed
    r = getPosition(accountId, sym);
    sym = addApostrophe(sym);
    accountId = addApostrophe(accountId);
    work W(*C);
    if (!r.empty()) {
        amount = addInt(amount, r[0][0].as<int>());
        sql = "UPDATE POSITION SET AMOUNT = " + amount + " WHERE "
                                                         "ACCOUNT_ID = " + accountId + " AND "
                                                                                       "SYMBOL = " + sym + ";";
    } else {
        sql = "INSERT INTO POSITION (ACCOUNT_ID, SYMBOL, AMOUNT) "
              "VALUES(" + accountId + "," + sym + "," + amount + ");";
    }
    W.exec(sql);
    W.commit();
    return "";
}

pair <string, string> MyData::order(string accountId, string sym, string amount, string limit) {
    std::lock_guard <std::mutex> lck(mtx);
    string msg = "";
    pqxx::result r;
    //first check if the account id exists
//    msg = accountNonExistError(accountId);
//    if(!msg.empty()){
//        return msg;
//    }
    //add order
    string transaction_type = amount[0] == '-' ? "s" : "b";
    string shares = amount;
    if (transaction_type == "s") {
        shares = shares.substr(1);
    }
    //if it is to buy, check if the balance of the account is enough. If enough, deduct from account. Otherwise return error.
    if (transaction_type == "b") {
        msg = deductFromAccount(accountId, stoi(shares), stod(limit));
        if (!msg.empty()) {
            return make_pair(msg, "");
        }
    } else {
        msg = deductFromPosition(accountId, sym, stoi(shares));
        if (!msg.empty()) {
            return make_pair(msg, "");
        }
    }
    work W(*C);
    string orderID = uniqueID.updateID();
    string sql =
            "INSERT INTO ORDERS (ID, ACCOUNT_ID, TRANSACTION_TYPE, SYMBOL, SHARES, OPEN_SHARES, LIMIT_PRICE, ISCANCELED) "
            "VALUES(" +
            orderID + "," +
            addApostrophe(accountId) + "," +
            addApostrophe(transaction_type) + "," +
            addApostrophe(sym) + "," +
            shares + "," +
            shares + "," +
            limit + "," +
            "FALSE" +
            ");";
    W.exec(sql);
    W.commit();
    execOrder(orderID);
    return make_pair("", orderID);
}
double MyData::getBalance(string accountId) {
    nontransaction N(*C);
    string sql = "SELECT BALANCE FROM ACCOUNT WHERE ID = " + addApostrophe(accountId) + ";";
    pqxx::result r(N.exec(sql));
    return moneyToDouble(r[0][0].as<string>());
}

void MyData::execOrder(string orderID) {
    pqxx::result r = getOrder(orderID);
    string accountID = r[0][1].as<string>();
    string transactionType = r[0][2].as<string>();
    string symbol = r[0][3].as<string>();
    double limitPrice = moneyToDouble(r[0][6].as<string>());

    string sql = "";
    if (transactionType == "b") {
        pqxx::result rS = getSellOrder(orderID, accountID, symbol, limitPrice);
//        cout << "buying: " << endl;
        bool finished = false;
        for (size_t i = 0; i < rS.size(); ++i) {
            finished = execBuyToSellOrder(orderID, rS[i][0].as<string>());
            if (finished) {
                break;
            }
        }
    } else {
        pqxx::result rB = getBuyOrder(orderID, accountID, symbol, limitPrice);
//        cout << "selling: " << endl;
        bool finished = false;
        for (size_t i = 0; i < rB.size(); ++i) {
            finished = execSellToBuyOrder(orderID, rB[i][0].as<string>());
            if (finished) {
                break;
            }
        }
    }
}

void MyData::addBalance(string accountID, double a) {
    double balance = getBalance(accountID);
    balance += a;
    work W(*C);
    string sql = "UPDATE ACCOUNT SET BALANCE = " + to_string(balance) +
                 " WHERE ID = " + addApostrophe(accountID) + ";";
    W.exec(sql);
    W.commit();
}

void MyData::updateOpenShares(string orderID, int u) {
    work W(*C);
    string sql = "UPDATE ORDERS SET OPEN_SHARES = " + to_string(u) +
                 " WHERE ID = " + orderID + ";";
    W.exec(sql);
    W.commit();
}

void MyData::insertExecuted(string orderID, int shares, double price) {
    work W(*C);
    string sql = "INSERT INTO EXECUTED (ORDER_ID, SHARES, PRICE, TIME) "
                 "VALUES(" +
                 orderID + "," +
                 to_string(shares) + "," +
                 to_string(price) + "," +
                 to_string(MyTimer::getElapsedMilliseconds()) + ");";
    W.exec(sql);
    W.commit();
}

bool MyData::execBuyToSellOrder(string buyOrderID, string sellOrderID) {
    pqxx::result rB = getOrder(buyOrderID);
    pqxx::result rS = getOrder(sellOrderID);

    string buyerAccountID = rB[0][1].as<string>();

    int buyerOpenShares = rB[0][5].as<int>();
    double buyerLimitPrice = moneyToDouble(rB[0][6].as<string>());

    string sellerAccountID = rS[0][1].as<string>();
    int sellerOpenShares = rS[0][5].as<int>();
    double sellerLimitPrice = moneyToDouble(rS[0][6].as<string>());
    double executedPrice = sellerLimitPrice;

    if (buyerOpenShares <= sellerOpenShares) {
        int executedShares = buyerOpenShares;
        //update balance
        double cost = executedPrice * executedShares;
        double refund = buyerLimitPrice * executedShares - cost;
        addBalance(buyerAccountID, refund);
        addBalance(sellerAccountID, cost);

        //update open shares
        buyerOpenShares = 0;
        sellerOpenShares -= executedShares;
        updateOpenShares(buyOrderID, buyerOpenShares);
        updateOpenShares(sellOrderID, sellerOpenShares);

        //create executed record
        insertExecuted(buyOrderID, executedShares, executedPrice);
        insertExecuted(sellOrderID, executedShares, executedPrice);
        return true;
    } else {
        int executedShares = sellerOpenShares;
        //update balance
        double cost = executedPrice * executedShares;
        double refund = buyerLimitPrice * executedShares - cost;
        addBalance(buyerAccountID, refund);
        addBalance(sellerAccountID, cost);

        //update open shares
        buyerOpenShares -= executedShares;
        sellerOpenShares = 0;
        updateOpenShares(buyOrderID, buyerOpenShares);
        updateOpenShares(sellOrderID, sellerOpenShares);

        //create executed record
        insertExecuted(buyOrderID, executedShares, executedPrice);
        insertExecuted(sellOrderID, executedShares, executedPrice);
        return false;
    }
}

bool MyData::execSellToBuyOrder(string sellOrderID, string buyOrderID) {
    pqxx::result rS = getOrder(sellOrderID);
    pqxx::result rB = getOrder(buyOrderID);

    string sellerAccountID = rS[0][1].as<string>();
    int sellerOpenShares = rS[0][5].as<int>();
    int buyerOpenShares = rB[0][5].as<int>();
    double buyerLimitPrice = moneyToDouble(rB[0][6].as<string>());
    double executedPrice = buyerLimitPrice;

    if (sellerOpenShares <= buyerOpenShares) {
        int executedShares = sellerOpenShares;
        //update balance
        double cost = executedPrice * executedShares;

        addBalance(sellerAccountID, cost);

        //update open shares
        sellerOpenShares = 0;
        buyerOpenShares -= executedShares;
        updateOpenShares(sellOrderID, sellerOpenShares);
        updateOpenShares(buyOrderID, buyerOpenShares);

        //create executed record
        insertExecuted(sellOrderID, executedShares, executedPrice);
        insertExecuted(buyOrderID, executedShares, executedPrice);

        return true;

    } else {
        int executedShares = buyerOpenShares;
        //update balance
        double cost = executedPrice * executedShares;
        addBalance(sellerAccountID, cost);

        //update open shares
        sellerOpenShares -= executedShares;
        buyerOpenShares = 0;
        updateOpenShares(sellOrderID, sellerOpenShares);
        updateOpenShares(buyOrderID, buyerOpenShares);

        //create executed record
        insertExecuted(sellOrderID, executedShares, executedPrice);
        insertExecuted(buyOrderID, executedShares, executedPrice);

        return false;
    }

}

pqxx::result MyData::getSellOrder(string orderID, string accountID, string symbol, double limitPrice) {
    nontransaction N(*C);
    string sql = "SELECT * FROM ORDERS WHERE "
                 "ISCANCELED = FALSE AND "
                 "ID < " + orderID + " AND " +
                 "TRANSACTION_TYPE = 's' AND " +
                 "SYMBOL = " + addApostrophe(symbol) + " AND " +
                 "OPEN_SHARES > 0 AND " +
                 "LIMIT_PRICE <= CAST(" + to_string(limitPrice) + " AS MONEY) " + " AND " +
                 "ACCOUNT_ID != " + addApostrophe(accountID) +
                 "ORDER BY LIMIT_PRICE ASC;";
    return N.exec(sql);
}

pqxx::result MyData::getBuyOrder(string orderID, string accountID, string symbol, double limitPrice) {
    nontransaction N(*C);
    string sql = "SELECT * FROM ORDERS WHERE "
                 "ISCANCELED = FALSE AND "
                 "ID < " + orderID + " AND " +
                 "TRANSACTION_TYPE = 'b' AND " +
                 "SYMBOL = " + addApostrophe(symbol) + " AND " +
                 "OPEN_SHARES > 0 AND " +
                 "LIMIT_PRICE >= CAST(" + to_string(limitPrice) + " AS MONEY) " + " AND " +
                 "ACCOUNT_ID != " + addApostrophe(accountID) +
                 "ORDER BY LIMIT_PRICE DESC;";
    return N.exec(sql);
}

Order MyData::query(string accountId, string orderId) {
    std::lock_guard <std::mutex> lck(mtx);

    Order order1;
    string sql = "";
    //get the order
    pqxx::result r = getOrder(orderId);

//    nontransaction N(*C);

    if (r.empty()) {
        order1.msg = "Order to query does not exist!";
    } else {
        bool isCanceled = r[0]["ISCANCELED"].as<bool>();
        if (isCanceled) {
            r = getCanceledOrder(orderId);
            order1.canceledShares.first = r[0][1].as<string>();
            order1.canceledShares.second = r[0][2].as<string>();
        } else {
            string openShares = r[0][5].as<string>();
            order1.openShares = openShares == "0" ? "" : openShares;
        }
        r = getExecutedOrder(orderId);
        vector <string> record(3, "");
        for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
            record[0] = c[3].as<string>();
            record[1] = c[2].as<string>();
            record[2] = c[1].as<string>();
            order1.executedShares.insert(record);
        }
    }
    return order1;
}

int MyData::getAmountOfSymbol(string accountID, string symbol) {
    nontransaction N(*C);
    string sql = "SELECT AMOUNT FROM POSITION WHERE "
                 "ACCOUNT_ID = " + addApostrophe(accountID) + " AND " +
                 "SYMBOL = " + addApostrophe(symbol) + ";";
    pqxx::result r(N.exec(sql));
    return r[0][0].as<int>();
}

void MyData::addAmountOfSymbol(string accountID, string symbol, int a) {
    int amount = getAmountOfSymbol(accountID, symbol);
    amount += a;
    work W(*C);
    string sql = "UPDATE POSITION SET AMOUNT = " + to_string(amount) +
                 " WHERE ACCOUNT_ID = " + addApostrophe(accountID) + " AND " +
                 "SYMBOL = " + addApostrophe(symbol) + ";";
    W.exec(sql);
    W.commit();
}

Order MyData::cancel(string accountId, string orderId) {
    std::lock_guard <std::mutex> lck(mtx);
    Order order1;
    string sql = "";
    //get the order
    pqxx::result r = getOrder(orderId);
    if (r.empty()) {
        order1.msg = "Order to cancel does not exist!";
        return order1;
    } else {
        bool isCanceled = r[0][7].as<bool>();
        int openShares = r[0][5].as<int>();
        double limitPrice = moneyToDouble(r[0][0].as<string>());
        string transactionType = r[0][2].as<string>();
        string orderAccountId = r[0][1].as<string>();
        string symbol = r[0][3].as<string>();
        if (orderAccountId != accountId) {
            order1.msg = "Cannot cancel order from another account!";
            return order1;
        } else if (isCanceled) {
            order1.msg = "Order already canceled!";
            return order1;
        } else if (openShares == 0) {
            order1.msg = "Order already executed fully!";
            return order1;
        } else {
            work W(*C);
            //opened ---> canceled
            string sqlOrders = "UPDATE ORDERS SET ISCANCELED = TRUE WHERE "
                               "ID = " + orderId + ";";
            string currentTime = to_string(MyTimer::getElapsedMilliseconds());
            string sqlCanceled = "INSERT INTO CANCELED (ORDER_ID, SHARES, TIME) "
                                 "VALUES(" +
                                 orderId + "," +
                                 to_string(openShares) + "," +
                                 currentTime +
                                 ");";
            W.exec(sqlOrders);
            W.exec(sqlCanceled);
            W.commit();
            order1.canceledShares.first = to_string(openShares);
            order1.canceledShares.second = currentTime;

            //refund if it is buy order or return position if it is sell orders
            if (transactionType == "b") {
                double refund = openShares * limitPrice;
                addBalance(orderAccountId, refund);
            } else {
                addAmountOfSymbol(orderAccountId, symbol, openShares);
            }
        }
        r = getExecutedOrder(orderId);
        vector <string> record(3, "");
        for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
            record[0] = c[3].as<string>();
            record[1] = c[2].as<string>();
            record[2] = c[1].as<string>();
            order1.executedShares.insert(record);
        }
    }
    return order1;
}

//helper function
string MyData::accountExistError(string accountId) {
    nontransaction N(*C);
    string checkAccountSql = "SELECT * FROM ACCOUNT WHERE ID = " + addApostrophe(accountId) + ";";
    pqxx::result r(N.exec(checkAccountSql));
    return !r.empty() ? "Account already exist" : "";
}

string MyData::accountNonExistError(string accountId) {
    nontransaction N(*C);
    string checkAccountSql = "SELECT * FROM ACCOUNT WHERE ID = " + addApostrophe(accountId) + ";";
    pqxx::result r(N.exec(checkAccountSql));
    return r.empty() ? "Account does not exist" : "";
}

pqxx::result MyData::getPosition(string accountId, string sym) {
    accountId = addApostrophe(accountId);
    sym = addApostrophe(sym);
    nontransaction N(*C);
    string checkSymSql = "SELECT AMOUNT FROM POSITION WHERE "
                         "ACCOUNT_ID = " + accountId + " AND "
                                                       "SYMBOL = " + sym + ";";
    //pqxx::result r(N.exec(checkSymSql));
    return N.exec(checkSymSql);
}

pqxx::result MyData::getOrder(string orderId) {
    nontransaction N(*C);
    string sql = "SELECT * FROM ORDERS WHERE ID = " + orderId + ";";
    return N.exec(sql);
}

pqxx::result MyData::getCanceledOrder(string orderId) {
    nontransaction N(*C);
    string sql = "SELECT * FROM CANCELED WHERE ORDER_ID = " + orderId + ";";
    return N.exec(sql);
}

pqxx::result MyData::getExecutedOrder(string orderId) {
    nontransaction N(*C);
    string sql = "SELECT * FROM EXECUTED WHERE ORDER_ID = " + orderId + ";";
    return N.exec(sql);
}

string MyData::deductFromAccount(string accountId, int shares, double limit) {
    std::cout << std::setprecision(12);
    accountId = addApostrophe(accountId);
    work W(*C);
    string sql = "SELECT BALANCE FROM ACCOUNT WHERE "
                 "ID = " + accountId + ";";
    pqxx::result r(W.exec(sql));
    double rest = moneyToDouble(r[0][0].as<string>())-shares * limit;
    if (rest < 0) {
        return "Order is rejected due to insufficient funds";
    } else {
        sql = "UPDATE ACCOUNT SET BALANCE = " + to_string(rest) + " WHERE "
                                                                  "ID = " + accountId + ";";
        W.exec(sql);
        W.commit();
        return "";
    }
    return "";
}

string MyData::deductFromPosition(string accountId, string sym, int shares) {
    accountId = addApostrophe(accountId);
    sym = addApostrophe(sym);
    work W(*C);
    string sql = "SELECT AMOUNT FROM POSITION WHERE "
                 "ACCOUNT_ID = " + accountId + " AND " +
                 "SYMBOL = " + sym + ";";
    pqxx::result r(W.exec(sql));
    if (r.empty()) {
        return "Account does not have this symbol";
    } else {
        int rest = r[0][0].as<int>() - shares;
        if (rest < 0) {
            return "Order is rejected due to insufficient shares";
        } else {
            sql = "UPDATE POSITION SET AMOUNT = " + to_string(rest) + " WHERE "
                                                                      "ACCOUNT_ID = " + accountId + " AND " +
                  "SYMBOL = " + sym + ";";
            W.exec(sql);
            W.commit();
            return "";
        }
    }
}

string MyData::getMaxOrderID() {
    nontransaction N(*C);
    string sql = "SELECT MAX(ID) FROM ORDERS;";
    pqxx::result r(N.exec(sql));
    return r[0][0].is_null() ? "0" : r[0][0].as<string>();
}

void MyData::setUniqueID(string targetID) {
    uniqueID.setUniqueID(targetID);
}