#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <filesystem>
#include "AConsole.h"
#include "ConsoleManager.h"
#include "ConfigReader.h"
#include "GlobalScheduler.h"
#include "CPUTick.h"

typedef std::string String;
namespace fs = std::filesystem;

class MainConsole : public AConsole
{
public:
	MainConsole();
	MainConsole(String name);
	~MainConsole() = default;

	String getName() const;
	void onEnabled() override;
	void display() override;
	void process() override;

	String separateCommands(const std::string& str);

	void printHeader() const;

	String screenLS;

	friend class ConsoleManager;
};

