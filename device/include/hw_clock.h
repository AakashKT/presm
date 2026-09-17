#ifndef HW_CLOCK_H
#define HW_CLOCK_H

#include "common.h"
#include "logging.h"

class HwClock {
public:
    HwClock(std::string name);
    ~HwClock();

    void wait_for_tick();

private:
    std::condition_variable cv;
    std::mutex mtx;

    std::thread clk_thread;

    Logger* log;

    uint64_t cycle_count = 0;
};

#endif