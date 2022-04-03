//
// Created by Xiaodong Liu on 3/27/22.
//

#ifndef ERSS_HWK4_ORDER_H
#define ERSS_HWK4_ORDER_H
#include <vector>
#include <set>
#include <string>
#include <cstring>
#include <utility>
using std::vector;
using std::set;
using std::string;
using std::pair;
using std::make_pair;

class Order{
public:
    string msg;
    string openShares; //open_shares
    pair<string, string> canceledShares; // shares time
    set<vector<string>> executedShares; //time prices shares
    Order():msg(""), openShares(""), canceledShares(make_pair("", "")){}
};
#endif //ERSS_HWK4_ORDER_H
