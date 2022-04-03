//
// Created by Xiaodong Liu on 3/28/22.
//

#include "UniqueID.h"

UniqueID::UniqueID() : unique_id("0") {}

string UniqueID::updateID() {
    int endIdx = unique_id.length() - 1;
    for (int i = endIdx; i >= 0; --i) {
        if (unique_id[i] < '9') {
            ++unique_id[i];
            return unique_id;
        } else {
            unique_id[i] = '0';
        }
    }
    if (unique_id[0] == '0') {
        unique_id = "1" + unique_id;
    }
    return unique_id;
}

string UniqueID::getUniqueID() {
    return unique_id;
}

void UniqueID::setUniqueID(string targetID) {
    this->unique_id = targetID;
}