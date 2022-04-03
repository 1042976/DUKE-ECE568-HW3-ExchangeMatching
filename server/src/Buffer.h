//
// Created by Xiaodong Liu on 3/31/22.
//

#ifndef ERSS_HWK4_BUFFER_H
#define ERSS_HWK4_BUFFER_H
#include <string>
#include <utility>
#include "MyData.h"

class Buffer{
public:
    static vector<char> removeHeader(vector<char> buffer);
    static vector<char> addHeader(vector<char> buffer);
    static int getXMLLength(vector<char> buffer);
    static int getWholeBufferLength(vector<char> buffer);
    static pair<int, vector<char>> dissemble(vector<char> buffer);
};

#endif //ERSS_HWK4_BUFFER_H
