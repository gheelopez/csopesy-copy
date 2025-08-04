#include "CPUTick.h"

CPUTick* CPUTick::sharedInstance = nullptr;

CPUTick::CPUTick() {
    this->nTicks = 0;
    ticking = false;
}

CPUTick::~CPUTick() {
    stopAutoTick();
}

CPUTick* CPUTick::getInstance() {
    if (sharedInstance == nullptr) {
        sharedInstance = new CPUTick();
    }
    return sharedInstance;
}

void CPUTick::destroy() {
    if (sharedInstance) {
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

void CPUTick::resetTicks() {
    this->nTicks = 0;
}

void CPUTick::addTick(int ticks) {
    this->nTicks += ticks;
}

int CPUTick::getTicks() const {
    return this->nTicks;
}

void CPUTick::startAutoTick(int intervalMs) {
    if (ticking) return; // already ticking

    ticking = true;
    tickThread = std::thread([this, intervalMs]() {
        while (ticking) {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            this->addTick(1);
        }
        });
}

void CPUTick::stopAutoTick() {
    ticking = false;
    if (tickThread.joinable()) {
        tickThread.join();
    }
}
