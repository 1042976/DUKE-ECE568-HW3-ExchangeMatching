#include <pthread.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include "util.hpp"
#include "TestClient.hpp"

std::size_t START_CASE = 1;
std::size_t END_CASE = 20;

typedef struct {
    std::shared_ptr<std::vector<std::string> > xmls_ptr;
    int epoch;
    const char* hostname;
    const char* port;
} args_t;

void* task(void* thread_args) {
    args_t* args = (args_t*) thread_args;
    std::shared_ptr<std::vector<std::string> > xmls = args->xmls_ptr;
    int epoch = args->epoch;
    const char* hostname = args->hostname;
    const char* port = args->port;

    TestClient tc;
    tc.connectToServer(hostname, port);

    for (int i = 0; i < epoch; ++i) {
        for (std::size_t j = 0; j < xmls->size(); ++j) {

            tc.sendMsg(xmls->at(j));

            tc.recvMsg();
        }
    }

    tc.close();
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: ./scalability_test.out <#thread> <#epoch> <hostname> <port>" << std::endl;
        return EXIT_FAILURE;
    }

    int num_threads = std::stoi(argv[1]);
    int epoch = std::stoi(argv[2]);

    const char* hostname = argv[3];
    const char* port = argv[4];

    std::shared_ptr<std::vector<std::string> > xmls_ptr = std::make_shared<std::vector<std::string> >();

    for (std::size_t i = START_CASE; i < END_CASE; ++i) {
        std::string filename = "xmls/test" + std::to_string(i) + ".xml";
        xmls_ptr->push_back(read_file(filename));
    }

    args_t thread_args = {xmls_ptr, epoch, hostname, port};

    std::vector<pthread_t> threads(num_threads, 0);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        int rc = pthread_create(&threads[i], nullptr, task, (void*)&thread_args);
        if (rc != 0) {
            std::cerr << "failed to create thread!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> fs = end_time - start_time;
    std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
    std::cout << "Seconds: " << fs.count() << "s" << std::endl;
    std::cout << "Or in ms: " << d.count() << "ms" << std::endl;

    return EXIT_SUCCESS;
};