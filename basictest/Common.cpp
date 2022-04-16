//
// Created by Xiaodong Liu on 3/29/22.
//
#include "Common.h"

size_t FIRSTBUFFERLEN = 1024;

int findChar(const vector<char> &A, char target) {
    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i] == target) {
            return i;
        }
    }
    return -1;
}

int findAnyCharInSubStr(const vector<char> &A, vector<char> target) {
    int min_index = INT_MAX;
    for (size_t i = 0; i < target.size(); ++i) {
        int cur = findChar(A, target[i]);
        if (cur == -1) {
            continue;
        }
        min_index = std::min(min_index, cur);
    }
    return min_index == INT_MAX ? -1 : min_index;
}

void setZeroInTheEnd(vector<char> &buffer) {
    int firstZeroIdx = findChar(buffer, '\0');
    if (firstZeroIdx == -1) {
        buffer.emplace_back('\0');
    }
}

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

string readFile(const char *filename) {
    std::ifstream ifs(filename);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

string addLenAsHeader(string content) {
    int contentLen = content.length();
    return std::to_string(contentLen) + "\n" + content;
}