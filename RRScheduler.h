#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <string>

#include "Process.h"
#include "AScheduler.h"
#include "RRSchedulerWorker.h"
#include "MemoryManager.h"
// Round Robin Scheduler
class RRScheduler : public AScheduler
{
public:
	using CPUWorkers = std::vector<std::shared_ptr<RRSchedulerWorker>>;

	RRScheduler();
	explicit RRScheduler(int cores, int quantumCycles);
	~RRScheduler() override = default;

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

	void setQuantumCycles(int quantumCycles);

private:
	String name = "RR";
	int numCores = 1;
	int quantumCycles = 1;
	
	std::thread schedulerThread;
	std::mutex schedulerMutex;

	std::atomic<bool> schedulerRun = false;

	CPUWorkers cpuWorkers;
	std::queue<std::shared_ptr<Process>> globalQueue;
	std::vector<std::shared_ptr<Process>> processList;

};

