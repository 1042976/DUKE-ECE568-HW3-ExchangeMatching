//
// Created by Xiaodong Liu on 3/29/22.
//

#ifndef ERSS_HWK4_COMMON_H
#define ERSS_HWK4_COMMON_H

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
using std::string;
using std::vector;
using std::cout;
using std::endl;

extern size_t MAXBUFFERLEN;
int findChar(const vector<char> &A, char target);
void setZeroInTheEnd(vector<char> &buffer);
vector<char> strToVector(string str);
string vectorToStr(vector<char> arr);
void standardizeVector(vector<char> &buffer);
void printVector(vector<char> arr);
string readFile(const char* filename);
string addLenAsHeader(string content);
int findAnyCharInSubStr(const vector<char> &A, vector<char> target);
#endif //ERSS_HWK4_COMMON_H
