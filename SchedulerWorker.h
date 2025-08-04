#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <chrono>
#include "IETThread.h"
#include "Process.h"
#include "CPUTick.h"
#include "ConfigReader.h"

class SchedulerWorker : public IETThread {
public:
    explicit SchedulerWorker(int coreId = 0);
    ~SchedulerWorker();

    void start();                       // Launch thread
    void stop();                        // Graceful stop
    void run() override;               // Execution loop

    void assignProcess(std::shared_ptr<Process> process); // Assign process
    bool isBusy() const;               // Is worker executing?
    std::shared_ptr<Process> getProcess() const; // Get current process

private:
    int coreId;
    std::thread workerThread;
    std::atomic<bool> running;
    std::atomic<bool> busy;

    std::shared_ptr<Process> currentProcess;

    mutable std::mutex mtx;
    std::condition_variable cv;
};
