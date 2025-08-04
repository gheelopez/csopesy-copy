#pragma once

#include <queue>
#include <mutex>
#include "Process.h"  // Adjust the path if needed

class GlobalProcessQueue {
private:
    std::queue<std::shared_ptr<Process>> processQueue;
    std::mutex queueMutex;

    GlobalProcessQueue();  // Private constructor

public:
    // Delete copy/move operations
    GlobalProcessQueue(const GlobalProcessQueue&) = delete;
    GlobalProcessQueue& operator=(const GlobalProcessQueue&) = delete;

    static GlobalProcessQueue& getInstance();

    void push(std::shared_ptr<Process> process);
    std::shared_ptr<Process> pop();
    bool empty();
};
