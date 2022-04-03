#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <sstream>
#include <assert.h>

std::string wrap_message(const std::string& msg);
std::vector<char> strip_content_length(const std::vector<char>& target);

std::string one_liner(const std::string& original, bool is_resp);
std::string read_file(const std::string& filename);
#endif