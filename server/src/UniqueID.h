//
// Created by Xiaodong Liu on 3/28/22.
//

#ifndef ERSS_HWK4_UNIQUEID_H
#define ERSS_HWK4_UNIQUEID_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>

using std::string;

class UniqueID {

private:
    string unique_id;

public:
    UniqueID();

    string updateID();

    string getUniqueID();

    void setUniqueID(string targetID);

};

#endif //ERSS_HWK4_UNIQUEID_H
