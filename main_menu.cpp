#include <iostream>
#include <string>
#include <windows.h>
#include "ConsoleManager.h"
#include "CPUTick.h"
typedef std::string String;

// This is the main entry point of the application

int main() {
	bool running = true;
	ConsoleManager::initialize();
	CPUTick::getInstance()->startAutoTick(25); // ticks every 100 ms
	ConsoleManager::getInstance();
	
	while (running) {

		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		running = ConsoleManager::getInstance()->isRunning();
	}

	ConsoleManager::destroy(); // Cleanup the console manager before exiting
	CPUTick::destroy();

	return 0;
}

