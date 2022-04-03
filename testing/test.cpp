//
// Created by Xiaodong Liu on 3/24/22.
//


#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "TestClient.hpp"
#include "util.hpp"
#include "testcases.hpp"

int main(int argc, char* argv[]){
    if (argc != 4) {
        std::cerr << "Usage: ./test <hostname> <port> <#testcases>" << std::endl;
        return EXIT_FAILURE;
    }

    TestClient tc;
    int num_test_cases = std::stoi(argv[3]);
    bool status = tc.connectToServer(argv[1], argv[2]);
    if (!status) {
        std::cerr << "failed to connect to " << argv[1] << " at port " << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    for (std::size_t i = 1; i <= num_test_cases; ++i) {
        run_testcase(i, tc);
    }

    // testcase1(tc);
    // testcase2(tc);
    // testcase3(tc);
    tc.close();
    return EXIT_SUCCESS;
}
