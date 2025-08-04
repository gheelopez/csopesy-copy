#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
typedef std::string String;

class Screen {
public:
	String screenName;
	int currentInstruction;
	int totalInstruction;
	const time_t timeCreated;

	Screen(String screenName) 
		: timeCreated(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
		{
		this->screenName = screenName;
		this->currentInstruction = 0;
		this->totalInstruction = 0;
	}

	void displayScreen();

private:
	void printTimestamp();
};
