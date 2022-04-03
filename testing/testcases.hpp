#ifndef __TESTCASES_HPP__
#define __TESTCASES_HPP__

#include "util.hpp"
#include "TestClient.hpp"

void testcase1(TestClient& tc);
void testcase2(TestClient& tc);
// void testcase3(TestClient& tc);

std::string get_resp(const std::string& filename, TestClient& tc);

void run_testcase(int test_id, TestClient& tc);

#endif
