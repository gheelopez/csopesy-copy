#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include "AScheduler.h"
#include "RRScheduler.h"
#include "Process.h"
#include "ConsoleManager.h"
#include "MemoryManager.h" 

// Singleton class to manage the selected scheduler and process list across the application.
class GlobalScheduler {
public:
    // Singleton access
    static GlobalScheduler* getInstance();
    static void initialize();
    static void destroy();

    // Scheduler control
    void selectScheduler(const std::string& algoName);
    std::shared_ptr<AScheduler> getScheduler();

    // Process management
    void addProcess(std::shared_ptr<Process> process);
    int getProcessCount() const;
    std::shared_ptr<Process>& getProcess(int index);
    std::shared_ptr<Process> getMostRecentProcess();
    void schedulerStart(); // this is where dummy processes are generated
    void schedulerStop();
    std::shared_ptr<Process> getProcessByPID(int pid);

    // Core configuration
    void setCoreCount(int cores);
    int getCoreCount() const;

private:
    // Constructor and destructor
    GlobalScheduler();
    ~GlobalScheduler();

    // Disallow copying
    GlobalScheduler(const GlobalScheduler&) = delete;
    GlobalScheduler& operator=(const GlobalScheduler&) = delete;

    // Singleton instance
    static GlobalScheduler* instance;

	// Helper function to generate random memory size
    int getRandomMemSize(int minMem, int maxMem);

    // Current scheduler
    std::shared_ptr<AScheduler> scheduler;

    // List of all submitted processes
    std::vector<std::shared_ptr<Process>> processList;

    bool scheduler_start = false; // boolean for creating dummy processes
    std::thread schedulerThread;

    // Core configuration
    int coreCount = 1;

    std::unique_ptr<MemoryManager> memoryManager;
};
