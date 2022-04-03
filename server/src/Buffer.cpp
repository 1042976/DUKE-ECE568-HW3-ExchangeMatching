//
// Created by Xiaodong Liu on 3/31/22.
//

#include "Buffer.h"


int Buffer::getXMLLength(vector<char> buffer) {
    int idx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    string lenStr = string(buffer.begin(), buffer.begin() + idx);
    return stoi(lenStr);
}

vector<char> Buffer::removeHeader(vector<char> buffer) {
    size_t idx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    while (idx < buffer.size() && (buffer[idx] == '\r' || buffer[idx] == '\n')) {
        ++idx;
    }
    return vector<char>(buffer.begin() + idx, buffer.end());
}

vector<char> Buffer::addHeader(vector<char> buffer) {
    string header = to_string(buffer.size()) + "\n";
    vector<char> res;
    res.reserve(header.length() + buffer.size());
    res.insert(res.end(), header.begin(), header.end());
    res.insert(res.end(), buffer.begin(), buffer.end());
    return res;
}

pair<int, vector<char>> Buffer::dissemble(vector<char> buffer) {
    size_t idx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    int len = stoi(string(buffer.begin(), buffer.begin() + idx));
    while (idx < buffer.size() && (buffer[idx] == '\r' || buffer[idx] == '\n')) {
        ++idx;
    }
    return make_pair(len, vector<char>(buffer.begin() + idx, buffer.end()));
}

int Buffer::getWholeBufferLength(vector<char> buffer) {
    size_t idx = findAnyCharInSubStr(buffer, strToVector("\r\n"));
    int contentLen = stoi(string(buffer.begin(), buffer.begin() + idx));
    while (idx < buffer.size() && (buffer[idx] == '\r' || buffer[idx] == '\n')) {
        ++idx;
    }
    return idx + contentLen;
}