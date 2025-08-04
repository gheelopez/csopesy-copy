#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include "Process.h"
#include "AScheduler.h"
#include "SchedulerWorker.h"

// First-Come, First-Served Scheduler
class FCFSScheduler : public AScheduler {
public:
    using CPUWorkers = std::vector<std::shared_ptr<SchedulerWorker>>;

    FCFSScheduler();
    explicit FCFSScheduler(int cores);
    ~FCFSScheduler() override = default;

    // Lifecycle
    void init() override;
    void run() override;
    void stop() override;
    void execute() override;

    // Process management
    void addProcess(std::shared_ptr<Process> process, int core) override;
    void assignCore(std::shared_ptr<Process> process, int core) override;


    // Utility
    std::shared_ptr<Process> getProcess(int core) const;
    const std::string& getProcessFromQueue(int index) const override;
	int getNumCores() override { return numCores; }

    void printCores() override;
    void printProcessQueues() override;
    void printRunningProcesses() override;
    void printFinishedProcesses() override;
    String screenLS() override;

    // Scheduler state
    bool allProcessesFinished();

private:
    std::string name = "FCFS";
    int numCores = 1;

    std::thread schedulerThread;
    std::mutex schedulerMutex;

    std::atomic<bool> schedulerRun = false;

    CPUWorkers cpuWorkers;
    std::queue<std::shared_ptr<Process>> globalQueue;
    std::vector<std::shared_ptr<Process>> processList; // stores the processes being taken from GlobalScheduler


};
