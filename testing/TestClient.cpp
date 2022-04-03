#include "TestClient.hpp"
#include "util.hpp"
#include <cstring>

using boost::asio::ip::tcp;

std::vector<char> filter_non_ascii(const std::vector<char>& target) {
    std::vector<char> res;
    for (std::size_t i = 0; i < target.size(); ++i) {
        if (target[i] <= 127) {
            res.push_back(target[i]);
        }
    }

    return res;
}

/**
 * Create an instance of TestClient 
 */
TestClient::TestClient() : socket_fd(-1), ctx(), sock(ctx), connected(false) {}

bool TestClient::connectToServer(const char* hostname, const char* port) {
    try {
        tcp::resolver resolver(this->ctx);
        tcp::resolver::results_type endpoints = resolver.resolve(hostname, port);
        boost::asio::connect(this->sock, endpoints);
        this->connected = true;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
} 

/**
 * @brief tries to send a string to remote server
 * 
 * @param payload xml string
 * @return int status code: 1 for success, 0 otherwise
 */
bool TestClient::sendMsg(const std::string& payload) {
    if (this->connected == false) {
        return false;
    }
    std::stringstream ss;
    ss << payload.size() << "\n" << payload;
    std::string message = ss.str();

    boost::system::error_code ignored_error;

    try {
        boost::asio::write(this->sock, boost::asio::buffer(message), ignored_error);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

std::vector<char> TestClient::recvMsg() {
    std::vector<char> res;
    // res.reserve(res.max_size());
    boost::system::error_code error;

    std::size_t rcvd;
    ssize_t should_recv = -1;
    ssize_t total_rcvd = 0;
    do {
        // buf.reserve(256);
        boost::array<char, 256> buffer_arr = {'\0'};
        rcvd = this->sock.read_some(boost::asio::buffer(buffer_arr, 256), error);
        if (rcvd == 0) {
            std::cout << "rcvd 0" << std::endl;
            break;
        }
        
        
        std::string buffer_str(buffer_arr.begin(), buffer_arr.end());
        std::vector<char> buf(buffer_arr.begin(), buffer_arr.end());


        // concat buf to result vector
        res.insert(res.end(), buf.begin(), buf.end());
        std::string temp(res.begin(), res.end());

        if (error == boost::asio::error::eof) {
            break;
        }
        // std::cout << "error code: " << error << std::endl;
        // TODO: check for other types of errors

        std::vector<char>::iterator it = std::find(res.begin(), res.end(), '\n');
        if (it != res.cend()) {
            std::size_t newline_idx = std::distance(res.begin(), it);
            std::vector<char> content_length_buf(res.begin(), it);
            std::string content_length_str(content_length_buf.begin(), content_length_buf.end());

            try {
                int content_length = std::stoi(content_length_str);
                should_recv = content_length - (res.size() - newline_idx + 1);
                total_rcvd = res.size() - newline_idx - 1;
            } catch (std::exception& e) {
                std::cerr << "failed to convert content length str (" << content_length_str << "): " << e.what() << std::endl;
                return std::vector<char>();
            }
            break;
        }

    } while (should_recv == -1);

    std::vector<char> leftover;
    while (should_recv > 0) {
        boost::array<char, 256> leftover_buffer_arr = {'\0'};

        std::size_t temp_rcvd = this->sock.read_some(boost::asio::buffer(leftover_buffer_arr), error);
        std::string temp_rcvd_str(leftover_buffer_arr.begin(), leftover_buffer_arr.end());
        std::vector<char> temp_rcvd_vec(temp_rcvd_str.begin(), temp_rcvd_str.end());
        res.insert(res.end(), temp_rcvd_vec.begin(), temp_rcvd_vec.end());
        total_rcvd += temp_rcvd;
        should_recv -= temp_rcvd;
    }


    res = filter_non_ascii(res);
    return res;
}

std::vector<char> TestClient::recvSomeMsg() {
    boost::system::error_code error;

    boost::array<char, 65535> buffer = {'\0'};
    this->sock.read_some(boost::asio::buffer(buffer), error);
    std::string rcvd_str(buffer.data());
    std::vector<char> buffer_vec(rcvd_str.begin(), rcvd_str.end());
    return buffer_vec;
}

void TestClient::sendAndRecv(const std::string& payload) {
    this->sendMsg(payload);
    this->recvSomeMsg();
}


void TestClient::close() {
    this->sock.close();
}
