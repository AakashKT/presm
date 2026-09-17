#include "hw_clock.h"

HwClock::HwClock(std::string name)
{
    this->log = new Logger();
    this->log->init("hw_clock_" + name);

    this->log->log_info("[HwClock] Clock started.");

    this->clk_thread = std::thread(
        [](HwClock* clk) {
            while(true) {
                clk->mtx.lock();
                clk->cv.notify_all();
                clk->cycle_count++;
                clk->mtx.unlock();
            }
        },
        this
    );
}

HwClock::~HwClock()
{
    this->log->log_info("[HwClock] Total cycles: " + std::to_string(this->cycle_count));
    this->clk_thread.detach();
}

void HwClock::wait_for_tick()
{
    std::unique_lock<std::mutex> lock(this->mtx);
    this->cv.wait(lock);
}