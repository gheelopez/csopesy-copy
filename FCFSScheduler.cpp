#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler() {}

FCFSScheduler::FCFSScheduler(int cores) : numCores(cores), schedulerRun(true) 
{
    init();
}

void FCFSScheduler::init() 
{
    std::lock_guard<std::mutex> lock(schedulerMutex);

    cpuWorkers.clear();
    while (!globalQueue.empty()) globalQueue.pop();  // clear global queue

    for (int i = 0; i < numCores; ++i) 
    {
        cpuWorkers.push_back(std::make_shared<SchedulerWorker>(i));
    }
}

void FCFSScheduler::run() 
{
    schedulerRun = true;
    schedulerThread = std::thread([this]() 
        {
        while (schedulerRun) 
        {
            execute();
        }
    });
}

void FCFSScheduler::stop() 
{
    schedulerRun = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void FCFSScheduler::execute() 
{
    std::lock_guard<std::mutex> lock(schedulerMutex);

    for (int core = 0; core < numCores; ++core) 
    {
        auto& worker = cpuWorkers[core];

        if (!worker->isBusy() && !globalQueue.empty()) 
        {
            auto process = globalQueue.front();
            globalQueue.pop();

            worker->assignProcess(process);
            process->setState(Process::RUNNING);
            worker->start();
        }
    }
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core) 
{
    std::lock_guard<std::mutex> lock(schedulerMutex);
    globalQueue.push(process);  // ignore core and just queue
    this->processList.push_back(process);
}

void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core) 
{
    addProcess(process, core);  // still uses global queue
}

std::shared_ptr<Process> FCFSScheduler::getProcess(int core) const 
{
    if (core >= 0 && core < numCores && cpuWorkers[core]->isBusy()) {
        return cpuWorkers[core]->getProcess();
    }
    return nullptr;
}

const std::string& FCFSScheduler::getProcessFromQueue(int index) const 
{
    static std::string empty = "";
    if (!globalQueue.empty()) {
        return globalQueue.front()->getName();  // Only returns front of global queue
    }
    return empty;
}

void FCFSScheduler::printCores() {
    for (int i = 0; i < numCores; ++i) 
    {
        std::cout << "Core " << i << ": ";
        if (cpuWorkers[i]->isBusy()) 
        {
            std::cout << cpuWorkers[i]->getProcess()->getName();
        } else {
            std::cout << "[Idle]";
        }
        std::cout << std::endl;
    }
}

// for debugging

void FCFSScheduler::printProcessQueues()
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

bool FCFSScheduler::allProcessesFinished() 
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

void FCFSScheduler::printRunningProcesses() 
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


void FCFSScheduler::printFinishedProcesses() 
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

String FCFSScheduler::screenLS() {
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