#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "MemoryManager.h"
#include <iostream>
#include <chrono>
#include <thread>

// Initialize the static instance pointer
GlobalScheduler* GlobalScheduler::instance = nullptr;

GlobalScheduler::GlobalScheduler() {
    // Default: use FCFS scheduler
    this->scheduler = std::make_shared<FCFSScheduler>();
}

GlobalScheduler::~GlobalScheduler() {
    schedulerStop();  // Cleanup running scheduler if any
}

void GlobalScheduler::initialize() {
    if (instance == nullptr) {
        instance = new GlobalScheduler();
    }
}

void GlobalScheduler::destroy() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

GlobalScheduler* GlobalScheduler::getInstance() {
    return instance;
}

// Select scheduling algorithm by name
void GlobalScheduler::selectScheduler(const std::string& algoName) {
    if (algoName == "fcfs") {
        this->scheduler = std::make_shared<FCFSScheduler>(coreCount);
    }
    else if (algoName == "rr") {
        this->scheduler = std::make_shared<RRScheduler>(coreCount, ConfigReader::getInstance()->getQuantum());
    }
}

int GlobalScheduler::getRandomMemSize(int minMem, int maxMem)
{
    int minExp = static_cast<int>(log2(minMem));
    int maxExp = static_cast<int>(log2(maxMem));

    int randExp = minExp + rand() % (maxExp - minExp + 1);

    return 1 << randExp;
}

// Generate dummy processes and assign them to the scheduler
void GlobalScheduler::schedulerStart() {
    scheduler_start = true;
    int lastSpawnTick = 0;
    int batchFreq = ConfigReader::getInstance()->getBatchProcessFreq();

    schedulerThread = std::thread([this, batchFreq, lastSpawnTick]() mutable {
        while (scheduler_start) {
            int currentTick = CPUTick::getInstance()->getTicks();

            if ((currentTick - lastSpawnTick) >= batchFreq) {
                int i = getProcessCount();
                i++;

				// Create a new process with a unique ID, name, and random memory size

                std::string name = "process_" + std::to_string(i); // Generate name based of process number

				int minMem = ConfigReader::getInstance()->getMinMemPerProc();
				int maxMem = ConfigReader::getInstance()->getMaxMemPerProc();

				int memSize = getRandomMemSize(minMem, maxMem); // Generate random memory size for the process
				int memPerFrame = ConfigReader::getInstance()->getMemPerFrame();

				int numPages = (memSize + memPerFrame - 1) / memPerFrame; // Calculate number of pages based on memory size and frame size

                auto process = std::make_shared<Process>(i, name, memSize, numPages);

                // Attempt to allocate memory for the process
                if (MemoryManager::getInstance()->loadPagesForProcess(process)) {
                    ConsoleManager::getInstance()->createBaseScreen(process, false);
                    processList.push_back(process);
                    this->scheduler->addProcess(process, -1);
                }
                else {
                    ConsoleManager::getInstance()->createBaseScreen(process, false);
                    processList.push_back(process);
                    this->scheduler->addProcess(process, -1);
                }

                lastSpawnTick = currentTick; // Update last spawn tick
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Keep thread responsive
        }
        });
}

void GlobalScheduler::schedulerStop() {
    scheduler_start = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

// Add a new process
void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
    processList.push_back(process);
    this->scheduler->addProcess(process, -1);
}

std::shared_ptr<Process> GlobalScheduler::getProcessByPID(int pid)
{
    for (auto& proc : this->processList) {
        if (proc->getPID() == pid) return proc;
    }
    return nullptr;
}

// Get total process count
int GlobalScheduler::getProcessCount() const {
    return static_cast<int>(processList.size());
}

// Get specific process
std::shared_ptr<Process>& GlobalScheduler::getProcess(int index) {
    return processList.at(index);
}

// Get most recently added process
std::shared_ptr<Process> GlobalScheduler::getMostRecentProcess() {
    if (!processList.empty()) {
        return processList.back();
    }
    return nullptr;
}

// Accessor for the current scheduler
std::shared_ptr<AScheduler> GlobalScheduler::getScheduler() {
    return scheduler;
}

// Core configuration
void GlobalScheduler::setCoreCount(int cores) {
    this->coreCount = cores;
}

int GlobalScheduler::getCoreCount() const {
    return coreCount;
}