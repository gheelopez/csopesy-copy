#include "AScheduler.h"

AScheduler::AScheduler() {}

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo)
	: schedulingAlgo(schedulingAlgo) {
}

std::shared_ptr<Process> AScheduler::findProcess(const std::string& processName) {
	for (const auto& process : processes) {
		if (process->getName() == processName) {
			return process;
		}
	}
	return nullptr;
}

void AScheduler::run() {
	init();
	while (this->running) {
		execute();
	}
}

void AScheduler::stop() {
	running = false;
}
