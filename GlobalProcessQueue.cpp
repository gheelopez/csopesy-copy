#include "GlobalProcessQueue.h"

GlobalProcessQueue::GlobalProcessQueue() {}

GlobalProcessQueue& GlobalProcessQueue::getInstance() {
    static GlobalProcessQueue instance;
    return instance;
}

void GlobalProcessQueue::push(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(queueMutex);
    processQueue.push(process);
}

std::shared_ptr<Process> GlobalProcessQueue::pop() {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (processQueue.empty()) return nullptr;
    std::shared_ptr<Process> front = processQueue.front();
    processQueue.pop();
    return front;
}

bool GlobalProcessQueue::empty() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return processQueue.empty();
}
