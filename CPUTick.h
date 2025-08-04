#pragma once

#include <thread>
#include <atomic>
#include <chrono>

class CPUTick {
public:
    static CPUTick* getInstance();
    static void destroy();  // Clean up memory

    void resetTicks();
    void addTick(int ticks);
    int getTicks() const;

    void startAutoTick(int intervalMs = 100);  // Start ticking in background
    void stopAutoTick();                       // Stop background ticking

    // Delete copy and assignment
    CPUTick(const CPUTick&) = delete;
    CPUTick& operator=(const CPUTick&) = delete;

private:
    CPUTick();
    ~CPUTick();

    static CPUTick* sharedInstance;
    int nTicks;

    std::thread tickThread;
    std::atomic<bool> ticking;
};
