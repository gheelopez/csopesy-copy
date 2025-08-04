#include "SchedulerWorker.h"

SchedulerWorker::SchedulerWorker(int coreId)
    : coreId(coreId), running(false), busy(false), currentProcess(nullptr) {
}

SchedulerWorker::~SchedulerWorker() {
    stop();  // Ensure thread joins on destruction
}

void SchedulerWorker::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&SchedulerWorker::run, this);
    }
}

void SchedulerWorker::stop() {
    running = false;
    cv.notify_all();  // Wake thread if waiting
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void SchedulerWorker::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    currentProcess = process;
    busy = true;
    cv.notify_one();  // Wake run() thread
}

bool SchedulerWorker::isBusy() const {
    return busy;
}

std::shared_ptr<Process> SchedulerWorker::getProcess() const {
    std::lock_guard<std::mutex> lock(mtx);
    return currentProcess;
}

void SchedulerWorker::run() {
    int delays_per_exec = ConfigReader::getInstance()->getDelays();
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return currentProcess != nullptr || !running; });

        if (!running) break;

        auto process = currentProcess;  // Copy safely while holding lock
        lock.unlock();

        while (process && !process->isFinished()) {
            process->executeCurrentCommand(coreId);
        }

        process->setState(Process::FINISHED);

        lock.lock();
        currentProcess = nullptr;
        busy = false;
        lock.unlock();
    }
}
