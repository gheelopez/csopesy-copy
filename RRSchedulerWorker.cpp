#include "RRSchedulerWorker.h"

RRSchedulerWorker::RRSchedulerWorker(int coreId)
    : coreId(coreId), running(false), busy(false), currentProcess(nullptr) {
}

RRSchedulerWorker::~RRSchedulerWorker() {
    stop();
}

void RRSchedulerWorker::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&RRSchedulerWorker::run, this);
        // Start a snapshot thread that runs alongside the worker thread
        snapshotThread = std::thread([this]() {
            while (running) {
                int currentTick = CPUTick::getInstance()->getTicks();
                int quantum = ConfigReader::getInstance()->getQuantum();
                if (currentTick % quantum == 0) {
                    MemoryManager::getInstance()->saveMemorySnapshot(currentTick);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // adjust as needed
            }
            });
    }
}

void RRSchedulerWorker::stop() {
    running = false;
    cv.notify_all();
    if (workerThread.joinable()) {
        workerThread.join();
    }
    if (snapshotThread.joinable()) {
        snapshotThread.join();
    }
}

void RRSchedulerWorker::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    currentProcess = process;
    busy = true;
    cv.notify_one();
}

bool RRSchedulerWorker::isBusy() const {
    return busy;
}

std::shared_ptr<Process> RRSchedulerWorker::getProcess() const {
    std::lock_guard<std::mutex> lock(mtx);
    return currentProcess;
}

void RRSchedulerWorker::run() {
    int delays_per_exec = ConfigReader::getInstance()->getDelays();
    int quantum = ConfigReader::getInstance()->getQuantum();

    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return currentProcess != nullptr || !running; });

        if (!running) break;

        auto process = currentProcess;
        int startTick = CPUTick::getInstance()->getTicks();
        int lastExecutedTick = startTick;
        bool executedAtLeastOnce = false;
        lock.unlock();

        while (process && !process->isFinished() && !preemptRequested) {
            int currentTick = CPUTick::getInstance()->getTicks();

            if (currentTick > lastExecutedTick) {
                process->executeCurrentCommand(coreId);
                lastExecutedTick = currentTick;
                executedAtLeastOnce = true;
            }

            // Preempt if quantum expired and we've executed at least once
            if (currentTick - startTick >= quantum && executedAtLeastOnce) {
                break;
            }

            std::this_thread::yield();
        }

        // Post-processing
        if (process && !process->isFinished()) {
            process->setState(Process::WAITING);
            GlobalProcessQueue::getInstance().push(process);
        } else if (process->isFinished()) {
            process->setState(Process::FINISHED);
            /*MemoryManager::getInstance()->unloadPagesForProcess(process);*/
		}

        lock.lock();
        currentProcess = nullptr;
        busy = false;
        preemptRequested = false;
        lock.unlock();
    }
}


void RRSchedulerWorker::preempt() {
    std::lock_guard<std::mutex> lock(mtx);
    if (currentProcess && !currentProcess->isFinished()) {
        currentProcess->setState(Process::WAITING);
    }
    preemptRequested = true;
}

bool RRSchedulerWorker::wasPreempted() const {
    return preemptRequested;
}
