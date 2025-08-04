#include "RRScheduler.h"

RRScheduler::RRScheduler() {}

RRScheduler::RRScheduler(int cores, int quantumCycles) : numCores(cores), quantumCycles(quantumCycles), schedulerRun(true) {
    init();
}

void RRScheduler::init() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    cpuWorkers.clear();
    while (!GlobalProcessQueue::getInstance().empty()) {
        GlobalProcessQueue::getInstance().pop();
    }

    for (int i = 0; i < numCores; i++) {
        cpuWorkers.push_back(std::make_shared<RRSchedulerWorker>(i));
    }
}

void RRScheduler::run() {
    schedulerRun = true;
    schedulerThread = std::thread([this]() {
        while (schedulerRun) {
            execute();
        }
        });
}

void RRScheduler::stop() {
    schedulerRun = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void RRScheduler::execute() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    for (int core = 0; core < numCores; ++core) {
        auto& worker = cpuWorkers[core];

        if (!worker->isBusy()) {
            auto process = worker->getProcess();

            if (process && process->getState() == Process::WAITING) {
                process->setState(Process::READY);
            }

            auto nextProcess = GlobalProcessQueue::getInstance().pop();
            if (nextProcess) {
                // Load pages using demand paging
                bool memoryLoaded = MemoryManager::getInstance()->loadPagesForProcess(nextProcess);

                if (memoryLoaded) {
                    nextProcess->setState(Process::RUNNING);
                    worker->assignProcess(nextProcess);
                    worker->start();
                }
                else {
                    // Page loading failed (e.g., backing store failure or FIFO problem)
                    nextProcess->setState(Process::WAITING);
                    GlobalProcessQueue::getInstance().push(nextProcess);
                }
            }
        }
    }
}


void RRScheduler::addProcess(std::shared_ptr<Process> process, int core) {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    GlobalProcessQueue::getInstance().push(process);
    this->processList.push_back(process);
}

void RRScheduler::assignCore(std::shared_ptr<Process> process, int core) {
    addProcess(process, core);
}

std::shared_ptr<Process> RRScheduler::getProcess(int core) const {
    if (core >= 0 && core < numCores && cpuWorkers[core]->isBusy()) {
        return cpuWorkers[core]->getProcess();
    }
    return nullptr;
}

const std::string& RRScheduler::getProcessFromQueue(int index) const
{
    static std::string empty = "";
    if (!globalQueue.empty()) {
        return globalQueue.front()->getName();  // Only returns front of global queue
    }
    return empty;
}

void RRScheduler::printCores() {
    for (int i = 0; i < numCores; ++i)
    {
        std::cout << "Core " << i << ": ";
        if (cpuWorkers[i]->isBusy())
        {
            std::cout << cpuWorkers[i]->getProcess()->getName();
        }
        else {
            std::cout << "[Idle]";
        }
        std::cout << std::endl;
    }
}

void RRScheduler::setQuantumCycles(int quantumCycles)
{
    this->quantumCycles = quantumCycles;
}

// for debugging

void RRScheduler::printProcessQueues()
{
    std::lock_guard<std::mutex> lock(schedulerMutex);
    std::cout << "Global Queue: ";
    std::queue<std::shared_ptr<Process>> tempQueue = globalQueue;
    while (!tempQueue.empty()) {
        std::cout << tempQueue.front()->getName() << " ";
        tempQueue.pop();
    }
    std::cout << std::endl;
}

// for debugging

bool RRScheduler::allProcessesFinished()
{
    std::lock_guard<std::mutex> lock(schedulerMutex);
    if (!globalQueue.empty()) return false;
    for (int i = 0; i < numCores; ++i)
    {
        if (cpuWorkers[i]->isBusy())
        {
            return false;
        }
    }
    return true;
}

// for debugging

void RRScheduler::printRunningProcesses()
{
    for (int i = 0; i < numCores; ++i) {
        auto process = cpuWorkers[i]->getProcess();
        if (cpuWorkers[i]->isBusy() && process->getState() == Process::RUNNING)
        {
            int currentLine = process->getCommandCounter();        // current instruction index
            size_t totalLines = process->getCommandList().size();        // total 

            std::cout << process->getName()
                << "     (" << process->getRunningTimestamp()
                << ")     Core: " << i << "     "
                << currentLine << " / " << totalLines;
        }
        else {
            std::cout << "[Idle]";
        }
        std::cout << std::endl;
    }
}

// for debugging


void RRScheduler::printFinishedProcesses()
{
    std::lock_guard<std::mutex> lock(schedulerMutex);
    std::cout << "Finished Processes: ";
    for (const auto& process : processList) {
        if (process->getState() == Process::FINISHED) {

            int totalLines = process->getCommandCounter();

            std::cout << process->getName()
                << "     (" << process->getFinishedTimestamp()
                << ")     Finished     "
                << totalLines << " / " << totalLines;
        }
    }
    std::cout << std::endl;
}

String RRScheduler::screenLS() {
    std::ostringstream out;

    int usedCores = 0;
    for (int i = 0; i < numCores; ++i) {
        if (cpuWorkers[i]->isBusy()) {
            ++usedCores;
        }
    }

    double utilization = (static_cast<double>(usedCores) / numCores) * 100.0;

    out << "\n";
    out << "CPU Utilization: " << utilization << "%\n";
    out << "Cores Used: " << usedCores << "\n";
    out << "Cores Available: " << (numCores - usedCores) << "\n";
    out << "------------------------------------\n";
    out << "Running Processes:\n";

    for (int i = 0; i < numCores; ++i) {
        auto process = cpuWorkers[i]->getProcess();
        if (cpuWorkers[i]->isBusy() && process->getState() == Process::RUNNING) {
            int currentLine = process->getCommandCounter();
            size_t totalLines = process->getCommandList().size();

            out << process->getName()
                << "     (" << process->getRunningTimestamp()
                << ")     Core: " << i << "     "
                << currentLine << " / " << totalLines;
        }
        else {
            out << "Core " << i << ": [Idle]";
        }
        out << "\n";
    }

    out << "\nFinished Processes:\n";

    {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        for (const auto& process : processList) {
            if (process->getState() == Process::FINISHED) {
                int totalLines = process->getCommandCounter();
                out << process->getName()
                    << "     (" << process->getFinishedTimestamp()
                    << ")     Finished     "
                    << totalLines << " / " << totalLines << "\n";
            }
        }
    }

    out << "------------------------------------\n";
    out << "\n";
    return out.str();
}