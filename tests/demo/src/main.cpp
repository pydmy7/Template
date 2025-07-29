#include <thread>

#include "scl/timer.hpp"

int main() {

    scl::Timer timer("test");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.timeStamp("part 1");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.timeStamp("part 2");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    timer.timeStamp("part 3");
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    timer.timeStamp("part 4");

    return 0;
}
