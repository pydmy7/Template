#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <format>

namespace scl {

class Timer final {
public:
    explicit Timer(const std::string& title) {
        title_ = title;

        auto current_time = std::chrono::steady_clock::now();
        start_time_ = current_time;
        last_time_ = current_time;
    }

    ~Timer() {
        auto current_time = std::chrono::steady_clock::now();
        auto start_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time_).count();
        std::cout << std::format("[Timer: {}, Mark: total] Duration from start timestamp: {} ms.", title_, start_duration) << std::endl;
    }

    void timeStamp(const std::string& mark) {
        auto current_time = std::chrono::steady_clock::now();
        auto last_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time_).count();
        auto start_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time_).count();
        if (last_duration != start_duration) {
            std::cout << std::format("[Timer: {}, Mark: {}] Duration from last timestamp: {} ms.", title_, mark, last_duration) << std::endl;
        }
        std::cout << std::format("[Timer: {}, Mark: {}] Duration from start timestamp: {} ms.", title_, mark, start_duration) << std::endl;

        last_time_ = current_time;
    }

private:
    std::string title_;
    std::chrono::steady_clock::time_point start_time_, last_time_;
};

}  // namespace scl
