//
// Created by Xiaodong Liu on 3/24/22.
//

#ifndef ERSS_HWK4_REQUEST_H
#define ERSS_HWK4_REQUEST_H

//Should be an abstract class
class Request{
private:
    vector<char> xmlRequest;
public:
    Request(vector<char>& _xmlRequest):xmlRequest(_xmlRequest){}
};
#endif //ERSS_HWK4_REQUEST_H
