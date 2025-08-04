#pragma once
#include "AConsole.h"
#include "Process.h"

class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName);
	void onEnabled() override;
	void process() override;
	void display() override;
	String getName() const;
	std::shared_ptr<Process> getProcess() const;

private:
	void printProcessInfo() const;
	std::shared_ptr<Process> attachedProcess;
	bool refreshed = false;
	String userInput;
	String commandInput;
};

