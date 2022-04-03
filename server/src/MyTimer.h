//
// Created by Xiaodong Liu on 3/27/22.
//

#ifndef ERSS_HWK4_MYTIMER_H
#define ERSS_HWK4_MYTIMER_H

#include <chrono>

namespace {
    class MyTimer {
    public:
        MyTimer();

        static long long getElapsedMilliseconds() {
            auto current = std::chrono::steady_clock::now();
            // std::chrono::duration<double> elapsed = current - MyTimer::start;
            auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                    current - MyTimer::start).count();
            return elapsedMilliseconds;
        }

        static std::chrono::time_point <std::chrono::steady_clock> start;
    };

    std::chrono::time_point <std::chrono::steady_clock> MyTimer::start = std::chrono::steady_clock::now();
}
#endif //ERSS_HWK4_MYTIMER_H
