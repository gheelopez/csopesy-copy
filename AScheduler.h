#pragma once

#include "IETThread.h"
#include "Process.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class AScheduler : public IETThread {
public:
	enum SchedulingAlgorithm {
		FCFS,
		ROUND_ROBIN
	};

	AScheduler();
	AScheduler(SchedulingAlgorithm schedulingAlgo);
	virtual ~AScheduler() = default;

	// Core functions
	virtual void init() = 0;
	virtual void run() override = 0;
	virtual void stop() = 0;
	virtual void execute() = 0;

	// Process management
	virtual void addProcess(std::shared_ptr<Process> process, int core) = 0;
	virtual void assignCore(std::shared_ptr<Process> process, int core) = 0;
	std::shared_ptr<Process> findProcess(const std::string& processName);

	// Utilities
	virtual const std::string& getProcessFromQueue(int index) const = 0;
	virtual void printCores() = 0;
	virtual void printProcessQueues() = 0;
	virtual void printRunningProcesses() = 0;
	virtual void printFinishedProcesses() = 0;
	virtual String screenLS() = 0;
	virtual int getNumCores() = 0;

protected:
	SchedulingAlgorithm schedulingAlgo;
	std::vector<std::shared_ptr<Process>> processes;
	bool running = true;
};
