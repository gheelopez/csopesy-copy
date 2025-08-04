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
#include "GlobalProcessQueue.h"
#include "MemoryManager.h"

class RRSchedulerWorker : public IETThread 
{
public:
	explicit RRSchedulerWorker(int coreId = 0);
	~RRSchedulerWorker();

	void start();
	void stop();
	void run() override;
	void preempt();

	void assignProcess(std::shared_ptr<Process> process); // Assign process
	bool isBusy() const;               // Is worker executing?
	std::shared_ptr<Process> getProcess() const; // Get current process

	bool wasPreempted() const;

private: 
	int coreId;
	std::thread workerThread;
	std::thread snapshotThread;
	std::atomic<bool> running;
	std::atomic<bool> busy;
	std::atomic<bool> preemptRequested;

	std::shared_ptr<Process> currentProcess;

	mutable std::mutex mtx;
	std::condition_variable cv;
};

