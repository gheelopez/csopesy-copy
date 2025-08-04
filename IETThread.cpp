#include "IETThread.h"

void IETThread::start() {
	std::thread thread(&IETThread::run, this);
	thread.detach();
}

void IETThread::sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}